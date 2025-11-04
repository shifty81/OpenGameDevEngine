/**
 * Logger Implementation
 */

#include "ogde/core/Logger.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace ogde {
namespace core {

static std::ofstream g_logFile;
static bool g_fileLoggingEnabled = false;

static std::string getCurrentTimestamp() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

static std::string getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Critical: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = getLevelString(level);
    std::string fullMessage = "[" + timestamp + "] [" + levelStr + "] " + message;

    // Console output
    if (level == LogLevel::Error || level == LogLevel::Critical) {
        std::cerr << fullMessage << std::endl;
    } else {
        std::cout << fullMessage << std::endl;
    }

    // File output
    if (g_fileLoggingEnabled && g_logFile.is_open()) {
        g_logFile << fullMessage << std::endl;
        g_logFile.flush();
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::Debug, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::Info, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::Warning, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::Error, message);
}

} // namespace core
} // namespace ogde
