#pragma once

#include "ogde/graphics/Texture.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace OGDE {
namespace Graphics {

/**
 * @brief Material properties and textures
 * 
 * Manages material properties for rendering including:
 * - Diffuse color
 * - Ambient color
 * - Specular properties
 * - Texture maps (diffuse, normal, specular, etc.)
 */
class Material {
public:
    /**
     * @brief Material texture types
     */
    enum class TextureType {
        Diffuse,    ///< Base color texture
        Normal,     ///< Normal map for surface detail
        Specular,   ///< Specular/glossiness map
        Emissive,   ///< Emissive/glow map
        Ambient,    ///< Ambient occlusion map
        Height      ///< Height/displacement map
    };

    Material();
    ~Material();

    // Allow copy and move
    Material(const Material& other);
    Material& operator=(const Material& other);
    Material(Material&& other) noexcept;
    Material& operator=(Material&& other) noexcept;

    /**
     * @brief Set material name
     * @param name Material name
     */
    void SetName(const std::string& name) { name_ = name; }

    /**
     * @brief Get material name
     * @return Material name
     */
    const std::string& GetName() const { return name_; }

    /**
     * @brief Set diffuse color (RGBA, 0-1 range)
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     * @param a Alpha component
     */
    void SetDiffuseColor(float r, float g, float b, float a = 1.0f);

    /**
     * @brief Get diffuse color
     * @param r Red component output
     * @param g Green component output
     * @param b Blue component output
     * @param a Alpha component output
     */
    void GetDiffuseColor(float& r, float& g, float& b, float& a) const;

    /**
     * @brief Set ambient color (RGB, 0-1 range)
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     */
    void SetAmbientColor(float r, float g, float b);

    /**
     * @brief Get ambient color
     * @param r Red component output
     * @param g Green component output
     * @param b Blue component output
     */
    void GetAmbientColor(float& r, float& g, float& b) const;

    /**
     * @brief Set specular properties
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     * @param shininess Shininess exponent (higher = more focused highlight)
     */
    void SetSpecular(float r, float g, float b, float shininess);

    /**
     * @brief Get specular properties
     * @param r Red component output
     * @param g Green component output
     * @param b Blue component output
     * @param shininess Shininess exponent output
     */
    void GetSpecular(float& r, float& g, float& b, float& shininess) const;

    /**
     * @brief Set a texture for this material
     * @param type Texture type
     * @param texture Texture to set (shared ownership)
     */
    void SetTexture(TextureType type, std::shared_ptr<Texture> texture);

    /**
     * @brief Get a texture from this material
     * @param type Texture type
     * @return Texture pointer or nullptr if not set
     */
    std::shared_ptr<Texture> GetTexture(TextureType type) const;

    /**
     * @brief Check if material has a specific texture type
     * @param type Texture type
     * @return true if texture is set
     */
    bool HasTexture(TextureType type) const;

    /**
     * @brief Remove a texture from this material
     * @param type Texture type to remove
     */
    void RemoveTexture(TextureType type);

    /**
     * @brief Set opacity (0 = transparent, 1 = opaque)
     * @param opacity Opacity value
     */
    void SetOpacity(float opacity) { opacity_ = opacity; }

    /**
     * @brief Get opacity
     * @return Opacity value
     */
    float GetOpacity() const { return opacity_; }

private:
    std::string name_;
    
    // Color properties
    float diffuseColor_[4] = {1.0f, 1.0f, 1.0f, 1.0f};  // RGBA
    float ambientColor_[3] = {0.2f, 0.2f, 0.2f};        // RGB
    float specularColor_[3] = {1.0f, 1.0f, 1.0f};       // RGB
    float shininess_ = 32.0f;
    float opacity_ = 1.0f;
    
    // Textures
    std::unordered_map<TextureType, std::shared_ptr<Texture>> textures_;
};

} // namespace Graphics
} // namespace OGDE
