#include "ogde/core/Config.h"
#include "ogde/core/FileSystem.h"
#include "ogde/core/Logger.h"
#include "../../external/json.hpp"

using json = nlohmann::json;

namespace OGDE {
namespace Core {

// Private implementation (PIMPL pattern)
class Config::Impl {
public:
    json data;
};

void Config::EnsureImpl() {
    if (!pImpl) {
        pImpl = std::make_unique<Impl>();
    }
}

Config::Config() = default;

Config::~Config() = default;

bool Config::LoadFromFile(const std::string& filepath) {
    EnsureImpl();
    
    auto content = FileSystem::ReadTextFile(filepath);
    if (!content.has_value()) {
        ogde::core::Logger::error("Failed to read config file: " + filepath);
        return false;
    }
    
    return LoadFromString(content.value());
}

bool Config::SaveToFile(const std::string& filepath, bool pretty) const {
    if (!pImpl) {
        ogde::core::Logger::warning("Attempting to save empty configuration");
        return FileSystem::WriteTextFile(filepath, "{}");
    }
    
    std::string jsonString = ToString(pretty);
    return FileSystem::WriteTextFile(filepath, jsonString);
}

bool Config::LoadFromString(const std::string& jsonString) {
    EnsureImpl();
    
    try {
        pImpl->data = json::parse(jsonString);
        return true;
    } catch (const json::parse_error& e) {
        ogde::core::Logger::error("JSON parse error: " + std::string(e.what()));
        return false;
    }
}

std::string Config::ToString(bool pretty) const {
    if (!pImpl) {
        return "{}";
    }
    
    return pImpl->data.dump(pretty ? 4 : -1);
}

// Helper to navigate nested keys using dot notation
static json* GetNestedValue(json& data, const std::string& key) {
    if (key.find('.') == std::string::npos) {
        // Simple key
        if (data.contains(key)) {
            return &data[key];
        }
        return nullptr;
    }
    
    // Nested key - split by dots
    json* current = &data;
    std::string remaining = key;
    
    while (!remaining.empty()) {
        size_t pos = remaining.find('.');
        std::string part = (pos == std::string::npos) ? remaining : remaining.substr(0, pos);
        
        if (!current->contains(part)) {
            return nullptr;
        }
        
        current = &(*current)[part];
        
        if (pos == std::string::npos) {
            break;
        }
        remaining = remaining.substr(pos + 1);
    }
    
    return current;
}

static const json* GetNestedValue(const json& data, const std::string& key) {
    if (key.find('.') == std::string::npos) {
        if (data.contains(key)) {
            return &data[key];
        }
        return nullptr;
    }
    
    const json* current = &data;
    std::string remaining = key;
    
    while (!remaining.empty()) {
        size_t pos = remaining.find('.');
        std::string part = (pos == std::string::npos) ? remaining : remaining.substr(0, pos);
        
        if (!current->contains(part)) {
            return nullptr;
        }
        
        current = &(*current)[part];
        
        if (pos == std::string::npos) {
            break;
        }
        remaining = remaining.substr(pos + 1);
    }
    
    return current;
}

static void SetNestedValue(json& data, const std::string& key, const json& value) {
    if (key.find('.') == std::string::npos) {
        data[key] = value;
        return;
    }
    
    // Nested key - create path if needed
    json* current = &data;
    std::string remaining = key;
    
    while (!remaining.empty()) {
        size_t pos = remaining.find('.');
        std::string part = (pos == std::string::npos) ? remaining : remaining.substr(0, pos);
        
        if (pos == std::string::npos) {
            // Last part - set the value
            (*current)[part] = value;
            break;
        }
        
        // Not last part - ensure object exists
        if (!current->contains(part) || !(*current)[part].is_object()) {
            (*current)[part] = json::object();
        }
        
        current = &(*current)[part];
        remaining = remaining.substr(pos + 1);
    }
}

std::string Config::GetString(const std::string& key, const std::string& defaultValue) const {
    if (!pImpl) return defaultValue;
    
    const json* value = GetNestedValue(pImpl->data, key);
    if (!value || !value->is_string()) {
        return defaultValue;
    }
    
    return value->get<std::string>();
}

int Config::GetInt(const std::string& key, int defaultValue) const {
    if (!pImpl) return defaultValue;
    
    const json* value = GetNestedValue(pImpl->data, key);
    if (!value || !value->is_number_integer()) {
        return defaultValue;
    }
    
    return value->get<int>();
}

float Config::GetFloat(const std::string& key, float defaultValue) const {
    if (!pImpl) return defaultValue;
    
    const json* value = GetNestedValue(pImpl->data, key);
    if (!value || !value->is_number()) {
        return defaultValue;
    }
    
    return value->get<float>();
}

bool Config::GetBool(const std::string& key, bool defaultValue) const {
    if (!pImpl) return defaultValue;
    
    const json* value = GetNestedValue(pImpl->data, key);
    if (!value || !value->is_boolean()) {
        return defaultValue;
    }
    
    return value->get<bool>();
}

void Config::SetString(const std::string& key, const std::string& value) {
    EnsureImpl();
    SetNestedValue(pImpl->data, key, value);
}

void Config::SetInt(const std::string& key, int value) {
    EnsureImpl();
    SetNestedValue(pImpl->data, key, value);
}

void Config::SetFloat(const std::string& key, float value) {
    EnsureImpl();
    SetNestedValue(pImpl->data, key, value);
}

void Config::SetBool(const std::string& key, bool value) {
    EnsureImpl();
    SetNestedValue(pImpl->data, key, value);
}

bool Config::HasKey(const std::string& key) const {
    if (!pImpl) return false;
    return GetNestedValue(pImpl->data, key) != nullptr;
}

bool Config::RemoveKey(const std::string& key) {
    if (!pImpl) return false;
    
    if (key.find('.') == std::string::npos) {
        if (pImpl->data.contains(key)) {
            pImpl->data.erase(key);
            return true;
        }
        return false;
    }
    
    // For nested keys, we need to navigate to parent and remove child
    size_t lastDot = key.rfind('.');
    std::string parentKey = key.substr(0, lastDot);
    std::string childKey = key.substr(lastDot + 1);
    
    json* parent = GetNestedValue(pImpl->data, parentKey);
    if (!parent || !parent->is_object()) {
        return false;
    }
    
    if (parent->contains(childKey)) {
        parent->erase(childKey);
        return true;
    }
    
    return false;
}

void Config::Clear() {
    if (pImpl) {
        pImpl->data.clear();
    }
}

} // namespace Core
} // namespace OGDE
