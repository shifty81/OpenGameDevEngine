/**
 * Core module tests
 * Tests for FileSystem and other core components
 */

#include "ogde/core/FileSystem.h"
#include <iostream>
#include <cassert>

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

int main() {
    std::cout << "=== Core Tests ===" << std::endl;
    
    try {
        TestTextFileReadWrite();
        TestBinaryFileReadWrite();
        TestFileExists();
        TestDirectoryOperations();
        TestPathManipulation();
        TestAppendMode();
        
        std::cout << "\n✓ All core tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
