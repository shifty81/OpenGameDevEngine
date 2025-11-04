/**
 * @file OpenGameDevEngine.h
 * @brief Main header file for the OpenGameDevEngine
 * 
 * Include this file to access all engine functionality.
 */

#ifndef OGDE_OPENGAMEDEVENGINE_H
#define OGDE_OPENGAMEDEVENGINE_H

// Core module
#include "ogde/core/Engine.h"
#include "ogde/core/Logger.h"
#include "ogde/core/Math.h"

// Graphics module
#include "ogde/graphics/Renderer.h"
#include "ogde/graphics/Window.h"

// Audio module
#include "ogde/audio/AudioEngine.h"

// Input module
#include "ogde/input/InputManager.h"

// Physics module
#include "ogde/physics/PhysicsEngine.h"

// Scripting module
#include "ogde/scripting/ScriptEngine.h"

// Networking module
#include "ogde/networking/NetworkManager.h"

namespace ogde {

/**
 * @brief Version information for the engine
 */
constexpr int VERSION_MAJOR = 0;
constexpr int VERSION_MINOR = 1;
constexpr int VERSION_PATCH = 0;

} // namespace ogde

#endif // OGDE_OPENGAMEDEVENGINE_H
