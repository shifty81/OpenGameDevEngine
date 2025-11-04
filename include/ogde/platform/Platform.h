/**
 * @file Platform.h
 * @brief Platform-specific functionality and abstractions
 */

#ifndef OGDE_PLATFORM_PLATFORM_H
#define OGDE_PLATFORM_PLATFORM_H

#include <cstdint>
#include <string>

// Platform detection
#ifdef _WIN32
    #define OGDE_PLATFORM_WINDOWS
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #include <Windows.h>
#endif

namespace ogde {
namespace platform {

/**
 * @brief Platform initialization and utility functions
 */
class Platform {
public:
    /**
     * @brief Initialize platform-specific systems
     * @return true if initialization was successful
     */
    static bool initialize();

    /**
     * @brief Shutdown platform-specific systems
     */
    static void shutdown();

    /**
     * @brief Get the current time in seconds (high precision)
     * @return Current time in seconds
     */
    static double getTime();

    /**
     * @brief Sleep for a specified number of milliseconds
     * @param milliseconds Time to sleep
     */
    static void sleep(uint32_t milliseconds);

    /**
     * @brief Check if the platform supports DirectX
     * @return true if DirectX is supported
     */
    static bool supportsDirectX();

    /**
     * @brief Get platform name
     * @return Platform name string
     */
    static std::string getPlatformName();

private:
    static bool s_initialized;
    static double s_performanceFrequency;
};

} // namespace platform
} // namespace ogde

#endif // OGDE_PLATFORM_PLATFORM_H
