#include "ogde/graphics/Texture.h"
#include "ogde/core/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../external/stb_image.h"

namespace OGDE {
namespace Graphics {

Texture::Texture() = default;

Texture::~Texture() {
    Release();
}

Texture::Texture(Texture&& other) noexcept
    : width_(other.width_)
    , height_(other.height_)
    , channels_(other.channels_)
    , data_(other.data_)
    , filepath_(std::move(other.filepath_))
#ifdef _WIN32
    , texture_(std::move(other.texture_))
    , shaderResourceView_(std::move(other.shaderResourceView_))
#endif
{
    other.data_ = nullptr;
    other.width_ = 0;
    other.height_ = 0;
    other.channels_ = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        Release();
        
        width_ = other.width_;
        height_ = other.height_;
        channels_ = other.channels_;
        data_ = other.data_;
        filepath_ = std::move(other.filepath_);
        
#ifdef _WIN32
        texture_ = std::move(other.texture_);
        shaderResourceView_ = std::move(other.shaderResourceView_);
#endif
        
        other.data_ = nullptr;
        other.width_ = 0;
        other.height_ = 0;
        other.channels_ = 0;
    }
    return *this;
}

bool Texture::LoadFromFile(const std::string& filepath) {
    FreeImageData();
    
    // Use stb_image to load the texture
    int width, height, channels;
    data_ = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    
    if (!data_) {
        ogde::core::Logger::error("Failed to load texture: " + filepath);
        return false;
    }
    
    width_ = width;
    height_ = height;
    channels_ = channels;
    filepath_ = filepath;
    
    ogde::core::Logger::info("Loaded texture: " + filepath + 
                             " (" + std::to_string(width) + "x" + std::to_string(height) + 
                             ", " + std::to_string(channels) + " channels)");
    
    return true;
}

bool Texture::LoadFromMemory(const uint8_t* data, int width, int height, int channels) {
    if (!data || width <= 0 || height <= 0 || channels <= 0) {
        ogde::core::Logger::error("Invalid texture parameters");
        return false;
    }
    
    FreeImageData();
    
    size_t dataSize = width * height * channels;
    data_ = new uint8_t[dataSize];
    memcpy(data_, data, dataSize);
    
    width_ = width;
    height_ = height;
    channels_ = channels;
    filepath_ = "(memory)";
    
    return true;
}

#ifdef _WIN32
bool Texture::InitializeD3D11(ID3D11Device* device) {
    if (!device || !data_) {
        ogde::core::Logger::error("Invalid device or texture data");
        return false;
    }
    
    // Release existing resources
    texture_.Reset();
    shaderResourceView_.Reset();
    
    // Determine DXGI format based on channels
    DXGI_FORMAT format;
    switch (channels_) {
        case 1:
            format = DXGI_FORMAT_R8_UNORM;
            break;
        case 2:
            format = DXGI_FORMAT_R8G8_UNORM;
            break;
        case 3:
            // RGB needs to be converted to RGBA
            format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        case 4:
            format = DXGI_FORMAT_R8G8B8A8_UNORM;
            break;
        default:
            ogde::core::Logger::error("Unsupported texture channel count: " + std::to_string(channels_));
            return false;
    }
    
    // Prepare texture data (convert RGB to RGBA if needed)
    std::vector<uint8_t> textureData;
    const uint8_t* dataToUse = data_;
    
    if (channels_ == 3) {
        // Convert RGB to RGBA
        size_t pixelCount = width_ * height_;
        textureData.resize(pixelCount * 4);
        
        for (size_t i = 0; i < pixelCount; ++i) {
            textureData[i * 4 + 0] = data_[i * 3 + 0]; // R
            textureData[i * 4 + 1] = data_[i * 3 + 1]; // G
            textureData[i * 4 + 2] = data_[i * 3 + 2]; // B
            textureData[i * 4 + 3] = 255;               // A
        }
        
        dataToUse = textureData.data();
    }
    
    // Create texture description
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width_;
    texDesc.Height = height_;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = format;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = 0;
    texDesc.MiscFlags = 0;
    
    // Create subresource data
    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = dataToUse;
    initData.SysMemPitch = width_ * (channels_ == 3 ? 4 : channels_);
    initData.SysMemSlicePitch = 0;
    
    // Create the texture
    HRESULT hr = device->CreateTexture2D(&texDesc, &initData, texture_.GetAddressOf());
    if (FAILED(hr)) {
        ogde::core::Logger::error("Failed to create D3D11 texture");
        return false;
    }
    
    // Create shader resource view description
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    
    // Create the shader resource view
    hr = device->CreateShaderResourceView(texture_.Get(), &srvDesc, shaderResourceView_.GetAddressOf());
    if (FAILED(hr)) {
        ogde::core::Logger::error("Failed to create shader resource view");
        texture_.Reset();
        return false;
    }
    
    ogde::core::Logger::info("Initialized D3D11 texture: " + filepath_);
    return true;
}
#endif

void Texture::Release() {
    FreeImageData();
    
#ifdef _WIN32
    shaderResourceView_.Reset();
    texture_.Reset();
#endif
    
    width_ = 0;
    height_ = 0;
    channels_ = 0;
    filepath_.clear();
}

void Texture::FreeImageData() {
    if (data_) {
        stbi_image_free(data_);
        data_ = nullptr;
    }
}

} // namespace Graphics
} // namespace OGDE
