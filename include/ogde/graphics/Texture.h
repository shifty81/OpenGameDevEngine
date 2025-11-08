#pragma once

#ifdef _WIN32
#include <d3d11.h>
#include <wrl/client.h>
#endif

#include <string>
#include <cstdint>
#include <memory>

namespace OGDE {
namespace Graphics {

/**
 * @brief Texture resource for graphics rendering
 * 
 * Handles loading and management of 2D textures with support for:
 * - Multiple image formats (PNG, JPG, BMP, etc. via stb_image)
 * - Automatic mipmap generation
 * - DirectX 11 shader resource views
 */
class Texture {
public:
    Texture();
    ~Texture();

    // Disable copy
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    // Allow move
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;

    /**
     * @brief Load texture from file
     * @param filepath Path to image file
     * @return true if loaded successfully
     */
    bool LoadFromFile(const std::string& filepath);

    /**
     * @brief Load texture from memory
     * @param data Raw image data
     * @param width Image width
     * @param height Image height
     * @param channels Number of color channels (3=RGB, 4=RGBA)
     * @return true if loaded successfully
     */
    bool LoadFromMemory(const uint8_t* data, int width, int height, int channels);

    /**
     * @brief Initialize DirectX 11 texture resources
     * @param device DirectX 11 device
     * @return true if initialized successfully
     */
#ifdef _WIN32
    bool InitializeD3D11(ID3D11Device* device);
#endif

    /**
     * @brief Release all resources
     */
    void Release();

    /**
     * @brief Get texture width
     * @return Width in pixels
     */
    int GetWidth() const { return width_; }

    /**
     * @brief Get texture height
     * @return Height in pixels
     */
    int GetHeight() const { return height_; }

    /**
     * @brief Get number of channels
     * @return Channel count
     */
    int GetChannels() const { return channels_; }

    /**
     * @brief Check if texture is loaded
     * @return true if texture has valid data
     */
    bool IsLoaded() const { return data_ != nullptr; }

#ifdef _WIN32
    /**
     * @brief Get DirectX 11 shader resource view
     * @return Shader resource view
     */
    ID3D11ShaderResourceView* GetShaderResourceView() const { 
        return shaderResourceView_.Get(); 
    }

    /**
     * @brief Get DirectX 11 texture
     * @return Texture 2D resource
     */
    ID3D11Texture2D* GetTexture() const { 
        return texture_.Get(); 
    }
#endif

private:
    int width_ = 0;
    int height_ = 0;
    int channels_ = 0;
    uint8_t* data_ = nullptr;
    std::string filepath_;

#ifdef _WIN32
    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture_;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView_;
#endif

    void FreeImageData();
};

} // namespace Graphics
} // namespace OGDE
