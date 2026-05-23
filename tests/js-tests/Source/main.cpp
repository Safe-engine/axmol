/*
 A tiny test runner that uses QuickJS engine to execute a JS test file
*/

#include "axmol/quickjs/QuickJSEngine.h"
#include <iostream>
#include <fstream>
#include <sstream>

static std::string readFile(const std::string &path) {
    std::ifstream ifs(path);
    if (!ifs) return std::string();
    std::ostringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

int main(int argc, char** argv) {
    std::string testPath = "Content/test1.js";
    if (argc > 1) testPath = argv[1];

    auto engine = ax::QuickJSEngine::create();
    if (!engine) {
        std::cerr << "Failed to create QuickJS engine" << std::endl;
        return 2;
    }

    std::string script = readFile(testPath);
    if (script.empty()) {
        std::cerr << "Failed to read test script: " << testPath << std::endl;
        return 3;
    }

    std::string result = engine->eval(script);
    std::cout << "Script eval result: " << result << std::endl;

    return 0;
}
