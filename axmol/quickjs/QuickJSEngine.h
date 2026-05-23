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

#pragma once

#include "axmol/axmol.h"

#include <string>
#include <functional>
#include <map>
#include <memory>
#include <vector>
#include <cstdint>

// Forward declarations from QuickJS
struct JSRuntime;
struct JSContext;
union JSValue;
typedef int JSAtom;
typedef int JSClassID;
typedef JSValue (*JSCFunction)(JSContext *ctx, JSValue this_val, int argc, JSValue *argv);

namespace ax {

class AX_DLL QuickJSEngine {
public:
    /**
     * Creates a QuickJS runtime and context
     */
    static std::shared_ptr<QuickJSEngine> create();

    /**
     * Destructor - cleans up QuickJS resources
     */
    ~QuickJSEngine();

    /**
     * Evaluates JavaScript code and returns the result
     * @param code JavaScript source code
     * @param filename Optional filename for error reporting
     * @return Result value as a string (or error message)
     */
    std::string eval(const std::string& code, const std::string& filename = "<eval>");

    /**
     * Calls a JavaScript function
     * @param funcName Name of the function to call
     * @param args Arguments to pass (as JSON strings)
     * @return Result as a string
     */
    std::string call(const std::string& funcName, const std::vector<std::string>& args = {});

    /**
     * Gets a global variable value
     * @param name Variable name
     * @return Value as string
     */
    std::string getGlobal(const std::string& name);

    /**
     * Sets a global variable
     * @param name Variable name
     * @param value Value as a string (can be JSON)
     */
    void setGlobal(const std::string& name, const std::string& value);

    /**
     * Registers a native C++ function callable from JavaScript
     * @param name Function name in JavaScript
     * @param func C++ function implementing the callback
     */
    void registerFunction(const std::string& name, 
                         const std::function<std::string(const std::vector<std::string>&)>& func);

    /**
     * Compiles JavaScript code without executing it
     * @param code Source code to compile
     * @return True if compilation succeeded
     */
    bool compile(const std::string& code);

    /**
     * Gets the last error message
     */
    std::string getLastError() const { return _lastError; }

    /**
     * Checks for pending exceptions and reports them
     */
    void reportException();

    /**
     * Gets raw QuickJS context (for advanced users)
     */
    JSContext* getContext() { return _context; }

private:
    QuickJSEngine();

    bool _initialize();

    JSRuntime* _runtime = nullptr;
    JSContext* _context = nullptr;
    std::string _lastError;
    std::map<std::string, std::function<std::string(const std::vector<std::string>&)>> _nativeFunctions;
};

} // namespace ax
