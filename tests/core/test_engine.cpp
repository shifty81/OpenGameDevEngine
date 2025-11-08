/**
 * Core module tests
 * Tests for FileSystem, Config, and other core components
 */

#include "ogde/core/FileSystem.h"
#include "ogde/core/Config.h"
#include <iostream>
#include <cassert>
#include <cmath>

using namespace OGDE::Core;

void TestTextFileReadWrite() {
    std::cout << "Testing text file read/write..." << std::endl;
    
    const std::string testFile = "/tmp/test_file.txt";
    const std::string testContent = "Hello, OpenGameDevEngine!";
    
    // Write test
    bool writeSuccess = FileSystem::WriteTextFile(testFile, testContent);
    assert(writeSuccess && "Failed to write text file");
    
    // Read test
    auto readContent = FileSystem::ReadTextFile(testFile);
    assert(readContent.has_value() && "Failed to read text file");
    assert(readContent.value() == testContent && "Content mismatch");
    
    std::cout << "  ✓ Text file read/write passed" << std::endl;
}

void TestBinaryFileReadWrite() {
    std::cout << "Testing binary file read/write..." << std::endl;
    
    const std::string testFile = "/tmp/test_binary.bin";
    std::vector<uint8_t> testData = {0x00, 0x01, 0x02, 0x03, 0xFF};
    
    // Write test
    bool writeSuccess = FileSystem::WriteBinaryFile(testFile, testData);
    assert(writeSuccess && "Failed to write binary file");
    
    // Read test
    auto readData = FileSystem::ReadBinaryFile(testFile);
    assert(readData.has_value() && "Failed to read binary file");
    assert(readData.value() == testData && "Binary data mismatch");
    
    std::cout << "  ✓ Binary file read/write passed" << std::endl;
}

void TestFileExists() {
    std::cout << "Testing file existence checks..." << std::endl;
    
    const std::string existingFile = "/tmp/existing_file.txt";
    const std::string nonExistingFile = "/tmp/nonexistent_file.txt";
    
    // Create a file
    FileSystem::WriteTextFile(existingFile, "test");
    
    // Test existence
    assert(FileSystem::FileExists(existingFile) && "File should exist");
    assert(!FileSystem::FileExists(nonExistingFile) && "File should not exist");
    
    std::cout << "  ✓ File existence checks passed" << std::endl;
}

void TestDirectoryOperations() {
    std::cout << "Testing directory operations..." << std::endl;
    
    const std::string testDir = "/tmp/ogde_test_dir";
    
    // Create directory
    bool createSuccess = FileSystem::CreateDirectory(testDir);
    assert(createSuccess && "Failed to create directory");
    
    // Check existence
    assert(FileSystem::DirectoryExists(testDir) && "Directory should exist");
    
    // Create subdirectory
    const std::string subDir = FileSystem::JoinPath(testDir, "subdir");
    assert(FileSystem::CreateDirectory(subDir) && "Failed to create subdirectory");
    assert(FileSystem::DirectoryExists(subDir) && "Subdirectory should exist");
    
    std::cout << "  ✓ Directory operations passed" << std::endl;
}

void TestPathManipulation() {
    std::cout << "Testing path manipulation..." << std::endl;
    
    const std::string testPath = "/home/user/documents/file.txt";
    
    // Get directory
    std::string dir = FileSystem::GetDirectory(testPath);
    assert(dir == "/home/user/documents" && "GetDirectory failed");
    
    // Get filename
    std::string filename = FileSystem::GetFilename(testPath);
    assert(filename == "file.txt" && "GetFilename failed");
    
    // Get extension
    std::string ext = FileSystem::GetExtension(testPath);
    assert(ext == ".txt" && "GetExtension failed");
    
    // Join path
    std::string joined = FileSystem::JoinPath("/home/user", "file.txt");
    assert(!joined.empty() && "JoinPath failed");
    
    std::cout << "  ✓ Path manipulation passed" << std::endl;
}

void TestAppendMode() {
    std::cout << "Testing append mode..." << std::endl;
    
    const std::string testFile = "/tmp/test_append.txt";
    
    // Write initial content
    FileSystem::WriteTextFile(testFile, "Line 1\n", false);
    
    // Append more content
    FileSystem::WriteTextFile(testFile, "Line 2\n", true);
    
    // Read and verify
    auto content = FileSystem::ReadTextFile(testFile);
    assert(content.has_value() && "Failed to read appended file");
    assert(content.value() == "Line 1\nLine 2\n" && "Append mode failed");
    
    std::cout << "  ✓ Append mode passed" << std::endl;
}

void TestConfigBasics() {
    std::cout << "Testing config basic operations..." << std::endl;
    
    Config config;
    
    // Set values
    config.SetString("app.name", "OpenGameDevEngine");
    config.SetInt("graphics.width", 1920);
    config.SetInt("graphics.height", 1080);
    config.SetFloat("audio.volume", 0.75f);
    config.SetBool("debug.enabled", true);
    
    // Get values
    assert(config.GetString("app.name") == "OpenGameDevEngine" && "String value mismatch");
    assert(config.GetInt("graphics.width") == 1920 && "Int value mismatch");
    assert(config.GetInt("graphics.height") == 1080 && "Int value mismatch");
    assert(std::abs(config.GetFloat("audio.volume") - 0.75f) < 0.001f && "Float value mismatch");
    assert(config.GetBool("debug.enabled") == true && "Bool value mismatch");
    
    // Test defaults
    assert(config.GetString("nonexistent", "default") == "default" && "Default string failed");
    assert(config.GetInt("nonexistent", 42) == 42 && "Default int failed");
    
    std::cout << "  ✓ Config basic operations passed" << std::endl;
}

void TestConfigFileIO() {
    std::cout << "Testing config file I/O..." << std::endl;
    
    const std::string configFile = "/tmp/test_config.json";
    
    // Create and save config
    Config config1;
    config1.SetString("engine.version", "0.1.0");
    config1.SetInt("window.width", 800);
    config1.SetBool("vsync", true);
    
    assert(config1.SaveToFile(configFile) && "Failed to save config");
    
    // Load into new config
    Config config2;
    assert(config2.LoadFromFile(configFile) && "Failed to load config");
    
    // Verify values
    assert(config2.GetString("engine.version") == "0.1.0" && "Loaded string mismatch");
    assert(config2.GetInt("window.width") == 800 && "Loaded int mismatch");
    assert(config2.GetBool("vsync") == true && "Loaded bool mismatch");
    
    std::cout << "  ✓ Config file I/O passed" << std::endl;
}

void TestConfigStringParsing() {
    std::cout << "Testing config string parsing..." << std::endl;
    
    const std::string jsonStr = R"({
        "app": {
            "name": "TestApp",
            "version": "1.0"
        },
        "settings": {
            "enabled": true,
            "count": 10
        }
    })";
    
    Config config;
    assert(config.LoadFromString(jsonStr) && "Failed to parse JSON string");
    
    assert(config.GetString("app.name") == "TestApp" && "Parsed string mismatch");
    assert(config.GetString("app.version") == "1.0" && "Parsed version mismatch");
    assert(config.GetBool("settings.enabled") == true && "Parsed bool mismatch");
    assert(config.GetInt("settings.count") == 10 && "Parsed int mismatch");
    
    std::cout << "  ✓ Config string parsing passed" << std::endl;
}

void TestConfigKeyOperations() {
    std::cout << "Testing config key operations..." << std::endl;
    
    Config config;
    config.SetString("test.key", "value");
    
    // Test HasKey
    assert(config.HasKey("test.key") && "HasKey failed for existing key");
    assert(!config.HasKey("nonexistent") && "HasKey failed for non-existent key");
    
    // Test RemoveKey
    assert(config.RemoveKey("test.key") && "RemoveKey failed");
    assert(!config.HasKey("test.key") && "Key still exists after removal");
    assert(!config.RemoveKey("nonexistent") && "RemoveKey should return false for non-existent key");
    
    // Test Clear
    config.SetString("a", "1");
    config.SetString("b", "2");
    config.Clear();
    assert(!config.HasKey("a") && "Clear failed");
    assert(!config.HasKey("b") && "Clear failed");
    
    std::cout << "  ✓ Config key operations passed" << std::endl;
}

int main() {
    std::cout << "=== Core Tests ===" << std::endl;
    
    try {
        // FileSystem tests
        TestTextFileReadWrite();
        TestBinaryFileReadWrite();
        TestFileExists();
        TestDirectoryOperations();
        TestPathManipulation();
        TestAppendMode();
        
        // Config tests
        TestConfigBasics();
        TestConfigFileIO();
        TestConfigStringParsing();
        TestConfigKeyOperations();
        
        std::cout << "\n✓ All core tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
