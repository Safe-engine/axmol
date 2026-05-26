/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsRuntime.h"
// #include "js-bindings/manual/JsBindings.h"
#include "js-bindings/manual/core/axjs_core_manual.h"

#include "axmol/platform/FileUtils.h"
#include "axmol/base/Logging.h"

#include <cstring>

namespace ax
{

JsRuntime* JsRuntime::create()
{
    auto* runtime = new JsRuntime();
    if (!runtime->init())
    {
        delete runtime;
        return nullptr;
    }
    return runtime;
}

void JsRuntime::destroy()
{
    delete this;
}

JsRuntime::~JsRuntime()
{
    if (_ctx)
    {
        JS_FreeContext(_ctx);
        _ctx = nullptr;
    }
    if (_rt)
    {
        JS_FreeRuntime(_rt);
        _rt = nullptr;
    }
}

bool JsRuntime::init()
{
    _rt = JS_NewRuntime();
    if (!_rt)
        return false;

    _ctx = JS_NewContext(_rt);
    if (!_ctx)
        return false;

    // Register all Axmol bindings
    // js_register_all_bindings(_ctx);
    js_register_core_bindings(_ctx);

    return true;
}

void JsRuntime::clearError()
{
    _lastError.clear();
}

void JsRuntime::setErrorFromException()
{
    JSValue exc = JS_GetException(_ctx);
    const char* str = JS_ToCString(_ctx, exc);
    if (str)
    {
        _lastError = str;
        JS_FreeCString(_ctx, str);
    }
    else
    {
        _lastError = "JavaScript exception";
    }

    // Try to extract stack/fileName information from the exception object
    JSValue stack = JS_GetPropertyStr(_ctx, exc, "stack");
    if (!JS_IsUndefined(stack))
    {
        const char* s = JS_ToCString(_ctx, stack);
        if (s)
        {
            _lastError += std::string("\nStack: ") + s;
            JS_FreeCString(_ctx, s);
        }
    }
    JS_FreeValue(_ctx, stack);

    JSValue fname = JS_GetPropertyStr(_ctx, exc, "fileName");
    if (!JS_IsUndefined(fname))
    {
        const char* f = JS_ToCString(_ctx, fname);
        if (f)
        {
            _lastError += std::string("\nFile: ") + f;
            JS_FreeCString(_ctx, f);
        }
    }
    JS_FreeValue(_ctx, fname);

    JS_FreeValue(_ctx, exc);
}

bool JsRuntime::eval(const char* code, const char* filename)
{
    clearError();
    if (!code || !filename)
        return false;

    const size_t len = std::strlen(code);
    JSValue result = JS_Eval(_ctx, code, len, filename, JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result))
    {
        setErrorFromException();
        JS_FreeValue(_ctx, result);
        return false;
    }
    JS_FreeValue(_ctx, result);
    return true;
}

bool JsRuntime::evalFile(const char* filename)
{
    clearError();
    if (!filename)
        return false;

    const std::string path = FileUtils::getInstance()->fullPathForFilename(filename);
    if (path.empty())
    {
        _lastError = std::string("Script file not found: ") + filename;
        return false;
    }

    auto data = FileUtils::getInstance()->getDataFromFile(path);
    if (data.isNull())
    {
        _lastError = std::string("Failed to read script file: ") + path;
        return false;
    }

    const char* bytes = reinterpret_cast<const char*>(data.getBytes());
    const size_t len    = data.getSize();
    AXLOGI("Evaluating script file: {} (size={})", path, len);
    // Log first bytes to detect invalid UTF-8 or BOM
    size_t show = len < 64 ? len : 64;
    std::string hex;
    hex.reserve(show * 3 + 1);
    const unsigned char* ub = reinterpret_cast<const unsigned char*>(bytes);
    for (size_t i = 0; i < show; ++i)
    {
        char buf[4];
        std::snprintf(buf, sizeof(buf), "%02x ", ub[i]);
        hex += buf;
    }
    AXLOGI("First {} bytes: {}", show, hex);
    JSValue result =
        JS_Eval(_ctx, bytes, len, path.c_str(), JS_EVAL_TYPE_GLOBAL);
    if (JS_IsException(result))
    {
        setErrorFromException();
        // Append path and first-bytes hex to the last error for easier debugging
        try
        {
            std::string extra = " [path=" + path + " size=" + std::to_string(len) + " first=" + hex + "]";
            _lastError += extra;
        }
        catch (...) {}
        JS_FreeValue(_ctx, result);
        return false;
    }
    JS_FreeValue(_ctx, result);
    return true;
}

bool JsRuntime::getGlobalInt(const char* name, int& out) const
{
    if (!name)
        return false;

    JSValue global = JS_GetGlobalObject(_ctx);
    JSValue prop   = JS_GetPropertyStr(_ctx, global, name);
    JS_FreeValue(_ctx, global);

    if (JS_IsUndefined(prop) || !JS_IsNumber(prop))
    {
        JS_FreeValue(_ctx, prop);
        return false;
    }

    int32_t v = 0;
    if (JS_ToInt32(_ctx, &v, prop) < 0)
    {
        JS_FreeValue(_ctx, prop);
        return false;
    }
    JS_FreeValue(_ctx, prop);
    out = v;
    return true;
}

}  // namespace ax
