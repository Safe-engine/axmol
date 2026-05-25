/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsCallbacks.h"
#include "js-bindings/manual/JsBasicConversions.h"

namespace ax
{

JsCallback* JsCallback::create(JSContext* ctx, JSValue func)
{
    auto ret = new JsCallback(ctx, func);
    ret->autorelease();
    return ret;
}

JsCallback::JsCallback(JSContext* ctx, JSValue func)
    : _ctx(ctx)
    , _func(JS_DupValue(ctx, func))
{}

JsCallback::~JsCallback()
{
    JS_FreeValue(_ctx, _func);
}

void JsCallback::execute()
{
    JSValue ret = JS_Call(_ctx, _func, JS_UNDEFINED, 0, nullptr);
    if (JS_IsException(ret))
    {
        // Clear exception to prevent lockup
        JSValue exception = JS_GetException(_ctx);
        const char* msg = JS_ToCString(_ctx, exception);
        if (msg)
        {
            printf("[JS Error] Callback exception: %s\n", msg);
            JS_FreeCString(_ctx, msg);
        }
        JS_FreeValue(_ctx, exception);
    }
    JS_FreeValue(_ctx, ret);
}

void JsCallback::execute(JSValue arg)
{
    JSValue ret = JS_Call(_ctx, _func, JS_UNDEFINED, 1, &arg);
    if (JS_IsException(ret))
    {
        JSValue exception = JS_GetException(_ctx);
        const char* msg = JS_ToCString(_ctx, exception);
        if (msg)
        {
            printf("[JS Error] Callback exception: %s\n", msg);
            JS_FreeCString(_ctx, msg);
        }
        JS_FreeValue(_ctx, exception);
    }
    JS_FreeValue(_ctx, ret);
}

void JsCallback::execute(Ref* sender)
{
    JSValue arg = object_to_jsval(_ctx, sender, "ax::Ref");
    execute(arg);
    JS_FreeValue(_ctx, arg);
}

void js_ax_Ref_finalizer(JSRuntime* rt, JSValue val)
{
    Ref* ref = (Ref*)JS_GetOpaque(val, s_ax_Ref_class_id);
    if (ref)
    {
        unregister_js_wrapper(nullptr, ref);
        ref->release();
    }
}

} // namespace ax
