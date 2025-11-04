/**
 * Script Engine Implementation
 */

#include "ogde/scripting/ScriptEngine.h"

namespace ogde {
namespace scripting {

bool ScriptEngine::initialize() {
    return true;
}

void ScriptEngine::shutdown() {
}

bool ScriptEngine::executeScript(const std::string& filename) {
    return true;
}

bool ScriptEngine::executeString(const std::string& script) {
    return true;
}

} // namespace scripting
} // namespace ogde
