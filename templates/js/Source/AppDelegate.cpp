/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"

#ifdef AX_WITH_QUICKJS
#include "axmol/quickjs/QuickJSEngine.h"
#endif

#define USE_VR_RENDERER  0
#define USE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE
#    include "axmol/audio/AudioEngine.h"
#endif

#if USE_VR_RENDERER && defined(AX_ENABLE_VR)
#    include "axmol/vr/VRGenericRenderer.h"
#endif

using namespace ax;

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

// if you want a different context, modify the value of contextAttrs
// it will affect all platforms
void AppDelegate::initContextAttrs()
{
    // set app context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    // powerPreference only affect when RHI backend is D3D
    ContextAttrs contextAttrs = {.powerPreference = PowerPreference::HighPerformance};

    // V-Sync is enabled by default since axmol 2.2.
    // Uncomment to disable V-Sync and unlock FPS.
    // contextAttrs.vsync = false;

    // uncomment if your app need adapt high DPI scale monitors
    // contextAttrs.renderScaleMode = RenderScaleMode::Physical;
    setContextAttrs(contextAttrs);

    // Sets preferred orientation
    const auto orientations = Device::getSupportedOrientations();
    if (bitmask::any(orientations, Device::OrientationMask::Landscape) &&
        bitmask::any(orientations, Device::OrientationMask::ReverseLandscape))
        Device::setPreferredOrientation(Device::Orientation::SensorLandscape);
    else if (bitmask::any(orientations, Device::OrientationMask::Portrait) &&
             bitmask::any(orientations, Device::OrientationMask::ReversePortrait))
        Device::setPreferredOrientation(Device::Orientation::SensorPortrait);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

#ifdef AX_WITH_QUICKJS
    // Initialize QuickJS engine
    _jsEngine = QuickJSEngine::create();
    if (!_jsEngine)
    {
        AXLOG("Failed to create QuickJS engine");
        return false;
    }

    // Add search paths
    FileUtils::getInstance()->addSearchPath("src");
    FileUtils::getInstance()->addSearchPath("res");

    // Load main.js
    std::string mainJsPath = FileUtils::getInstance()->fullPathForFilename("main.js");
    std::string mainJsCode = FileUtils::getInstance()->getStringFromFile(mainJsPath);

    if (mainJsCode.empty())
    {
        AXLOG("Failed to load main.js from path: %s", mainJsPath.c_str());
        return false;
    }

    // Execute main.js
    if (_jsEngine->eval(mainJsCode, mainJsPath).empty())
    {
        AXLOG("Failed to execute main.js: %s", _jsEngine->getLastError().c_str());
        return false;
    }

    AXLOG("JavaScript engine initialized successfully");
#else
    AXLOG("Warning: QuickJS not available. Please rebuild with AX_WITH_QUICKJS=ON");
    return false;
#endif

#if USE_VR_RENDERER && defined(AX_ENABLE_VR)
    auto renderView = Director::getInstance()->getRenderView();
    if (renderView)
    {
        auto vrRenderer = VRGenericRenderer::create();
        if (vrRenderer)
        {
            renderView->setRenderer(vrRenderer);
        }
    }
#endif

    return true;
}

/**
 @brief  Called when the application moves to the background
 @param  the pointer of the application
 */
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->pause();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

/**
 @brief  Called when the application reenters the foreground
 @param  the pointer of the application
 */
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->resume();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}

void AppDelegate::applicationWillQuit()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}
