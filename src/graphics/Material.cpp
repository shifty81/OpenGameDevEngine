#include "ogde/graphics/Material.h"
#include <cstring>

namespace OGDE {
namespace Graphics {

Material::Material() = default;

Material::~Material() = default;

Material::Material(const Material& other)
    : name_(other.name_)
    , shininess_(other.shininess_)
    , opacity_(other.opacity_)
    , textures_(other.textures_)
{
    std::memcpy(diffuseColor_, other.diffuseColor_, sizeof(diffuseColor_));
    std::memcpy(ambientColor_, other.ambientColor_, sizeof(ambientColor_));
    std::memcpy(specularColor_, other.specularColor_, sizeof(specularColor_));
}

Material& Material::operator=(const Material& other) {
    if (this != &other) {
        name_ = other.name_;
        shininess_ = other.shininess_;
        opacity_ = other.opacity_;
        textures_ = other.textures_;
        
        std::memcpy(diffuseColor_, other.diffuseColor_, sizeof(diffuseColor_));
        std::memcpy(ambientColor_, other.ambientColor_, sizeof(ambientColor_));
        std::memcpy(specularColor_, other.specularColor_, sizeof(specularColor_));
    }
    return *this;
}

Material::Material(Material&& other) noexcept
    : name_(std::move(other.name_))
    , shininess_(other.shininess_)
    , opacity_(other.opacity_)
    , textures_(std::move(other.textures_))
{
    std::memcpy(diffuseColor_, other.diffuseColor_, sizeof(diffuseColor_));
    std::memcpy(ambientColor_, other.ambientColor_, sizeof(ambientColor_));
    std::memcpy(specularColor_, other.specularColor_, sizeof(specularColor_));
}

Material& Material::operator=(Material&& other) noexcept {
    if (this != &other) {
        name_ = std::move(other.name_);
        shininess_ = other.shininess_;
        opacity_ = other.opacity_;
        textures_ = std::move(other.textures_);
        
        std::memcpy(diffuseColor_, other.diffuseColor_, sizeof(diffuseColor_));
        std::memcpy(ambientColor_, other.ambientColor_, sizeof(ambientColor_));
        std::memcpy(specularColor_, other.specularColor_, sizeof(specularColor_));
    }
    return *this;
}

void Material::SetDiffuseColor(float r, float g, float b, float a) {
    diffuseColor_[0] = r;
    diffuseColor_[1] = g;
    diffuseColor_[2] = b;
    diffuseColor_[3] = a;
}

void Material::GetDiffuseColor(float& r, float& g, float& b, float& a) const {
    r = diffuseColor_[0];
    g = diffuseColor_[1];
    b = diffuseColor_[2];
    a = diffuseColor_[3];
}

void Material::SetAmbientColor(float r, float g, float b) {
    ambientColor_[0] = r;
    ambientColor_[1] = g;
    ambientColor_[2] = b;
}

void Material::GetAmbientColor(float& r, float& g, float& b) const {
    r = ambientColor_[0];
    g = ambientColor_[1];
    b = ambientColor_[2];
}

void Material::SetSpecular(float r, float g, float b, float shininess) {
    specularColor_[0] = r;
    specularColor_[1] = g;
    specularColor_[2] = b;
    shininess_ = shininess;
}

void Material::GetSpecular(float& r, float& g, float& b, float& shininess) const {
    r = specularColor_[0];
    g = specularColor_[1];
    b = specularColor_[2];
    shininess = shininess_;
}

void Material::SetTexture(TextureType type, std::shared_ptr<Texture> texture) {
    if (texture) {
        textures_[type] = texture;
    } else {
        textures_.erase(type);
    }
}

std::shared_ptr<Texture> Material::GetTexture(TextureType type) const {
    auto it = textures_.find(type);
    if (it != textures_.end()) {
        return it->second;
    }
    return nullptr;
}

bool Material::HasTexture(TextureType type) const {
    return textures_.find(type) != textures_.end();
}

void Material::RemoveTexture(TextureType type) {
    textures_.erase(type);
}

} // namespace Graphics
} // namespace OGDE
