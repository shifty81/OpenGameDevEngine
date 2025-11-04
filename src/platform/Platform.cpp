/**
 * Platform Implementation
 */

#include "ogde/platform/Platform.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <chrono>
#include <thread>
#endif

namespace ogde {
namespace platform {

bool Platform::s_initialized = false;
double Platform::s_performanceFrequency = 0.0;

bool Platform::initialize() {
    if (s_initialized) {
        return true;
    }

#ifdef _WIN32
    // Initialize high-resolution timer
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    s_performanceFrequency = static_cast<double>(frequency.QuadPart);
#else
    // On non-Windows, we use chrono which doesn't need initialization
    s_performanceFrequency = 1.0;
#endif

    s_initialized = true;
    return true;
}

void Platform::shutdown() {
    s_initialized = false;
}

double Platform::getTime() {
#ifdef _WIN32
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return static_cast<double>(counter.QuadPart) / s_performanceFrequency;
#else
    using namespace std::chrono;
    auto now = high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    return duration_cast<std::chrono::duration<double>>(duration).count();
#endif
}

void Platform::sleep(uint32_t milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
#endif
}

bool Platform::supportsDirectX() {
#ifdef _WIN32
    return true;
#else
    return false;
#endif
}

std::string Platform::getPlatformName() {
#ifdef _WIN32
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "Unknown";
#endif
}

} // namespace platform
} // namespace ogde
