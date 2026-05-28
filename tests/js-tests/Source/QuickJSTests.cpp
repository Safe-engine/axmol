/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "QuickJSTests.h"

#include "js-bindings/manual/JsEngine.h"
#include "js-bindings/manual/JsRuntime.h"

#include "axmol/base/Logging.h"

using namespace ax;

namespace
{

struct TestContext
{
    int failed = 0;

    void check(bool ok, const char* name)
    {
        if (ok)
            AXLOGI("[PASS] {}\n", name);
        else
        {
            AXLOGE("[FAIL] {}\n", name);
            ++failed;
        }
    }
};

void testEvalArithmetic(TestContext& ctx)
{
    auto* rt = JsRuntime::create();
    if (!rt)
    {
        ctx.check(false, "eval arithmetic (runtime create)");
        return;
    }

    bool ok = rt->eval("globalThis.__axSmoke = 40 + 2;");
    int value = 0;
    ok = ok && rt->getGlobalInt("__axSmoke", value) && value == 42;
    if (!ok && !rt->getLastError().empty())
        AXLOGE("  {}\n", rt->getLastError());

    ctx.check(ok, "eval arithmetic");
    rt->destroy();
}

void testJsEngineExecuteString(TestContext& ctx)
{
    auto* engine = JsEngine::getInstance();
    bool ok      = engine != nullptr && engine->getScriptType() == kScriptTypeJavaScript;
    ok           = ok && engine->executeString("globalThis.__axEngine = 7 * 6;") == 0;

    int value = 0;
    ok        = ok && engine->getJsRuntime()->getGlobalInt("__axEngine", value) && value == 42;

    ctx.check(ok, "JsEngine executeString");
}

void testEvalScriptFile(TestContext& ctx)
{
    auto* rt = JsRuntime::create();
    if (!rt)
    {
        ctx.check(false, "eval script file (runtime create)");
        return;
    }

    bool ok = rt->evalFile("src/index.js");
    if (!ok && !rt->getLastError().empty())
        AXLOGE("  {}\n", rt->getLastError());

    ctx.check(ok, "eval script file");
    // rt->destroy();
}

}  // namespace

int runQuickJSTests()
{
    AXLOGI("Running QuickJS runtime tests on device...\n");

    TestContext ctx;
    // testEvalArithmetic(ctx);
    // testJsEngineExecuteString(ctx);
    testEvalScriptFile(ctx);

    if (ctx.failed == 0)
        AXLOGI("All QuickJS tests passed.\n");
    else
        AXLOGE("{} QuickJS test(s) failed.\n", ctx.failed);

    return ctx.failed;
}
