/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

#include "axmol/base/ScriptSupport.h"
#include "js-bindings/manual/JsRuntime.h"
#include "js-bindings/manual/Js-BindingsExport.h"

namespace ax
{

/**
 * QuickJS-NG script engine implementing ScriptEngineProtocol (minimal subset).
 */
class AX_JS_DLL JsEngine : public ScriptEngineProtocol
{
public:
    static JsEngine* getInstance();
    virtual ~JsEngine();

    ccScriptType getScriptType() override { return kScriptTypeJavaScript; }

    JsRuntime* getJsRuntime() { return _runtime; }

    int executeString(const char* codes) override;
    int executeScriptFile(const char* filename) override;
    int executeGlobalFunction(const char* functionName) override;
    int sendEvent(const ScriptEvent& evt) override;
    bool handleAssert(const char* msg) override;
    bool parseConfig(ConfigType type, std::string_view str) override;

private:
    JsEngine() = default;
    bool init();

    static JsEngine* _defaultEngine;
    JsRuntime* _runtime{nullptr};
};

}  // namespace ax
