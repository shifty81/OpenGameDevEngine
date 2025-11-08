#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <optional>
#include <cstdint>

namespace OGDE {
namespace Core {

/**
 * @brief File system utilities for reading and writing files
 * 
 * Provides cross-platform file I/O operations including:
 * - Reading and writing text files
 * - Reading and writing binary files
 * - Path manipulation
 * - File/directory existence checks
 * - Directory creation
 */
class FileSystem {
public:
    /**
     * @brief Read entire text file into a string
     * @param filepath Path to the file to read
     * @return File contents as a string, or empty optional if failed
     */
    static std::optional<std::string> ReadTextFile(const std::string& filepath);

    /**
     * @brief Write text to a file
     * @param filepath Path to the file to write
     * @param content Text content to write
     * @param append If true, append to file; if false, overwrite
     * @return true if successful, false otherwise
     */
    static bool WriteTextFile(const std::string& filepath, const std::string& content, bool append = false);

    /**
     * @brief Read entire binary file into a byte vector
     * @param filepath Path to the file to read
     * @return File contents as byte vector, or empty optional if failed
     */
    static std::optional<std::vector<uint8_t>> ReadBinaryFile(const std::string& filepath);

    /**
     * @brief Write binary data to a file
     * @param filepath Path to the file to write
     * @param data Binary data to write
     * @return true if successful, false otherwise
     */
    static bool WriteBinaryFile(const std::string& filepath, const std::vector<uint8_t>& data);

    /**
     * @brief Check if a file exists
     * @param filepath Path to check
     * @return true if file exists, false otherwise
     */
    static bool FileExists(const std::string& filepath);

    /**
     * @brief Check if a directory exists
     * @param dirpath Path to check
     * @return true if directory exists, false otherwise
     */
    static bool DirectoryExists(const std::string& dirpath);

    /**
     * @brief Create a directory (and parent directories if needed)
     * @param dirpath Path to create
     * @return true if successful or already exists, false otherwise
     */
    static bool CreateDirectory(const std::string& dirpath);

    /**
     * @brief Get the directory portion of a file path
     * @param filepath Full file path
     * @return Directory path (without trailing separator)
     */
    static std::string GetDirectory(const std::string& filepath);

    /**
     * @brief Get the filename portion of a file path (without directory)
     * @param filepath Full file path
     * @return Filename with extension
     */
    static std::string GetFilename(const std::string& filepath);

    /**
     * @brief Get the file extension (e.g., ".txt", ".json")
     * @param filepath Full file path
     * @return File extension including the dot, or empty string if no extension
     */
    static std::string GetExtension(const std::string& filepath);

    /**
     * @brief Join two path components with the appropriate separator
     * @param path1 First path component
     * @param path2 Second path component
     * @return Combined path
     */
    static std::string JoinPath(const std::string& path1, const std::string& path2);

    /**
     * @brief Normalize a path (convert to platform-specific separators)
     * @param path Path to normalize
     * @return Normalized path
     */
    static std::string NormalizePath(const std::string& path);

    /**
     * @brief Get the platform-specific path separator
     * @return Path separator character ('/' or '\\')
     */
    static char GetPathSeparator();
};

} // namespace Core
} // namespace OGDE
