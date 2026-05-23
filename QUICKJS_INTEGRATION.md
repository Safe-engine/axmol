# QuickJS Integration Guide for Axmol Engine

## Overview

This document describes the integration of **QuickJS** - a lightweight JavaScript engine - into the Axmol game engine. QuickJS provides a fast, compact JavaScript runtime that can be embedded in games and applications for scripting, dynamic behavior, and game logic.

## What is QuickJS?

QuickJS is a small and embeddable JavaScript engine that:
- Supports ES2020 JavaScript standard
- Has a very small footprint (~200KB compiled)
- Provides fast execution
- Includes both an interpreter and compiler
- Can be easily integrated into C/C++ applications

## Integration Components

### 1. CMake Build Configuration

**File**: `3rdparty/CMakeLists.txt`

- Added `AX_WITH_QUICKJS` option (enabled by default)
- QuickJS is built as a static library from `3rdparty/3rdparty/quickjs`
- Compilation flags:
  - `QJS_ENABLE_INSTALL OFF` - Don't install QuickJS separately
  - `QJS_BUILD_EXAMPLES OFF` - Skip example compilation

### 2. C++ Wrapper - QuickJSEngine

**Files**:
- `axmol/quickjs/QuickJSEngine.h` - Header with public API
- `axmol/quickjs/QuickJSEngine.cpp` - Implementation

#### Main Features:

```cpp
class QuickJSEngine {
public:
    // Create and destroy
    static std::shared_ptr<QuickJSEngine> create();
    ~QuickJSEngine();
    
    // Evaluate JavaScript code
    std::string eval(const std::string& code, const std::string& filename = "<eval>");
    
    // Call JavaScript functions
    std::string call(const std::string& funcName, const std::vector<std::string>& args = {});
    
    // Global variable access
    std::string getGlobal(const std::string& name);
    void setGlobal(const std::string& name, const std::string& value);
    
    // Function registration (for native callbacks)
    void registerFunction(const std::string& name, 
                         const std::function<std::string(const std::vector<std::string>&)>& func);
    
    // Compilation
    bool compile(const std::string& code);
    
    // Error handling
    std::string getLastError() const;
    void reportException();
    
    // Low-level access
    JSContext* getContext();
};
```

### 3. Module Integration

**File**: `axmol/quickjs/CMakeLists.txt`

Integrates QuickJS into the Axmol library build system.

### 4. Header Export

**File**: `axmol/axmol.h`

QuickJS functionality is exposed via:
```cpp
#ifdef AX_WITH_QUICKJS
#include "axmol/quickjs/QuickJSEngine.h"
#endif
```

## Building with QuickJS

### Default Build

QuickJS is **enabled by default**. To disable it, use:

```bash
cmake -DAX_WITH_QUICKJS=OFF ..
```

### Conditional Compilation

The `axmol/quickjs/QuickJSEngine.h` header is automatically included only if `AX_WITH_QUICKJS` is enabled.

## Usage Examples

### Basic JavaScript Execution

```cpp
#include "axmol/axmol.h"

// Create engine
auto js = ax::QuickJSEngine::create();

// Evaluate code
std::string result = js->eval("2 + 3 * 4");
// result = "14"
```

### Define and Call Functions

```cpp
// Define function
js->eval(R"(
    function addNumbers(a, b) {
        return a + b;
    }
)");

// Call it
std::string result = js->call("addNumbers", {"5", "3"});
// result = "8"
```

### Global Variables

```cpp
// Set
js->setGlobal("playerScore", "1000");

// Get
std::string score = js->getGlobal("playerScore");
// score = "1000"
```

### Error Handling

```cpp
bool success = js->compile("var x = 10; y = x + 5;");
if (!success) {
    std::cerr << "Error: " << js->getLastError() << std::endl;
}
```

### Complex Objects

Since values are converted to/from strings, you can pass JSON:

```cpp
js->setGlobal("config", R"({"width": 800, "height": 600})");
std::string result = js->eval("config.width * config.height");
// result = "480000"
```

## Architecture

```
Axmol Engine
    ↓
axmol/quickjs/QuickJSEngine (C++ Wrapper)
    ↓
QuickJS C API (JS library + runtime)
    ↓
QuickJS (3rdparty/3rdparty/quickjs)
```

## Dependencies

- **QuickJS**: Already included as a git submodule
- **CMake 3.22+**: For building
- **C Compiler**: gcc, clang, or MSVC

## Features & Limitations

### Supported Features
- Full ES2020 JavaScript support
- Module system
- Async/await (via Promise)
- Generators
- Proxies
- WeakMap/WeakSet
- BigInt support

### Current Limitations

The current wrapper:
1. Uses string-based value conversion
2. Limited native function binding (stored but not fully integrated with JS runtime)
3. No garbage collection control from C++

### Future Enhancements

Potential improvements:
1. Native function binding with proper JSValue callbacks
2. Object/class binding for game entities
3. Memory management utilities
4. Performance profiling hooks
5. Async JavaScript support for game loops

## Performance Considerations

- QuickJS is designed for fast startup and execution
- Compilation overhead is minimal
- String conversion between C++ and JS adds overhead - for frequent calls, consider caching objects
- Memory usage is minimal (~5-10MB for typical game scripts)

## Security Notes

- Always validate user-provided JavaScript code before execution
- Use sandboxing for untrusted scripts if needed
- QuickJS provides basic memory limits (can be configured)

## Troubleshooting

### QuickJS Not Found

If CMake can't find QuickJS:
```bash
# Ensure submodule is initialized
git submodule update --init --recursive 3rdparty/3rdparty/quickjs
```

### Build Errors

- Check that CMake 3.22+ is installed
- Ensure C compiler is available
- On Windows, use Visual Studio 2019 or later

### Runtime Errors

Check the error message via:
```cpp
std::cerr << engine->getLastError() << std::endl;
```

## See Also

- QuickJS Documentation: https://bellard.org/quickjs/
- QuickJS GitHub: https://github.com/quickjs-ng/quickjs
- Axmol Engine: https://axmol.dev/

## Contributing

Improvements to the QuickJS integration are welcome! Please consider:
1. Adding more binding examples
2. Improving error messages
3. Optimizing value conversion
4. Adding native function binding support

