/**
 * Copyright (c) 2024 Axmol Engine contributors
 *
 * Example demonstrating QuickJS integration with Axmol
 */

#include "axmol/quickjs/QuickJSEngine.h"
#include <iostream>

int main() {
    // Create QuickJS engine
    auto engine = ax::QuickJSEngine::create();
    if (!engine) {
        std::cerr << "Failed to create QuickJS engine" << std::endl;
        return 1;
    }

    std::cout << "QuickJS Engine initialized successfully!" << std::endl;

    // Simple evaluation example
    std::string result = engine->eval("2 + 3");
    std::cout << "2 + 3 = " << result << std::endl;

    // Function definition and call
    engine->eval("function greet(name) { return 'Hello, ' + name + '!'; }");
    result = engine->call("greet", {"\"World\""});
    std::cout << "Function result: " << result << std::endl;

    // Set and get global variables
    engine->setGlobal("myVar", "42");
    result = engine->getGlobal("myVar");
    std::cout << "Global variable: " << result << std::endl;

    // Error handling
    bool compiled = engine->compile("var x = 100;");
    std::cout << "Compilation successful: " << (compiled ? "yes" : "no") << std::endl;

    return 0;
}
