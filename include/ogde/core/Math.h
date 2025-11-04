/**
 * @file Math.h
 * @brief Mathematical utilities and types
 */

#ifndef OGDE_CORE_MATH_H
#define OGDE_CORE_MATH_H

namespace ogde {
namespace core {

/**
 * @struct Vector2
 * @brief 2D vector
 */
struct Vector2 {
    float x, y;
    
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

/**
 * @struct Vector3
 * @brief 3D vector
 */
struct Vector3 {
    float x, y, z;
    
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

/**
 * @struct Vector4
 * @brief 4D vector
 */
struct Vector4 {
    float x, y, z, w;
    
    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

/**
 * @struct Matrix4
 * @brief 4x4 matrix for transformations
 */
struct Matrix4 {
    float m[16];
    
    Matrix4();
    static Matrix4 identity();
};

} // namespace core
} // namespace ogde

#endif // OGDE_CORE_MATH_H
