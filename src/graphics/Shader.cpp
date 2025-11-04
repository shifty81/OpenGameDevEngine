/**
 * Shader Implementation
 */

#ifdef _WIN32

#include "ogde/graphics/Shader.h"
#include "ogde/core/Logger.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace ogde {
namespace graphics {

Shader::Shader() {
}

Shader::~Shader() {
}

bool Shader::createFromSource(
    ID3D11Device* device,
    const std::string& vertexShaderSource,
    const std::string& pixelShaderSource,
    const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc,
    UINT numElements)
{
    if (!device) {
        core::Logger::error("Shader::createFromSource: device is null");
        return false;
    }

    // Compile vertex shader
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
    if (!compileShader(vertexShaderSource, "VSMain", "vs_5_0", vsBlob.GetAddressOf())) {
        core::Logger::error("Failed to compile vertex shader");
        return false;
    }

    // Create vertex shader
    HRESULT hr = device->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        m_vertexShader.GetAddressOf()
    );
    if (FAILED(hr)) {
        core::Logger::error("Failed to create vertex shader");
        return false;
    }

    // Create input layout
    if (inputLayoutDesc && numElements > 0) {
        hr = device->CreateInputLayout(
            inputLayoutDesc,
            numElements,
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            m_inputLayout.GetAddressOf()
        );
        if (FAILED(hr)) {
            core::Logger::error("Failed to create input layout");
            return false;
        }
    }

    // Compile pixel shader
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
    if (!compileShader(pixelShaderSource, "PSMain", "ps_5_0", psBlob.GetAddressOf())) {
        core::Logger::error("Failed to compile pixel shader");
        return false;
    }

    // Create pixel shader
    hr = device->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        m_pixelShader.GetAddressOf()
    );
    if (FAILED(hr)) {
        core::Logger::error("Failed to create pixel shader");
        return false;
    }

    core::Logger::info("Shader compiled and created successfully");
    return true;
}

void Shader::bind(ID3D11DeviceContext* deviceContext) {
    if (!deviceContext) {
        return;
    }

    deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    if (m_inputLayout) {
        deviceContext->IASetInputLayout(m_inputLayout.Get());
    }
}

bool Shader::compileShader(
    const std::string& source,
    const char* entryPoint,
    const char* target,
    ID3DBlob** blob)
{
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
    
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    flags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr = D3DCompile(
        source.c_str(),
        source.size(),
        nullptr,
        nullptr,
        nullptr,
        entryPoint,
        target,
        flags,
        0,
        blob,
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            std::string errorMsg = static_cast<const char*>(errorBlob->GetBufferPointer());
            core::Logger::error("Shader compilation error: " + errorMsg);
        }
        return false;
    }

    return true;
}

} // namespace graphics
} // namespace ogde

#endif // _WIN32
