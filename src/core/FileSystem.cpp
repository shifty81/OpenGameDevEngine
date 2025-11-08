#include "ogde/core/FileSystem.h"
#include "ogde/core/Logger.h"
#include <filesystem>
#include <sstream>

namespace OGDE {
namespace Core {

namespace fs = std::filesystem;

std::optional<std::string> FileSystem::ReadTextFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in);
    if (!file.is_open()) {
        ogde::core::Logger::error("Failed to open file for reading: " + filepath);
        return std::nullopt;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

bool FileSystem::WriteTextFile(const std::string& filepath, const std::string& content, bool append) {
    std::ios_base::openmode mode = std::ios::out;
    if (append) {
        mode |= std::ios::app;
    }

    std::ofstream file(filepath, mode);
    if (!file.is_open()) {
        ogde::core::Logger::error("Failed to open file for writing: " + filepath);
        return false;
    }

    file << content;
    file.close();

    return true;
}

std::optional<std::vector<uint8_t>> FileSystem::ReadBinaryFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        ogde::core::Logger::error("Failed to open binary file for reading: " + filepath);
        return std::nullopt;
    }

    // Get file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read file into vector
    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        ogde::core::Logger::error("Failed to read binary file: " + filepath);
        return std::nullopt;
    }

    file.close();
    return buffer;
}

bool FileSystem::WriteBinaryFile(const std::string& filepath, const std::vector<uint8_t>& data) {
    std::ofstream file(filepath, std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        ogde::core::Logger::error("Failed to open binary file for writing: " + filepath);
        return false;
    }

    file.write(reinterpret_cast<const char*>(data.data()), data.size());
    file.close();

    return true;
}

bool FileSystem::FileExists(const std::string& filepath) {
    try {
        return fs::exists(filepath) && fs::is_regular_file(filepath);
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error checking file existence: " + std::string(e.what()));
        return false;
    }
}

bool FileSystem::DirectoryExists(const std::string& dirpath) {
    try {
        return fs::exists(dirpath) && fs::is_directory(dirpath);
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error checking directory existence: " + std::string(e.what()));
        return false;
    }
}

bool FileSystem::CreateDirectory(const std::string& dirpath) {
    try {
        if (DirectoryExists(dirpath)) {
            return true;
        }
        return fs::create_directories(dirpath);
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error creating directory: " + std::string(e.what()));
        return false;
    }
}

std::string FileSystem::GetDirectory(const std::string& filepath) {
    try {
        fs::path p(filepath);
        if (p.has_parent_path()) {
            return p.parent_path().string();
        }
        return "";
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error getting directory: " + std::string(e.what()));
        return "";
    }
}

std::string FileSystem::GetFilename(const std::string& filepath) {
    try {
        fs::path p(filepath);
        return p.filename().string();
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error getting filename: " + std::string(e.what()));
        return "";
    }
}

std::string FileSystem::GetExtension(const std::string& filepath) {
    try {
        fs::path p(filepath);
        return p.extension().string();
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error getting extension: " + std::string(e.what()));
        return "";
    }
}

std::string FileSystem::JoinPath(const std::string& path1, const std::string& path2) {
    try {
        fs::path p1(path1);
        fs::path p2(path2);
        return (p1 / p2).string();
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error joining paths: " + std::string(e.what()));
        return path1 + GetPathSeparator() + path2;
    }
}

std::string FileSystem::NormalizePath(const std::string& path) {
    try {
        fs::path p(path);
        return p.lexically_normal().string();
    } catch (const fs::filesystem_error& e) {
        ogde::core::Logger::error("Error normalizing path: " + std::string(e.what()));
        return path;
    }
}

char FileSystem::GetPathSeparator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

} // namespace Core
} // namespace OGDE
