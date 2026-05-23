# JavaScript Project Template for Axmol

This is a template for creating JavaScript-based games and applications using the Axmol engine with QuickJS scripting support.

## Overview

This template demonstrates how to use the **QuickJS JavaScript engine** with Axmol for game development. It provides:

- **C++ Application Framework** - AppDelegate handles initialization and lifecycle
- **QuickJS Integration** - Execute JavaScript code for game logic
- **Asset Management** - Simple structure for organizing game resources
- **Cross-platform Support** - Build for Windows, macOS, Linux, iOS, Android, and WebAssembly

## Project Structure

```
HelloJS/
├── Source/
│   ├── AppDelegate.h        # Application entry point (header)
│   └── AppDelegate.cpp      # Application entry point (implementation)
├── Content/
│   ├── src/
│   │   └── main.js          # JavaScript entry point
│   └── res/
│       └── [game assets]
├── cmake/
│   └── modules/             # CMake build modules
├── proj.android/            # Android project files
├── proj.win32/              # Windows project files
├── proj.ios_mac/            # iOS/macOS project files
├── proj.linux/              # Linux project files
├── proj.wasm/               # WebAssembly project files
├── proj.winrt/              # Windows RT/UWP project files
├── CMakeLists.txt           # Main build configuration
└── config.json              # Game configuration
```

## How It Works

### 1. **C++ Side (AppDelegate.cpp)**

The AppDelegate initializes the Axmol engine and:
1. Creates a QuickJS engine instance
2. Loads `Content/src/main.js`
3. Executes the JavaScript code to initialize your game

### 2. **JavaScript Side (main.js)**

Write your game logic in JavaScript:

```javascript
// Game configuration
const gameConfig = {
    title: "My Game",
    width: 960,
    height: 640,
    fps: 60
};

// Initialize game
function initGame() {
    console.log("Game starting...");
    // Your initialization code here
}

// Update game logic (called every frame)
function updateGame(deltaTime) {
    // Your game update code here
}

// Cleanup
function cleanupGame() {
    console.log("Game ending...");
}

// Start the game
initGame();
```

## Building Your Project

### Windows
```pwsh
axmol -p win32
```

### macOS
```pwsh
axmol -p osx
```

### Linux
```pwsh
axmol -p linux
```

### iOS
```pwsh
axmol -p ios -a arm64 -c
```

### Android
```pwsh
axmol -p android -a arm64
```

### WebAssembly
```pwsh
axmol -p wasm
```

## JavaScript Features

### ES2020 Support

QuickJS supports modern JavaScript features:
- Async/await and Promises
- Arrow functions
- Classes
- Template literals
- Destructuring
- Spread operator
- And more...

### Available APIs

JavaScript code can access Axmol engine capabilities through C++ bindings. To add custom bindings, modify `AppDelegate.cpp` to expose engine functions:

```cpp
// In AppDelegate.cpp, you can register native functions:
_jsEngine->registerFunction("createLabel", [](const std::vector<std::string>& args) {
    // Create a label in the engine and return result
    return "label_created";
});
```

### File Access

Use the FileUtils API to load assets:

```cpp
// In C++, you can expose file utilities to JavaScript
std::string assetPath = FileUtils::getInstance()->fullPathForFilename("myimage.png");
```

## Development Workflow

### 1. Edit Your Game

1. Edit `Content/src/main.js` with your game logic
2. Add game assets to `Content/res/`
3. Modify `Content/config.json` for game settings

### 2. Build the Project

```pwsh
axmol
```

### 3. Run the Project

```pwsh
axmol run
```

### 4. Debug

- **Console Output** - Use `console.log()` in JavaScript
- **Error Messages** - Check AppDelegate logs for QuickJS errors
- **Source Debugging** - Use your platform's native debugger

## Configuration

### config.json

Configure your game:

```json
{
    "init_cfg": {
        "isLandscape": true,
        "width": 960,
        "height": 640,
        "entry": "src/main.js",
        "consolePort": 6050,
        "uploadPort": 6060
    }
}
```

**Options:**
- `isLandscape` - Window orientation (true=landscape, false=portrait)
- `width`, `height` - Window dimensions
- `entry` - Path to JavaScript entry point
- `consolePort` - Debug console port
- `uploadPort` - Asset upload port

## Performance Tips

1. **Minimize String Conversions** - The C++ ↔ JS bridge uses string conversion, which is slower for large datasets
2. **Cache Objects** - Store frequently used values in JavaScript variables
3. **Use Native Code for Hot Loops** - Critical game loops should be in C++ if performance is needed
4. **Profile Your Code** - Use browser DevTools or profilers to find bottlenecks

## Troubleshooting

### JavaScript doesn't load

1. Check that `Content/src/main.js` exists
2. Verify file paths in AppDelegate.cpp
3. Check logs for QuickJS errors

### QuickJS not available

Build with QuickJS support:
```pwsh
axmol -xc "-DAX_WITH_QUICKJS=ON"
```

### Runtime errors

1. Check console output for error messages
2. Verify JavaScript syntax with a linter
3. Use try/catch in JavaScript for error handling

## Next Steps

1. **Read QuickJS Documentation** - https://bellard.org/quickjs/
2. **Explore Axmol APIs** - https://axmol.dev/docs
3. **Study Example Code** - Check the tests/quickjs-example.cpp
4. **Join Community** - https://github.com/axmolengine/axmol

## License

This template is part of Axmol Engine and uses the same license (MIT).

See [LICENSE](../../LICENSE) for details.
