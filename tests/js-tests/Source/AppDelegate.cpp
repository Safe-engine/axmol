/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "AppDelegate.h"

#include "QuickJSTests.h"
#include "js-bindings/manual/JsEngine.h"

using namespace ax;

static Vec2 gWindowSize = Vec2(800, 600);

void AppDelegate::initContextAttrs()
{
    ContextAttrs contextAttrs = {8, 8, 8, 8, 24, 8, 0};
    setContextAttrs(contextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    ax::setLogFmtFlag(ax::LogFmtFlag::Full);

    auto director   = Director::getInstance();
    auto renderView = director->getRenderView();
    if (!renderView)
    {
        std::string title = "JS Runtime Test";
#ifdef AX_PLATFORM_GLFW
        renderView = RenderViewImpl::createWithRect(title, Rect(0, 0, gWindowSize.x, gWindowSize.y), 1.0F, true);
#else
        renderView = RenderViewImpl::createWithRect(title, Rect(0, 0, gWindowSize.x, gWindowSize.y));
#endif
        director->setRenderView(renderView);
    }

#ifdef AX_PLATFORM_PC
    director->setAnimationInterval(1.0f / Device::getDisplayRefreshRate());
#else
    director->setAnimationInterval(1.0f / 60);
#endif

    renderView->setDesignResolutionSize(720, 1280, ResolutionPolicy::SHOW_ALL);

    ScriptEngineManager::getInstance()->setScriptEngine(JsEngine::getInstance());

    auto scene = Scene::create();
    director->runWithScene(scene);
    // Ensure the scene becomes the running scene before JavaScript tests execute.
    director->drawScene();

    const int failed = runQuickJSTests();

    const char* summary = failed == 0 ? "QuickJS: all tests passed" : "QuickJS: tests failed (see log)";
    auto label          = Label::createWithSystemFont(summary, "Helvetica", 20);
    label->setPosition(renderView->getVisibleSize().width * 0.5f, renderView->getVisibleSize().height - 40);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    if (failed != 0)
        label->setColor(Color32::RED);
    scene->addChild(label, 100);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}
