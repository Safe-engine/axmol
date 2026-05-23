# Axmol Project Templates

This directory contains project templates for different development approaches with Axmol:

## Available Templates

### 1. **C++ Template** (`cpp/`)

Pure C++ game development using Axmol engine directly.

**Best for:**
- High-performance games
- Complex native integrations
- Developers comfortable with C++
- Full engine API access

**Create:** `axmol new -l cpp MyGame`

### 2. **Lua Template** (`lua/`)

Lua scripting with Axmol bindings for rapid development.

**Best for:**
- Rapid prototyping
- Game jams
- Developers familiar with Lua
- Quick iteration cycles

**Create:** `axmol new -l lua MyGame`

**Features:**
- Full Lua 5.1+ support
- Axmol API bindings
- Hot reload capability
- Large ecosystem of Lua libraries

### 3. **JavaScript Template** (`js/`)

JavaScript with QuickJS engine for modern JavaScript game development.

**Best for:**
- Web developers transitioning to games
- Modern JavaScript features (ES2020+)
- Developers familiar with JavaScript ecosystem
- Rapid iteration and prototyping

**Create:** `axmol new -l js MyGameJS`

**Features:**
- ES2020 JavaScript support
- QuickJS engine (lightweight, fast)
- Async/await, Promises, classes, etc.
- Easy to learn and debug

### 4. **Common Files** (`common/`)

Shared files and configurations used by all templates:
- Platform-specific build files (Android, iOS, Windows, etc.)
- CMake configuration
- Resource structure

## Choosing a Template

| Aspect | C++ | Lua | JavaScript |
|--------|-----|-----|-----------|
| Performance | ⭐⭐⭐ Excellent | ⭐⭐ Good | ⭐⭐ Good |
| Learning Curve | Steep | Moderate | Easy |
| Development Speed | Slow | Fast | Fast |
| IDE Support | Excellent | Good | Excellent |
| Mobile Performance | Best | Good | Good |
| Features | Full API | Bindings | Bindings |

## Template Structure

Each language-specific template contains:

```
template-name/
├── Source/              # C++ source files
├── Content/
│   ├── src/             # Game scripts (Lua/JS)
│   └── res/             # Game assets
├── cmake/
│   └── modules/         # CMake configuration
├── CMakeLists.txt       # Build configuration
├── axproj-template.json # Project generation config
├── config.json          # Game settings
└── README.md            # Template documentation
```

## Quick Start

1. **Choose a template** based on your preference
2. **Create a project:**
   ```pwsh
   axmol new -p <pkg.name> -d ./projects -l <lang> <ProjectName>
   ```
3. **Build the project:**
   ```pwsh
   cd projects/ProjectName
   axmol
   ```
4. **Run the project:**
   ```pwsh
   axmol run
   ```

## Creating Your Own Template

To create a new template (e.g., for TypeScript compilation):

1. Create a new directory (e.g., `typescript/`)
2. Copy structure from an existing template
3. Modify Source files as needed
4. Create `axproj-template.json` with generation rules
5. Add `CMakeLists.txt` configuration
6. Document in this README

## Resources

- **Axmol Documentation:** https://axmol.dev/
- **QuickJS Documentation:** https://bellard.org/quickjs/
- **Lua Documentation:** https://www.lua.org/
- **CMake Documentation:** https://cmake.org/cmake/help/latest/

## Support

For issues or questions about templates:
- GitHub Issues: https://github.com/axmolengine/axmol/issues
- Community: https://github.com/axmolengine/axmol/discussions
- Documentation: https://axmol.dev/docs/
