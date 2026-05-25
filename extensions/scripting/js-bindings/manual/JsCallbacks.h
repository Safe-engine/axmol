/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

extern "C" {
#include "quickjs.h"
}

#include "axmol/base/Object.h"
#include "js-bindings/manual/Js-BindingsExport.h"
#include <functional>

namespace ax
{

/**
 * Helper class to hold a JS function callback and execute it.
 * Manages the lifetime of the JSValue (duplicates/frees it).
 */
class AX_JS_DLL JsCallback : public Object
{
public:
    static JsCallback* create(JSContext* ctx, JSValue func);

    void execute();
    void execute(JSValue arg);
    void execute(Object* sender);

    JSValue getJSFunction() const { return _func; }

private:
    JsCallback(JSContext* ctx, JSValue func);
    virtual ~JsCallback();

    JSContext* _ctx;
    JSValue _func;
};

// Global finalizer for all C++ Ref objects wrapped in QuickJS
AX_JS_DLL void js_ax_Ref_finalizer(JSRuntime* rt, JSValue val);

} // namespace ax
