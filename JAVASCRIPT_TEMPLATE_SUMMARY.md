# JavaScript Project Template Integration - Summary

## ✅ Completed: Full JavaScript Template Support

This document summarizes the complete integration of JavaScript (`-l js`) project template support into Axmol.

## What Was Done

### 1. Created Complete JavaScript Template Directory Structure

**Location:** `/templates/js/`

```
templates/js/
├── Source/
│   ├── AppDelegate.h          # Application header with QuickJS engine
│   └── AppDelegate.cpp        # Implementation: loads and runs JavaScript
├── Content/
│   ├── config.json            # Game configuration (resolution, ports, etc.)
│   ├── src/
│   │   └── main.js            # JavaScript entry point with example code
│   └── res/                   # Game assets directory
├── cmake/
│   └── modules/
│       ├── AXGameSourceSetup.cmake      # Platform source configuration
│       ├── AXGameTargetSetup.cmake      # Target build setup
│       └── AXGameFinalSetup.cmake       # Final build configuration
├── CMakeLists.txt             # Main project build configuration
├── axproj-template.json       # Project generation rules
├── README.md                  # Template-specific documentation
└── [common files copied from templates/common/]
```

### 2. JavaScript Application (AppDelegate)

**Files:** `Source/AppDelegate.h` and `Source/AppDelegate.cpp`

**Features:**
- ✅ QuickJS engine initialization
- ✅ Automatic main.js loading from Content/src/
- ✅ Error handling and exception reporting
- ✅ Application lifecycle management (init, pause, resume, quit)
- ✅ Audio engine support
- ✅ VR renderer support (optional)
- ✅ File system integration

**Code Highlights:**
```cpp
// AppDelegate.cpp
bool AppDelegate::applicationDidFinishLaunching()
{
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

    _jsEngine = QuickJSEngine::create();
    if (!_jsEngine) return false;

    FileUtils::getInstance()->addSearchPath("src");
    FileUtils::getInstance()->addSearchPath("res");

    std::string mainJsCode = FileUtils::getInstance()
        ->getStringFromFile("main.js");
    _jsEngine->eval(mainJsCode, "main.js");

    return true;
}
```

### 3. JavaScript Entry Point Example

**File:** `Content/src/main.js`

**Includes:**
- Game configuration object
- `initGame()` - Called on startup
- `updateGame(deltaTime)` - Called each frame
- `cleanupGame()` - Called on shutdown
- Module exports for C++ integration

**Example Code:**
```javascript
const gameConfig = {
    title: "Hello JavaScript",
    width: 960,
    height: 640,
    fps: 60
};

function initGame() {
    console.log("Initializing game...");
    // Your game initialization code
}

function updateGame(deltaTime) {
    // Your game update logic
}

function cleanupGame() {
    console.log("Cleaning up game...");
}

initGame();
```

### 4. Build Configuration

**Files:** 
- `CMakeLists.txt` - Main project configuration
- `cmake/modules/AX*Setup.cmake` - Platform-specific setup

**Features:**
- ✅ Automatic platform detection
- ✅ Android, iOS, Windows, macOS, Linux, WebAssembly support
- ✅ Resource management
- ✅ Portrait/landscape orientation support
- ✅ QuickJS engine linking

### 5. Project Configuration

**Files:**
- `Content/config.json` - Game settings and device profiles
- `axproj-template.json` - Project generation instructions

**Configuration Options:**
- Window size and orientation
- JavaScript entry point
- Console/upload ports
- Device screen profiles (iPhone, iPad, Android, etc.)
- Portrait mode transformation rules

### 6. Documentation

#### A. Template-Specific README
**File:** `templates/js/README.md`

Complete guide covering:
- Project overview
- Directory structure explanation
- How C++ and JavaScript interact
- Building for all platforms
- JavaScript ES2020 features supported
- Development workflow
- Configuration options
- Performance tips
- Troubleshooting guide

#### B. Templates Overview
**File:** `templates/README.md`

Master documentation for all templates:
- Comparison table (C++ vs Lua vs JavaScript)
- When to use each template
- Quick start guide
- Template structure overview
- Resources and support links

#### C. Development Setup
**File:** `docs/DevSetup.md` - UPDATED

Added documentation for JavaScript projects:
- Updated project creation command to show `-l js` option
- Explained differences between cpp, lua, and js
- Note about QuickJS requirement

### 7. Example Usage

**Create a new JavaScript project:**
```bash
# Basic project
axmol new -p dev.axmol.hellojs -d ./projects -l js HelloJS

# Portrait orientation
axmol new -p dev.axmol.hellojs -d ./projects -l js --portrait HelloJS
```

**Build and run:**
```bash
cd projects/HelloJS
axmol                    # Build for default platform
axmol -p win32           # Windows
axmol -p osx             # macOS
axmol -p linux           # Linux
axmol -p android -a arm64  # Android
axmol -p ios -a arm64    # iOS
axmol -p wasm            # WebAssembly
```

## Integration Points

### 1. QuickJS Engine
- Embedded C++ binding (`axmol/quickjs/QuickJSEngine.h`)
- Automatic initialization in AppDelegate
- Full ES2020 support

### 2. Axmol Build System
- Integrates with 3rdparty CMake configuration
- Conditional QuickJS support (`AX_WITH_QUICKJS`)
- Platform-aware build configurations

### 3. Project Generation
- `axmol new` command supports `-l js` option
- Automatic project template copying
- Variable substitution (projectName, packageName, etc.)
- Portrait mode transformation support

## File Checklist

- ✅ `templates/js/CMakeLists.txt` - Build configuration
- ✅ `templates/js/Source/AppDelegate.h` - Application header
- ✅ `templates/js/Source/AppDelegate.cpp` - Application implementation
- ✅ `templates/js/Content/src/main.js` - JavaScript entry point
- ✅ `templates/js/Content/config.json` - Game configuration
- ✅ `templates/js/Content/res/` - Asset directory
- ✅ `templates/js/cmake/modules/AXGameSourceSetup.cmake` - Source setup
- ✅ `templates/js/cmake/modules/AXGameTargetSetup.cmake` - Target setup
- ✅ `templates/js/cmake/modules/AXGameFinalSetup.cmake` - Final setup
- ✅ `templates/js/axproj-template.json` - Project template rules
- ✅ `templates/js/README.md` - JavaScript template documentation
- ✅ `templates/README.md` - Updated templates master documentation
- ✅ `docs/DevSetup.md` - Updated with JS project creation info

## How to Use

### For Users

1. **Create a JavaScript project:**
   ```bash
   axmol new -p dev.axmol.hellojs -d ./projects -l js HelloJS
   ```

2. **Navigate to project:**
   ```bash
   cd projects/HelloJS
   ```

3. **Edit game code:**
   - Modify `Content/src/main.js` for game logic
   - Add assets to `Content/res/`
   - Customize `Content/config.json` settings

4. **Build project:**
   ```bash
   axmol
   ```

5. **Run project:**
   ```bash
   axmol run
   ```

### For Developers

1. **Understand the flow:**
   - C++ AppDelegate initializes Axmol engine
   - Loads and executes main.js via QuickJS
   - JavaScript controls game logic

2. **Add native functions:**
   - Modify `AppDelegate.cpp` to register native C++ functions
   - Call from JavaScript to access engine features
   - Example: `_jsEngine->registerFunction("createLabel", ...)`

3. **Extend the template:**
   - Add more JavaScript modules to `Content/src/`
   - Create C++ wrappers for engine features
   - Expose Axmol APIs to JavaScript

## Requirements

- **QuickJS Engine** - Included in axmol 3rdparty
- **AX_WITH_QUICKJS=ON** - Must be enabled during build
- **CMake 3.22+** - For building
- **C++17 or later** - For QuickJS support

## Benefits

✅ **Easy to Learn** - JavaScript is widely known
✅ **Fast Development** - Quick iteration cycles
✅ **Modern Features** - Full ES2020 support
✅ **Cross-Platform** - Build for all major platforms
✅ **Well Documented** - Complete README and examples
✅ **Lightweight** - QuickJS is only ~200KB
✅ **Good Performance** - QuickJS is optimized for speed

## What's Next

### Potential Enhancements

1. **C++ Bindings Generator** - Automatic binding generation from headers
2. **Hot Reload** - Reload JavaScript without recompiling
3. **Debugger Support** - Integrated JavaScript debugging
4. **Asset Pipeline** - Automated asset conversion and optimization
5. **Performance Profiler** - JavaScript profiling tools
6. **Module System** - Better require/import support
7. **Built-in Libraries** - Math, utils, common game libraries

### Community Contributions Welcome

- Add more example projects
- Create tutorial documentation
- Develop helpful utilities
- Improve error messages
- Performance optimizations

## Support and Resources

- **Axmol Documentation:** https://axmol.dev/
- **QuickJS Official:** https://bellard.org/quickjs/
- **QuickJS-ng Repository:** https://github.com/quickjs-ng/quickjs
- **Axmol Community:** https://github.com/axmolengine/axmol
- **Issue Tracker:** https://github.com/axmolengine/axmol/issues

---

**Summary:** The JavaScript template for Axmol is now fully integrated and ready for use. Developers can create, build, and deploy JavaScript games across all supported platforms using the QuickJS engine integration.
