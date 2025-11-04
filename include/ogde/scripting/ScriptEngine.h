/**
 * @file ScriptEngine.h
 * @brief Scripting system for game logic
 */

#ifndef OGDE_SCRIPTING_SCRIPTENGINE_H
#define OGDE_SCRIPTING_SCRIPTENGINE_H

#include <string>

namespace ogde {
namespace scripting {

/**
 * @class ScriptEngine
 * @brief Scripting system for executing game scripts
 */
class ScriptEngine {
public:
    /**
     * @brief Initialize the script engine
     * @return true if initialization was successful
     */
    bool initialize();

    /**
     * @brief Shutdown the script engine
     */
    void shutdown();

    /**
     * @brief Load and execute a script file
     * @param filename Path to the script file
     * @return true if script executed successfully
     */
    bool executeScript(const std::string& filename);

    /**
     * @brief Execute a script string
     * @param script The script code to execute
     * @return true if script executed successfully
     */
    bool executeString(const std::string& script);
};

} // namespace scripting
} // namespace ogde

#endif // OGDE_SCRIPTING_SCRIPTENGINE_H
