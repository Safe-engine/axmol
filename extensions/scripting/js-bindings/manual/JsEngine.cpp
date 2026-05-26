/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsEngine.h"

#include "axmol/base/Logging.h"

#include <string>

namespace ax
{

JsEngine* JsEngine::_defaultEngine = nullptr;

JsEngine* JsEngine::getInstance()
{
    if (!_defaultEngine)
    {
        _defaultEngine = new JsEngine();
        _defaultEngine->init();
    }
    return _defaultEngine;
}

JsEngine::~JsEngine()
{
    if (_runtime)
    {
        _runtime->destroy();
        _runtime = nullptr;
    }
    _defaultEngine = nullptr;
}

bool JsEngine::init()
{
    _runtime = JsRuntime::create();
    return _runtime != nullptr;
}

int JsEngine::executeString(const char* codes)
{
    if (!_runtime || !codes)
        return -1;
    return _runtime->eval(codes) ? 0 : -1;
}

int JsEngine::executeScriptFile(const char* filename)
{
    if (!_runtime || !filename)
        return -1;
    return _runtime->evalFile(filename) ? 0 : -1;
}

int JsEngine::executeGlobalFunction(const char* functionName)
{
    if (!_runtime || !functionName)
        return -1;

    std::string code = std::string(functionName) + "();";
    return executeString(code.c_str());
}

int JsEngine::sendEvent(const ScriptEvent& /*evt*/)
{
    return 0;
}

bool JsEngine::handleAssert(const char* msg)
{
    AXLOGE("JS assert: {}", msg ? msg : "(null)");
    return false;
}

bool JsEngine::parseConfig(ConfigType /*type*/, std::string_view /*str*/)
{
    return false;
}

}  // namespace ax
