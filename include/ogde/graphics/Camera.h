/**
 * @file Camera.h
 * @brief Camera system for 3D rendering
 */

#ifndef OGDE_GRAPHICS_CAMERA_H
#define OGDE_GRAPHICS_CAMERA_H

#ifdef _WIN32
#include <DirectXMath.h>
#endif

namespace ogde {
namespace graphics {

/**
 * @enum ProjectionType
 * @brief Camera projection types
 */
enum class ProjectionType {
    Perspective,    ///< Perspective projection (3D)
    Orthographic    ///< Orthographic projection (2D/UI)
};

/**
 * @class Camera
 * @brief Camera for 3D rendering with view and projection matrices
 */
class Camera {
public:
    Camera();
    ~Camera();

    /**
     * @brief Set up a perspective projection
     * @param fovDegrees Field of view in degrees
     * @param aspectRatio Width / height ratio
     * @param nearPlane Near clipping plane distance
     * @param farPlane Far clipping plane distance
     */
    void setPerspective(float fovDegrees, float aspectRatio, float nearPlane = 0.1f, float farPlane = 1000.0f);

    /**
     * @brief Set up an orthographic projection
     * @param width Viewport width
     * @param height Viewport height
     * @param nearPlane Near clipping plane distance
     * @param farPlane Far clipping plane distance
     */
    void setOrthographic(float width, float height, float nearPlane = -1.0f, float farPlane = 1.0f);

    /**
     * @brief Set camera position
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     */
    void setPosition(float x, float y, float z);

    /**
     * @brief Set camera rotation
     * @param pitch Rotation around X axis (degrees)
     * @param yaw Rotation around Y axis (degrees)
     * @param roll Rotation around Z axis (degrees)
     */
    void setRotation(float pitch, float yaw, float roll);

    /**
     * @brief Set camera to look at a target
     * @param eyeX Camera position X
     * @param eyeY Camera position Y
     * @param eyeZ Camera position Z
     * @param targetX Target position X
     * @param targetY Target position Y
     * @param targetZ Target position Z
     * @param upX Up vector X
     * @param upY Up vector Y
     * @param upZ Up vector Z
     */
    void lookAt(float eyeX, float eyeY, float eyeZ,
                float targetX, float targetY, float targetZ,
                float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);

    /**
     * @brief Get the view matrix
     * @return Pointer to view matrix data (16 floats, column-major)
     */
    const float* getViewMatrix() const;

    /**
     * @brief Get the projection matrix
     * @return Pointer to projection matrix data (16 floats, column-major)
     */
    const float* getProjectionMatrix() const;

    /**
     * @brief Get the view-projection matrix (projection * view)
     * @return Pointer to combined matrix data (16 floats, column-major)
     */
    const float* getViewProjectionMatrix() const;

    /**
     * @brief Get camera position
     * @param outX Output X coordinate
     * @param outY Output Y coordinate
     * @param outZ Output Z coordinate
     */
    void getPosition(float& outX, float& outY, float& outZ) const;

    /**
     * @brief Get camera forward direction
     * @param outX Output X component
     * @param outY Output Y component
     * @param outZ Output Z component
     */
    void getForward(float& outX, float& outY, float& outZ) const;

    /**
     * @brief Get camera right direction
     * @param outX Output X component
     * @param outY Output Y component
     * @param outZ Output Z component
     */
    void getRight(float& outX, float& outY, float& outZ) const;

    /**
     * @brief Get camera up direction
     * @param outX Output X component
     * @param outY Output Y component
     * @param outZ Output Z component
     */
    void getUp(float& outX, float& outY, float& outZ) const;

    /**
     * @brief Update camera matrices (call after changing position/rotation)
     */
    void update();

    /**
     * @brief Get the projection type
     * @return Current projection type
     */
    ProjectionType getProjectionType() const { return m_projectionType; }

private:
    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateViewProjectionMatrix();

    ProjectionType m_projectionType;
    
    // Position and orientation
    float m_position[3];        // x, y, z
    float m_rotation[3];        // pitch, yaw, roll (degrees)
    float m_forward[3];         // forward vector
    float m_right[3];           // right vector
    float m_up[3];              // up vector

    // Projection parameters
    float m_fov;                // Field of view (degrees)
    float m_aspectRatio;        // Width / height
    float m_nearPlane;          // Near clipping plane
    float m_farPlane;           // Far clipping plane
    float m_orthoWidth;         // Orthographic width
    float m_orthoHeight;        // Orthographic height

    // Matrices (stored as 16 floats, column-major for DirectX)
    float m_viewMatrix[16];
    float m_projectionMatrix[16];
    float m_viewProjectionMatrix[16];

    bool m_viewDirty;           // View matrix needs update
    bool m_projectionDirty;     // Projection matrix needs update
};

} // namespace graphics
} // namespace ogde

#endif // OGDE_GRAPHICS_CAMERA_H
