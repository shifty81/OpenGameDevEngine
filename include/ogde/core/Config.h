#pragma once

#include <string>
#include <optional>
#include <unordered_map>
#include <memory>

namespace OGDE {
namespace Core {

/**
 * @brief Configuration management system
 * 
 * Provides JSON-based configuration management with support for:
 * - Loading/saving configuration files
 * - Type-safe parameter access
 * - Default values
 * - Nested configuration sections
 */
class Config {
public:
    Config();
    ~Config();
    
    /**
     * @brief Load configuration from a JSON file
     * @param filepath Path to the JSON configuration file
     * @return true if loaded successfully, false otherwise
     */
    bool LoadFromFile(const std::string& filepath);
    
    /**
     * @brief Save configuration to a JSON file
     * @param filepath Path to save the configuration file
     * @param pretty If true, format JSON with indentation (default: true)
     * @return true if saved successfully, false otherwise
     */
    bool SaveToFile(const std::string& filepath, bool pretty = true) const;
    
    /**
     * @brief Load configuration from a JSON string
     * @param jsonString JSON string to parse
     * @return true if parsed successfully, false otherwise
     */
    bool LoadFromString(const std::string& jsonString);
    
    /**
     * @brief Convert configuration to a JSON string
     * @param pretty If true, format JSON with indentation (default: true)
     * @return JSON string representation
     */
    std::string ToString(bool pretty = true) const;
    
    /**
     * @brief Get a string value from the configuration
     * @param key Configuration key (supports dot notation, e.g., "graphics.resolution")
     * @param defaultValue Default value if key doesn't exist
     * @return Value or default value
     */
    std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
    
    /**
     * @brief Get an integer value from the configuration
     * @param key Configuration key
     * @param defaultValue Default value if key doesn't exist
     * @return Value or default value
     */
    int GetInt(const std::string& key, int defaultValue = 0) const;
    
    /**
     * @brief Get a floating-point value from the configuration
     * @param key Configuration key
     * @param defaultValue Default value if key doesn't exist
     * @return Value or default value
     */
    float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
    
    /**
     * @brief Get a boolean value from the configuration
     * @param key Configuration key
     * @param defaultValue Default value if key doesn't exist
     * @return Value or default value
     */
    bool GetBool(const std::string& key, bool defaultValue = false) const;
    
    /**
     * @brief Set a string value in the configuration
     * @param key Configuration key (supports dot notation)
     * @param value Value to set
     */
    void SetString(const std::string& key, const std::string& value);
    
    /**
     * @brief Set an integer value in the configuration
     * @param key Configuration key
     * @param value Value to set
     */
    void SetInt(const std::string& key, int value);
    
    /**
     * @brief Set a floating-point value in the configuration
     * @param key Configuration key
     * @param value Value to set
     */
    void SetFloat(const std::string& key, float value);
    
    /**
     * @brief Set a boolean value in the configuration
     * @param key Configuration key
     * @param value Value to set
     */
    void SetBool(const std::string& key, bool value);
    
    /**
     * @brief Check if a key exists in the configuration
     * @param key Configuration key
     * @return true if key exists, false otherwise
     */
    bool HasKey(const std::string& key) const;
    
    /**
     * @brief Remove a key from the configuration
     * @param key Configuration key to remove
     * @return true if key was removed, false if it didn't exist
     */
    bool RemoveKey(const std::string& key);
    
    /**
     * @brief Clear all configuration data
     */
    void Clear();
    
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    void EnsureImpl();
};

} // namespace Core
} // namespace OGDE
