/**
 * Copyright (c) 2024 Axmol Engine contributors
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "QuickJSEngine.h"
#include "quickjs.h"
#include <cstring>
#include <sstream>
#include <iostream>

namespace ax {

std::shared_ptr<QuickJSEngine> QuickJSEngine::create() {
    auto engine = std::shared_ptr<QuickJSEngine>(new QuickJSEngine());
    if (!engine->_initialize()) {
        return nullptr;
    }
    return engine;
}

QuickJSEngine::QuickJSEngine() 
    : _runtime(nullptr), _context(nullptr) {}

QuickJSEngine::~QuickJSEngine() {
    if (_context) {
        JS_FreeContext(_context);
    }
    if (_runtime) {
        JS_FreeRuntime(_runtime);
    }
}

bool QuickJSEngine::_initialize() {
    _runtime = JS_NewRuntime();
    if (!_runtime) {
        _lastError = "Failed to create QuickJS runtime";
        return false;
    }

    _context = JS_NewContext(_runtime);
    if (!_context) {
        _lastError = "Failed to create QuickJS context";
        JS_FreeRuntime(_runtime);
        _runtime = nullptr;
        return false;
    }

    return true;
}

std::string QuickJSEngine::eval(const std::string& code, const std::string& filename) {
    JSValue val = JS_Eval(_context, code.c_str(), code.length(), 
                          filename.c_str(), JS_EVAL_TYPE_GLOBAL);
    
    if (JS_IsException(val)) {
        reportException();
        return "";
    }

    // Convert result to string
    JSValue str = JS_ToString(_context, val);
    const char* cstr = JS_ToCString(_context, str);
    std::string result = cstr ? cstr : "";
    
    JS_FreeCString(_context, cstr);
    JS_FreeValue(_context, str);
    JS_FreeValue(_context, val);

    return result;
}

std::string QuickJSEngine::call(const std::string& funcName, 
                                const std::vector<std::string>& args) {
    JSValue global = JS_GetGlobalObject(_context);
    JSValue func = JS_GetPropertyStr(_context, global, funcName.c_str());
    
    if (!JS_IsFunction(_context, func)) {
        _lastError = "Function '" + funcName + "' not found or not callable";
        JS_FreeValue(_context, func);
        JS_FreeValue(_context, global);
        return "";
    }

    // Convert string arguments to JSValues
    std::vector<JSValue> jsArgs;
    for (const auto& arg : args) {
        JSValue val = JS_Eval(_context, arg.c_str(), arg.length(), 
                              "<arg>", JS_EVAL_TYPE_GLOBAL);
        jsArgs.push_back(val);
    }

    JSValue result = JS_Call(_context, func, JS_UNDEFINED, 
                             jsArgs.size(), jsArgs.data());

    // Clean up arguments
    for (auto& val : jsArgs) {
        JS_FreeValue(_context, val);
    }

    if (JS_IsException(result)) {
        reportException();
        JS_FreeValue(_context, func);
        JS_FreeValue(_context, global);
        return "";
    }

    // Convert result to string
    JSValue str = JS_ToString(_context, result);
    const char* cstr = JS_ToCString(_context, str);
    std::string resultStr = cstr ? cstr : "";
    
    JS_FreeCString(_context, cstr);
    JS_FreeValue(_context, str);
    JS_FreeValue(_context, result);
    JS_FreeValue(_context, func);
    JS_FreeValue(_context, global);

    return resultStr;
}

std::string QuickJSEngine::getGlobal(const std::string& name) {
    JSValue global = JS_GetGlobalObject(_context);
    JSValue val = JS_GetPropertyStr(_context, global, name.c_str());

    if (JS_IsUndefined(val)) {
        JS_FreeValue(_context, val);
        JS_FreeValue(_context, global);
        return "";
    }

    JSValue str = JS_ToString(_context, val);
    const char* cstr = JS_ToCString(_context, str);
    std::string result = cstr ? cstr : "";

    JS_FreeCString(_context, cstr);
    JS_FreeValue(_context, str);
    JS_FreeValue(_context, val);
    JS_FreeValue(_context, global);

    return result;
}

void QuickJSEngine::setGlobal(const std::string& name, const std::string& value) {
    JSValue global = JS_GetGlobalObject(_context);
    JSValue val = JS_Eval(_context, value.c_str(), value.length(), 
                          "<value>", JS_EVAL_TYPE_GLOBAL);
    
    if (JS_IsException(val)) {
        reportException();
    } else {
        JS_SetPropertyStr(_context, global, name.c_str(), val);
    }
    
    JS_FreeValue(_context, global);
}

void QuickJSEngine::registerFunction(const std::string& name,
                                     const std::function<std::string(const std::vector<std::string>&)>& func) {
    _nativeFunctions[name] = func;
    
    // This is a simplified version - in production, you'd create proper C closures
    // that can access the stored function pointers
}

bool QuickJSEngine::compile(const std::string& code) {
    JSValue val = JS_Eval(_context, code.c_str(), code.length(),
                          "<compile>", JS_EVAL_TYPE_GLOBAL | JS_EVAL_FLAG_COMPILE_ONLY);
    
    if (JS_IsException(val)) {
        reportException();
        return false;
    }

    JS_FreeValue(_context, val);
    return true;
}

void QuickJSEngine::reportException() {
    JSValue exc = JS_GetException(_context);
    
    JSValue val = JS_ToString(_context, exc);
    const char* str = JS_ToCString(_context, val);
    
    if (str) {
        _lastError = str;
        std::cerr << "QuickJS Exception: " << str << std::endl;
    }

    JS_FreeCString(_context, str);
    JS_FreeValue(_context, val);
    JS_FreeValue(_context, exc);
}

} // namespace ax
