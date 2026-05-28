/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsBindings.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "js-bindings/manual/core/axjs_core_manual.h"

#include "axmol/2d/SpriteFrameCache.h"
#include "axmol/base/Director.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Global class IDs that are used by multiple binding modules
JSClassID s_spriteClassID;
JSClassID s_sceneClassID;
JSClassID s_directorClassID;
JSClassID s_textureClassID;
JSClassID s_actionClassID;
JSClassID s_vec2ClassID;
JSClassID s_spriteFrameCacheClassID;
JSClassID s_spriteFrameClassID;
static bool s_jsClassIDsInitialized = false;

static void ensureJsClassIDs(JSRuntime* rt)
{
    if (s_jsClassIDsInitialized)
        return;

    JS_NewClassID(rt, &s_spriteClassID);
    JS_NewClassID(rt, &s_sceneClassID);
    JS_NewClassID(rt, &s_directorClassID);
    JS_NewClassID(rt, &s_textureClassID);
    JS_NewClassID(rt, &s_actionClassID);
    JS_NewClassID(rt, &s_vec2ClassID);
    JS_NewClassID(rt, &s_spriteFrameCacheClassID);
    JS_NewClassID(rt, &s_spriteFrameClassID);

    // Also initialize the Ref class ID for conversions
    if (s_ax_Ref_class_id == 0)
    {
        s_ax_Ref_class_id = s_spriteClassID;  // We just need A class ID that we use for all Ref objects
    }

    s_jsClassIDsInitialized = true;
}

// SpriteFrameCache.getInstance() binding
static JSValue js_SpriteFrameCache_getInstance(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    SpriteFrameCache* cache = SpriteFrameCache::getInstance();
    if (!cache)
        return JS_NULL;

    return object_to_jsval(ctx, (void*)cache, "SpriteFrameCache");
}

// SpriteFrameCache.addSpriteFramesWithFile(plist) binding
static JSValue js_SpriteFrameCache_addSpriteFramesWithFile(JSContext* ctx,
                                                           JSValueConst this_val,
                                                           int argc,
                                                           JSValueConst* argv)
{
    SpriteFrameCache* cache = (SpriteFrameCache*)JS_GetOpaque(this_val, 0);  // 0 for non-Ref class ID
    if (!cache)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    const char* plist = JS_ToCString(ctx, argv[0]);
    if (!plist)
        return JS_EXCEPTION;

    cache->addSpriteFramesWithFile(plist);
    JS_FreeCString(ctx, plist);

    return JS_UNDEFINED;
}

// View object stubs
static JSValue js_view_getDesignResolutionSize(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "width", JS_NewInt32(ctx, 720));
    JS_SetPropertyStr(ctx, obj, "height", JS_NewInt32(ctx, 1280));
    return obj;
}

static JSValue js_view_enableRetina(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    return JS_UNDEFINED;
}

static JSValue js_view_adjustViewPort(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    return JS_UNDEFINED;
}

static JSValue js_view_setDesignResolutionSize(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    return JS_UNDEFINED;
}

static JSValue js_view_resizeWithBrowserSize(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    return JS_UNDEFINED;
}

// game.run() binding
static JSValue js_game_run(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    if (argc < 2)
        return JS_FALSE;

    // Get the callback function
    JSValue callback = argv[1];
    if (!JS_IsFunction(ctx, callback))
        return JS_EXCEPTION;

    // Call the callback to initialize the game
    JSValue result = JS_Call(ctx, callback, JS_UNDEFINED, 0, nullptr);
    if (JS_IsException(result))
        return JS_EXCEPTION;

    JS_FreeValue(ctx, result);

    return JS_UNDEFINED;
}

// console.log binding
static JSValue js_console_log(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    std::string line;
    for (int i = 0; i < argc; ++i)
    {
        const char* str = JS_ToCString(ctx, argv[i]);
        if (str)
        {
            if (i > 0)
            {
                line += " ";
            }
            line += str;
            JS_FreeCString(ctx, str);
        }
    }
    AXLOGI("{}", line);
    return JS_UNDEFINED;
}

void js_register_all_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns  = JS_NewObject(ctx);

    JSRuntime* rt = JS_GetRuntime(ctx);
    ensureJsClassIDs(rt);

    // Register ax namespace early so core bindings can attach properties to it
    JS_SetPropertyStr(ctx, global, "ax", JS_DupValue(ctx, ax_ns));

    // Setup class definitions for QuickJS runtime
    JSClassDef spriteClassDef = {"Sprite", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_spriteClassID, &spriteClassDef);
    JSClassDef sceneClassDef = {"Scene", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_sceneClassID, &sceneClassDef);
    JSClassDef directorClassDef = {"Director", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_directorClassID, &directorClassDef);
    JSClassDef textureClassDef = {"Texture", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_textureClassID, &textureClassDef);
    JSClassDef actionClassDef = {"Action", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_actionClassID, &actionClassDef);
    JSClassDef vec2ClassDef = {"Vec2", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_vec2ClassID, &vec2ClassDef);
    JSClassDef spriteFrameCacheClassDef = {"SpriteFrameCache", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_spriteFrameCacheClassID, &spriteFrameCacheClassDef);
    JSClassDef spriteFrameClassDef = {"SpriteFrame", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_spriteFrameClassID, &spriteFrameClassDef);

    // Register all core bindings (Sprite, Scene, Director, Action, Node)
    js_register_core_bindings(ctx);

    // SpriteFrameCache class
    JSValue SFC_class           = JS_NewObject(ctx);
    JSValue sfc_get_instance_fn = JS_NewCFunction(ctx, js_SpriteFrameCache_getInstance, "getInstance", 0);
    JS_SetPropertyStr(ctx, SFC_class, "getInstance", sfc_get_instance_fn);
    JS_SetPropertyStr(ctx, ax_ns, "SpriteFrameCache", SFC_class);

    // SpriteFrameCache prototype
    JSValue sfc_proto = JS_NewObject(ctx);
    JSValue sfc_add_frames_fn =
        JS_NewCFunction(ctx, js_SpriteFrameCache_addSpriteFramesWithFile, "addSpriteFramesWithFile", 1);
    JS_SetPropertyStr(ctx, sfc_proto, "addSpriteFramesWithFile", sfc_add_frames_fn);
    JS_SetClassProto(ctx, s_spriteFrameCacheClassID, sfc_proto);
    register_class_prototype(ctx, "SpriteFrameCache", JS_DupValue(ctx, sfc_proto));

    // Director instance exported to global
    JSValue director_instance = JS_NewObjectClass(ctx, s_directorClassID);
    JS_SetOpaque(director_instance, Director::getInstance());
    JS_SetPropertyStr(ctx, global, "director", director_instance);

    // View object
    JSValue view_obj = JS_NewObject(ctx);
    JSValue view_get_design_res_fn =
        JS_NewCFunction(ctx, js_view_getDesignResolutionSize, "getDesignResolutionSize", 0);
    JSValue view_enable_retina_fn   = JS_NewCFunction(ctx, js_view_enableRetina, "enableRetina", 1);
    JSValue view_adjust_viewport_fn = JS_NewCFunction(ctx, js_view_adjustViewPort, "adjustViewPort", 1);
    JSValue view_set_design_res_fn =
        JS_NewCFunction(ctx, js_view_setDesignResolutionSize, "setDesignResolutionSize", 3);
    JSValue view_resize_browser_fn = JS_NewCFunction(ctx, js_view_resizeWithBrowserSize, "resizeWithBrowserSize", 1);
    JS_SetPropertyStr(ctx, view_obj, "getDesignResolutionSize", view_get_design_res_fn);
    JS_SetPropertyStr(ctx, view_obj, "enableRetina", view_enable_retina_fn);
    JS_SetPropertyStr(ctx, view_obj, "adjustViewPort", view_adjust_viewport_fn);
    JS_SetPropertyStr(ctx, view_obj, "setDesignResolutionSize", view_set_design_res_fn);
    JS_SetPropertyStr(ctx, view_obj, "resizeWithBrowserSize", view_resize_browser_fn);
    JS_SetPropertyStr(ctx, ax_ns, "view", view_obj);

    // sys object
    JSValue sys_obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, sys_obj, "os", JS_NewInt32(ctx, 0));
    JS_SetPropertyStr(ctx, sys_obj, "OS_IOS", JS_NewInt32(ctx, 1));
    JS_SetPropertyStr(ctx, ax_ns, "sys", sys_obj);

    // ResolutionPolicy object
    JSValue res_policy_obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, res_policy_obj, "FIXED_HEIGHT", JS_NewInt32(ctx, 1));
    JS_SetPropertyStr(ctx, res_policy_obj, "FIXED_WIDTH", JS_NewInt32(ctx, 2));
    JS_SetPropertyStr(ctx, ax_ns, "ResolutionPolicy", res_policy_obj);

    // game object
    JSValue game_obj    = JS_NewObject(ctx);
    JSValue game_run_fn = JS_NewCFunction(ctx, js_game_run, "run", 2);
    JS_SetPropertyStr(ctx, game_obj, "run", game_run_fn);
    JS_SetPropertyStr(ctx, ax_ns, "game", game_obj);

    // global object - already available but ensure it's accessible
    JS_SetPropertyStr(ctx, global, "global", JS_DupValue(ctx, global));

    // console object
    JSValue console_obj    = JS_NewObject(ctx);
    JSValue console_log_fn = JS_NewCFunction(ctx, js_console_log, "log", 1);
    JS_SetPropertyStr(ctx, console_obj, "log", console_log_fn);
    JS_SetPropertyStr(ctx, global, "console", console_obj);

    // Free the ax and global references. The objects remain reachable from global scope.
    JS_FreeValue(ctx, ax_ns);
    JS_FreeValue(ctx, global);
}

}  // namespace ax
