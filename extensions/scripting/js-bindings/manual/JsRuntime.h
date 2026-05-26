/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

#include "js-bindings/manual/Js-BindingsExport.h"

extern "C" {
#include "quickjs.h"
}

#include <cstddef>
#include <string>

namespace ax
{

/**
 * Minimal QuickJS-NG runtime wrapper (runtime + context lifecycle).
 */
class AX_JS_DLL JsRuntime
{
public:
    static JsRuntime* create();
    void destroy();

    JSContext* getContext() const { return _ctx; }

    /** Evaluate JS source; returns false on exception. */
    bool eval(const char* code, const char* filename = "<eval>");

    /** Evaluate a script file resolved via FileUtils search paths. */
    bool evalFile(const char* filename);

    /** Read global property as int (for tests); returns false if missing or not a number. */
    bool getGlobalInt(const char* name, int& out) const;

    std::string getLastError() const { return _lastError; }

private:
    JsRuntime() = default;
    ~JsRuntime();
    bool init();
    void setErrorFromException();
    void clearError();

    JSRuntime* _rt{nullptr};
    JSContext* _ctx{nullptr};
    std::string _lastError;
};

}  // namespace ax
