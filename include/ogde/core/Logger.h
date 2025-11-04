/**
 * @file Logger.h
 * @brief Logging system for the engine
 */

#ifndef OGDE_CORE_LOGGER_H
#define OGDE_CORE_LOGGER_H

#include <string>

namespace ogde {
namespace core {

/**
 * @enum LogLevel
 * @brief Log message severity levels
 */
enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Critical
};

/**
 * @class Logger
 * @brief Simple logging system for engine messages
 */
class Logger {
public:
    /**
     * @brief Initialize file logging
     * @param filename Path to log file
     * @return true if file logging was enabled successfully
     */
    static bool initializeFileLogging(const std::string& filename);

    /**
     * @brief Shutdown file logging
     */
    static void shutdownFileLogging();

    /**
     * @brief Log a message with specified severity level
     * @param level The severity level
     * @param message The message to log
     */
    static void log(LogLevel level, const std::string& message);

    /**
     * @brief Log a debug message
     * @param message The message to log
     */
    static void debug(const std::string& message);

    /**
     * @brief Log an info message
     * @param message The message to log
     */
    static void info(const std::string& message);

    /**
     * @brief Log a warning message
     * @param message The message to log
     */
    static void warning(const std::string& message);

    /**
     * @brief Log an error message
     * @param message The message to log
     */
    static void error(const std::string& message);

    /**
     * @brief Log a critical message
     * @param message The message to log
     */
    static void critical(const std::string& message);
};

} // namespace core
} // namespace ogde

#endif // OGDE_CORE_LOGGER_H
