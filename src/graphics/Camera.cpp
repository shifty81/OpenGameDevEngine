/**
 * @file Camera.cpp
 * @brief Camera system implementation
 */

#include "ogde/graphics/Camera.h"
#include <cmath>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace ogde {
namespace graphics {

// Helper function to convert degrees to radians
static inline float degreesToRadians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

// Helper function for matrix multiplication (C = A * B)
static void matrixMultiply(const float* a, const float* b, float* c) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            c[i * 4 + j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                c[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
            }
        }
    }
}

// Helper function to create identity matrix
static void matrixIdentity(float* m) {
    memset(m, 0, sizeof(float) * 16);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

Camera::Camera()
    : m_projectionType(ProjectionType::Perspective)
    , m_fov(45.0f)
    , m_aspectRatio(16.0f / 9.0f)
    , m_nearPlane(0.1f)
    , m_farPlane(1000.0f)
    , m_orthoWidth(800.0f)
    , m_orthoHeight(600.0f)
    , m_viewDirty(true)
    , m_projectionDirty(true)
{
    // Initialize position at origin
    m_position[0] = 0.0f;
    m_position[1] = 0.0f;
    m_position[2] = 0.0f;

    // Initialize rotation
    m_rotation[0] = 0.0f;  // pitch
    m_rotation[1] = 0.0f;  // yaw
    m_rotation[2] = 0.0f;  // roll

    // Initialize direction vectors (DirectX left-handed coordinate system)
    m_forward[0] = 0.0f;
    m_forward[1] = 0.0f;
    m_forward[2] = 1.0f;  // Forward is positive Z in DirectX left-handed system

    m_right[0] = 1.0f;
    m_right[1] = 0.0f;
    m_right[2] = 0.0f;

    m_up[0] = 0.0f;
    m_up[1] = 1.0f;
    m_up[2] = 0.0f;

    // Initialize matrices to identity
    matrixIdentity(m_viewMatrix);
    matrixIdentity(m_projectionMatrix);
    matrixIdentity(m_viewProjectionMatrix);

    // Set up default perspective projection
    updateProjectionMatrix();
}

Camera::~Camera() {
}

void Camera::setPerspective(float fovDegrees, float aspectRatio, float nearPlane, float farPlane) {
    m_projectionType = ProjectionType::Perspective;
    m_fov = fovDegrees;
    m_aspectRatio = aspectRatio;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_projectionDirty = true;
}

void Camera::setOrthographic(float width, float height, float nearPlane, float farPlane) {
    m_projectionType = ProjectionType::Orthographic;
    m_orthoWidth = width;
    m_orthoHeight = height;
    m_nearPlane = nearPlane;
    m_farPlane = farPlane;
    m_projectionDirty = true;
}

void Camera::setPosition(float x, float y, float z) {
    m_position[0] = x;
    m_position[1] = y;
    m_position[2] = z;
    m_viewDirty = true;
}

void Camera::setRotation(float pitch, float yaw, float roll) {
    m_rotation[0] = pitch;
    m_rotation[1] = yaw;
    m_rotation[2] = roll;

    // Update direction vectors based on rotation
    float pitchRad = degreesToRadians(pitch);
    float yawRad = degreesToRadians(yaw);
    float rollRad = degreesToRadians(roll);

    // Calculate forward vector
    m_forward[0] = std::cos(pitchRad) * std::sin(yawRad);
    m_forward[1] = std::sin(pitchRad);
    m_forward[2] = std::cos(pitchRad) * std::cos(yawRad);

    // Normalize forward vector
    float forwardLength = std::sqrt(m_forward[0] * m_forward[0] + 
                                   m_forward[1] * m_forward[1] + 
                                   m_forward[2] * m_forward[2]);
    if (forwardLength > 0.0001f) {
        m_forward[0] /= forwardLength;
        m_forward[1] /= forwardLength;
        m_forward[2] /= forwardLength;
    }

    // Calculate right vector (cross product of world up and forward)
    float worldUp[3] = { 0.0f, 1.0f, 0.0f };
    m_right[0] = worldUp[1] * m_forward[2] - worldUp[2] * m_forward[1];
    m_right[1] = worldUp[2] * m_forward[0] - worldUp[0] * m_forward[2];
    m_right[2] = worldUp[0] * m_forward[1] - worldUp[1] * m_forward[0];

    // Normalize right vector
    float rightLength = std::sqrt(m_right[0] * m_right[0] + 
                                 m_right[1] * m_right[1] + 
                                 m_right[2] * m_right[2]);
    if (rightLength > 0.0001f) {
        m_right[0] /= rightLength;
        m_right[1] /= rightLength;
        m_right[2] /= rightLength;
    }

    // Calculate up vector (cross product of forward and right)
    m_up[0] = m_forward[1] * m_right[2] - m_forward[2] * m_right[1];
    m_up[1] = m_forward[2] * m_right[0] - m_forward[0] * m_right[2];
    m_up[2] = m_forward[0] * m_right[1] - m_forward[1] * m_right[0];

    // Normalize up vector
    float upLength = std::sqrt(m_up[0] * m_up[0] + m_up[1] * m_up[1] + m_up[2] * m_up[2]);
    if (upLength > 0.0001f) {
        m_up[0] /= upLength;
        m_up[1] /= upLength;
        m_up[2] /= upLength;
    }

    m_viewDirty = true;
}

void Camera::lookAt(float eyeX, float eyeY, float eyeZ,
                    float targetX, float targetY, float targetZ,
                    float upX, float upY, float upZ) {
    // Set position
    m_position[0] = eyeX;
    m_position[1] = eyeY;
    m_position[2] = eyeZ;

    // Calculate forward vector (from eye to target)
    m_forward[0] = targetX - eyeX;
    m_forward[1] = targetY - eyeY;
    m_forward[2] = targetZ - eyeZ;

    // Normalize forward
    float forwardLength = std::sqrt(m_forward[0] * m_forward[0] + 
                                   m_forward[1] * m_forward[1] + 
                                   m_forward[2] * m_forward[2]);
    if (forwardLength > 0.0001f) {
        m_forward[0] /= forwardLength;
        m_forward[1] /= forwardLength;
        m_forward[2] /= forwardLength;
    }

    // Calculate right vector (cross product of up and forward)
    m_right[0] = upY * m_forward[2] - upZ * m_forward[1];
    m_right[1] = upZ * m_forward[0] - upX * m_forward[2];
    m_right[2] = upX * m_forward[1] - upY * m_forward[0];

    // Normalize right
    float rightLength = std::sqrt(m_right[0] * m_right[0] + 
                                 m_right[1] * m_right[1] + 
                                 m_right[2] * m_right[2]);
    if (rightLength > 0.0001f) {
        m_right[0] /= rightLength;
        m_right[1] /= rightLength;
        m_right[2] /= rightLength;
    }

    // Recalculate up vector (cross product of forward and right)
    m_up[0] = m_forward[1] * m_right[2] - m_forward[2] * m_right[1];
    m_up[1] = m_forward[2] * m_right[0] - m_forward[0] * m_right[2];
    m_up[2] = m_forward[0] * m_right[1] - m_forward[1] * m_right[0];

    // Normalize up
    float upLength = std::sqrt(m_up[0] * m_up[0] + m_up[1] * m_up[1] + m_up[2] * m_up[2]);
    if (upLength > 0.0001f) {
        m_up[0] /= upLength;
        m_up[1] /= upLength;
        m_up[2] /= upLength;
    }

    m_viewDirty = true;
}

const float* Camera::getViewMatrix() const {
    return m_viewMatrix;
}

const float* Camera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

const float* Camera::getViewProjectionMatrix() const {
    return m_viewProjectionMatrix;
}

void Camera::getPosition(float& outX, float& outY, float& outZ) const {
    outX = m_position[0];
    outY = m_position[1];
    outZ = m_position[2];
}

void Camera::getForward(float& outX, float& outY, float& outZ) const {
    outX = m_forward[0];
    outY = m_forward[1];
    outZ = m_forward[2];
}

void Camera::getRight(float& outX, float& outY, float& outZ) const {
    outX = m_right[0];
    outY = m_right[1];
    outZ = m_right[2];
}

void Camera::getUp(float& outX, float& outY, float& outZ) const {
    outX = m_up[0];
    outY = m_up[1];
    outZ = m_up[2];
}

void Camera::update() {
    if (m_viewDirty) {
        updateViewMatrix();
        m_viewDirty = false;
    }

    if (m_projectionDirty) {
        updateProjectionMatrix();
        m_projectionDirty = false;
    }

    updateViewProjectionMatrix();
}

void Camera::updateViewMatrix() {
    // Create view matrix using look-at approach
    // View matrix is the inverse of camera's world transform
    // For an orthonormal rotation matrix, the inverse is the transpose
    // The translation is negated and transformed by the transposed rotation
    
    // Column-major format for DirectX (transposed rotation)
    m_viewMatrix[0] = m_right[0];
    m_viewMatrix[1] = m_up[0];
    m_viewMatrix[2] = m_forward[0];
    m_viewMatrix[3] = 0.0f;

    m_viewMatrix[4] = m_right[1];
    m_viewMatrix[5] = m_up[1];
    m_viewMatrix[6] = m_forward[1];
    m_viewMatrix[7] = 0.0f;

    m_viewMatrix[8] = m_right[2];
    m_viewMatrix[9] = m_up[2];
    m_viewMatrix[10] = m_forward[2];
    m_viewMatrix[11] = 0.0f;

    // Translation (dot product with camera position, negated)
    m_viewMatrix[12] = -(m_right[0] * m_position[0] + m_right[1] * m_position[1] + m_right[2] * m_position[2]);
    m_viewMatrix[13] = -(m_up[0] * m_position[0] + m_up[1] * m_position[1] + m_up[2] * m_position[2]);
    m_viewMatrix[14] = -(m_forward[0] * m_position[0] + m_forward[1] * m_position[1] + m_forward[2] * m_position[2]);
    m_viewMatrix[15] = 1.0f;
}

void Camera::updateProjectionMatrix() {
    if (m_projectionType == ProjectionType::Perspective) {
        // Perspective projection matrix (column-major, left-handed for DirectX)
        float fovRad = degreesToRadians(m_fov);
        float tanHalfFov = std::tan(fovRad / 2.0f);

        memset(m_projectionMatrix, 0, sizeof(float) * 16);

        m_projectionMatrix[0] = 1.0f / (m_aspectRatio * tanHalfFov);
        m_projectionMatrix[5] = 1.0f / tanHalfFov;
        m_projectionMatrix[10] = m_farPlane / (m_farPlane - m_nearPlane);
        m_projectionMatrix[11] = 1.0f;
        m_projectionMatrix[14] = -(m_farPlane * m_nearPlane) / (m_farPlane - m_nearPlane);
    } else {
        // Orthographic projection matrix (column-major)
        memset(m_projectionMatrix, 0, sizeof(float) * 16);

        m_projectionMatrix[0] = 2.0f / m_orthoWidth;
        m_projectionMatrix[5] = 2.0f / m_orthoHeight;
        m_projectionMatrix[10] = 1.0f / (m_farPlane - m_nearPlane);
        m_projectionMatrix[14] = -m_nearPlane / (m_farPlane - m_nearPlane);
        m_projectionMatrix[15] = 1.0f;
    }
}

void Camera::updateViewProjectionMatrix() {
    // Multiply projection * view
    matrixMultiply(m_projectionMatrix, m_viewMatrix, m_viewProjectionMatrix);
}

} // namespace graphics
} // namespace ogde
