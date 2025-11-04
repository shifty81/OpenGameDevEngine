/**
 * @file Shader.h
 * @brief Shader management system
 */

#ifndef OGDE_GRAPHICS_SHADER_H
#define OGDE_GRAPHICS_SHADER_H

#ifdef _WIN32

#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <vector>

namespace ogde {
namespace graphics {

/**
 * @class Shader
 * @brief Manages vertex and pixel shaders
 */
class Shader {
public:
    Shader();
    ~Shader();

    /**
     * @brief Compile and create vertex and pixel shaders from source code
     * @param device DirectX device
     * @param vertexShaderSource Vertex shader source code (HLSL)
     * @param pixelShaderSource Pixel shader source code (HLSL)
     * @param inputLayoutDesc Input layout description
     * @param numElements Number of elements in the input layout
     * @return true if successful
     */
    bool createFromSource(
        ID3D11Device* device,
        const std::string& vertexShaderSource,
        const std::string& pixelShaderSource,
        const D3D11_INPUT_ELEMENT_DESC* inputLayoutDesc,
        UINT numElements
    );

    /**
     * @brief Bind the shader for rendering
     * @param deviceContext DirectX device context
     */
    void bind(ID3D11DeviceContext* deviceContext);

    /**
     * @brief Get the vertex shader
     */
    ID3D11VertexShader* getVertexShader() const { return m_vertexShader.Get(); }

    /**
     * @brief Get the pixel shader
     */
    ID3D11PixelShader* getPixelShader() const { return m_pixelShader.Get(); }

    /**
     * @brief Get the input layout
     */
    ID3D11InputLayout* getInputLayout() const { return m_inputLayout.Get(); }

private:
    /**
     * @brief Compile shader from source code
     * @param source Shader source code
     * @param entryPoint Entry point function name
     * @param target Shader target (e.g., "vs_5_0", "ps_5_0")
     * @param blob Output compiled shader blob
     * @return true if successful
     */
    bool compileShader(
        const std::string& source,
        const char* entryPoint,
        const char* target,
        ID3DBlob** blob
    );

    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};

} // namespace graphics
} // namespace ogde

#endif // _WIN32

#endif // OGDE_GRAPHICS_SHADER_H
