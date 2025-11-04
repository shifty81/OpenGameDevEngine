/**
 * @file PhysicsEngine.h
 * @brief Physics simulation and collision detection
 */

#ifndef OGDE_PHYSICS_PHYSICSENGINE_H
#define OGDE_PHYSICS_PHYSICSENGINE_H

namespace ogde {
namespace physics {

/**
 * @class PhysicsEngine
 * @brief Physics simulation system
 */
class PhysicsEngine {
public:
    /**
     * @brief Initialize the physics engine
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Shutdown the physics engine
     */
    void shutdown();

    /**
     * @brief Update physics simulation
     * @param deltaTime Time step for simulation
     */
    void update(float deltaTime);

    /**
     * @brief Set gravity
     * @param x Gravity X component
     * @param y Gravity Y component
     * @param z Gravity Z component
     */
    void setGravity(float x, float y, float z);
};

} // namespace physics
} // namespace ogde

#endif // OGDE_PHYSICS_PHYSICSENGINE_H
