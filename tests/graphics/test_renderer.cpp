/**
 * Graphics module tests
 * Tests for graphics components including camera
 */

#include "ogde/graphics/Camera.h"
#include <iostream>
#include <cmath>
#include <cstring>

// Simple test framework
int testsPassed = 0;
int testsFailed = 0;

#define TEST(name) \
    std::cout << "Testing: " << name << "... "; \
    do {} while(false); if (true)

#define EXPECT_TRUE(condition) \
    if (condition) { \
        testsPassed++; \
        std::cout << "PASS" << std::endl; \
    } else { \
        testsFailed++; \
        std::cout << "FAIL: " #condition << std::endl; \
    }

#define EXPECT_NEAR(a, b, epsilon) \
    if (std::abs((a) - (b)) < (epsilon)) { \
        testsPassed++; \
        std::cout << "PASS" << std::endl; \
    } else { \
        testsFailed++; \
        std::cout << "FAIL: " #a " (" << (a) << ") != " #b " (" << (b) << ")" << std::endl; \
    }

void testCameraInitialization() {
    TEST("Camera initialization") {
        ogde::graphics::Camera camera;
        
        float x, y, z;
        camera.getPosition(x, y, z);
        
        bool positionCorrect = (x == 0.0f && y == 0.0f && z == 0.0f);
        bool projectionType = (camera.getProjectionType() == ogde::graphics::ProjectionType::Perspective);
        
        EXPECT_TRUE(positionCorrect && projectionType);
    }
}

void testCameraPosition() {
    TEST("Camera position setting") {
        ogde::graphics::Camera camera;
        camera.setPosition(1.0f, 2.0f, 3.0f);
        camera.update();
        
        float x, y, z;
        camera.getPosition(x, y, z);
        
        bool correct = (std::abs(x - 1.0f) < 0.001f && 
                       std::abs(y - 2.0f) < 0.001f && 
                       std::abs(z - 3.0f) < 0.001f);
        
        EXPECT_TRUE(correct);
    }
}

void testCameraPerspective() {
    TEST("Camera perspective projection") {
        ogde::graphics::Camera camera;
        camera.setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        camera.update();
        
        const float* projMatrix = camera.getProjectionMatrix();
        
        // Just verify the matrix is not identity (projection was set)
        bool notIdentity = (projMatrix[0] != 1.0f || projMatrix[5] != 1.0f);
        
        EXPECT_TRUE(notIdentity);
    }
}

void testCameraOrthographic() {
    TEST("Camera orthographic projection") {
        ogde::graphics::Camera camera;
        camera.setOrthographic(800.0f, 600.0f, -1.0f, 1.0f);
        camera.update();
        
        const float* projMatrix = camera.getProjectionMatrix();
        
        // Verify projection type changed
        bool typeCorrect = (camera.getProjectionType() == ogde::graphics::ProjectionType::Orthographic);
        bool notIdentity = (projMatrix[0] != 1.0f || projMatrix[5] != 1.0f);
        
        EXPECT_TRUE(typeCorrect && notIdentity);
    }
}

void testCameraLookAt() {
    TEST("Camera lookAt function") {
        ogde::graphics::Camera camera;
        camera.lookAt(0.0f, 0.0f, -5.0f,  // Eye
                     0.0f, 0.0f, 0.0f,    // Target
                     0.0f, 1.0f, 0.0f);   // Up
        camera.update();
        
        float x, y, z;
        camera.getPosition(x, y, z);
        
        bool positionCorrect = (std::abs(z + 5.0f) < 0.001f);
        
        EXPECT_TRUE(positionCorrect);
    }
}

void testCameraViewMatrix() {
    TEST("Camera view matrix generation") {
        ogde::graphics::Camera camera;
        camera.setPosition(0.0f, 0.0f, -5.0f);
        camera.update();
        
        const float* viewMatrix = camera.getViewMatrix();
        
        // Verify matrix is not null and not all zeros
        bool valid = (viewMatrix != nullptr);
        if (valid) {
            float sum = 0.0f;
            for (int i = 0; i < 16; ++i) {
                sum += std::abs(viewMatrix[i]);
            }
            valid = (sum > 0.001f);
        }
        
        EXPECT_TRUE(valid);
    }
}

void testCameraViewProjectionMatrix() {
    TEST("Camera view-projection matrix") {
        ogde::graphics::Camera camera;
        camera.setPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
        camera.setPosition(0.0f, 0.0f, -5.0f);
        camera.update();
        
        const float* vpMatrix = camera.getViewProjectionMatrix();
        
        // Verify matrix is not null and not identity
        bool valid = (vpMatrix != nullptr);
        if (valid) {
            bool notIdentity = (vpMatrix[0] != 1.0f || vpMatrix[5] != 1.0f || 
                               vpMatrix[10] != 1.0f || vpMatrix[15] != 1.0f);
            valid = notIdentity;
        }
        
        EXPECT_TRUE(valid);
    }
}

void testCameraDirectionVectors() {
    TEST("Camera direction vectors") {
        ogde::graphics::Camera camera;
        camera.setRotation(0.0f, 0.0f, 0.0f);
        camera.update();
        
        float fx, fy, fz, rx, ry, rz, ux, uy, uz;
        camera.getForward(fx, fy, fz);
        camera.getRight(rx, ry, rz);
        camera.getUp(ux, uy, uz);
        
        // Check that vectors are normalized (length ~ 1.0)
        float forwardLen = std::sqrt(fx*fx + fy*fy + fz*fz);
        float rightLen = std::sqrt(rx*rx + ry*ry + rz*rz);
        float upLen = std::sqrt(ux*ux + uy*uy + uz*uz);
        
        bool normalized = (std::abs(forwardLen - 1.0f) < 0.01f &&
                          std::abs(rightLen - 1.0f) < 0.01f &&
                          std::abs(upLen - 1.0f) < 0.01f);
        
        EXPECT_TRUE(normalized);
    }
}

int main() {
    std::cout << "=== Graphics Module Tests ===" << std::endl;
    std::cout << std::endl;
    
    std::cout << "--- Camera Tests ---" << std::endl;
    testCameraInitialization();
    testCameraPosition();
    testCameraPerspective();
    testCameraOrthographic();
    testCameraLookAt();
    testCameraViewMatrix();
    testCameraViewProjectionMatrix();
    testCameraDirectionVectors();
    
    std::cout << std::endl;
    std::cout << "=== Test Results ===" << std::endl;
    std::cout << "Passed: " << testsPassed << std::endl;
    std::cout << "Failed: " << testsFailed << std::endl;
    
    return (testsFailed == 0) ? 0 : 1;
}

