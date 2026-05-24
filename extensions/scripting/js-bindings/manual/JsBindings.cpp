/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsBindings.h"

#include "axmol/2d/Sprite.h"
#include "axmol/scene/Scene.h"
#include "axmol/scene/Node.h"
#include "axmol/base/Director.h"
#include "axmol/2d/Action.h"
#include "axmol/2d/ActionInterval.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

static JSClassID s_spriteClassID;
static JSClassID s_sceneClassID;
static JSClassID s_directorClassID;
static JSClassID s_textureClassID;
static JSClassID s_actionClassID;
static JSClassID s_vec2ClassID;
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
    s_jsClassIDsInitialized = true;
}

// Sprite.create(filename) binding
static JSValue js_Sprite_create(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    const char* filename = nullptr;
    if (argc > 0)
    {
        filename = JS_ToCString(ctx, argv[0]);
        if (!filename)
            return JS_EXCEPTION;
    }

    Sprite* sprite = filename ? Sprite::create(filename) : Sprite::create();

    if (filename)
        JS_FreeCString(ctx, filename);

    if (!sprite)
        return JS_NULL;

    // Wrap the sprite pointer in a JS object with a proper prototype.
    JSValue obj = JS_NewObjectClass(ctx, s_spriteClassID);
    JS_SetOpaque(obj, sprite);
    return obj;
}

// Sprite.setPosition(x, y) binding
static JSValue js_Sprite_setPosition(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_spriteClassID);
    if (!sprite)
        return JS_EXCEPTION;

    if (argc < 2)
        return JS_FALSE;

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]) < 0 || JS_ToFloat64(ctx, &y, argv[1]) < 0)
        return JS_EXCEPTION;

    sprite->setPosition(x, y);
    return JS_UNDEFINED;
}

// Sprite.setScale(scale) binding
static JSValue js_Sprite_setScale(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_spriteClassID);
    if (!sprite)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    double scale;
    if (JS_ToFloat64(ctx, &scale, argv[0]) < 0)
        return JS_EXCEPTION;

    sprite->setScale(scale);
    return JS_UNDEFINED;
}

// Sprite.setAnchorPoint(x, y) binding
static JSValue js_Sprite_setAnchorPoint(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_spriteClassID);
    if (!sprite)
        return JS_EXCEPTION;

    if (argc < 2)
        return JS_FALSE;

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]) < 0 || JS_ToFloat64(ctx, &y, argv[1]) < 0)
        return JS_EXCEPTION;

    sprite->setAnchorPoint(Vec2(x, y));
    return JS_UNDEFINED;
}

// Sprite.setOpacity(opacity) binding
static JSValue js_Sprite_setOpacity(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_spriteClassID);
    if (!sprite)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    int32_t opacity;
    if (JS_ToInt32(ctx, &opacity, argv[0]) < 0)
        return JS_EXCEPTION;

    sprite->setOpacity(opacity);
    return JS_UNDEFINED;
}

// Sprite.getTexture() binding
static JSValue js_Sprite_getTexture(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_spriteClassID);
    if (!sprite)
        return JS_EXCEPTION;

    Texture2D* texture = sprite->getTexture();
    if (!texture)
        return JS_NULL;

    // Return object wrapper
    JSValue obj = JS_NewObjectClass(ctx, s_textureClassID);
    JS_SetOpaque(obj, texture);
    return obj;
}

// Scene.addChild(node, zorder) binding
static JSValue js_Scene_addChild(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Scene* scene = (Scene*)JS_GetOpaque(this_val, s_sceneClassID);
    if (!scene)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    JSClassID class_id;
    Node* child = (Node*)JS_GetAnyOpaque(argv[0], &class_id);
    if (!child)
        return JS_EXCEPTION;

    int zorder = 0;
    if (argc > 1)
    {
        if (JS_ToInt32(ctx, &zorder, argv[1]) < 0)
            return JS_EXCEPTION;
    }

    scene->addChild(child, zorder);
    return JS_UNDEFINED;
}

// Director.getInstance() binding
static JSValue js_Director_getInstance(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = Director::getInstance();
    if (!director)
        return JS_NULL;

    JSValue obj = JS_NewObjectClass(ctx, s_directorClassID);
    JS_SetOpaque(obj, director);
    return obj;
}

// Director.getRunningScene() binding
static JSValue js_Director_getRunningScene(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = (Director*)JS_GetOpaque(this_val, s_directorClassID);
    if (!director)
        return JS_EXCEPTION;

    Scene* scene = director->getRunningScene();
    if (!scene)
        return JS_NULL;

    JSValue obj = JS_NewObjectClass(ctx, s_sceneClassID);
    JS_SetOpaque(obj, scene);
    return obj;
}

// Director.runScene(scene) binding
static JSValue js_Director_runScene(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = (Director*)JS_GetOpaque(this_val, s_directorClassID);
    if (!director)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    JSClassID class_id;
    Scene* scene = (Scene*)JS_GetAnyOpaque(argv[0], &class_id);
    if (!scene)
        return JS_EXCEPTION;

    director->replaceScene(scene);
    return JS_UNDEFINED;
}

// Scene constructor
static JSValue js_Scene_constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
    Scene* scene = Scene::create();
    if (!scene)
        return JS_NULL;

    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        return JS_EXCEPTION;

    JSValue obj = JS_NewObjectProtoClass(ctx, proto, s_sceneClassID);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        return JS_EXCEPTION;

    JS_SetOpaque(obj, scene);
    return obj;
}

// Scene.onEnter() - to be called by native code
static JSValue js_Scene_onEnter(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Scene* scene = (Scene*)JS_GetOpaque(this_val, s_sceneClassID);
    if (!scene)
        return JS_EXCEPTION;

    scene->onEnter();
    return JS_UNDEFINED;
}

// Sprite.runAction(action) binding
static JSValue js_Sprite_runAction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_spriteClassID);
    if (!sprite)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    JSClassID class_id;
    Action* action = (Action*)JS_GetAnyOpaque(argv[0], &class_id);
    if (!action)
        return JS_EXCEPTION;

    sprite->runAction(action);
    return JS_UNDEFINED;
}

// p(x, y) - create a Vec2
static JSValue js_p(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    if (argc < 2)
        return JS_FALSE;

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]) < 0 || JS_ToFloat64(ctx, &y, argv[1]) < 0)
        return JS_EXCEPTION;

    // Create a JS object to represent Vec2
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, x));
    JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, y));

    return obj;
}

// moveBy(duration, offset) - create a MoveBy action
static JSValue js_moveBy(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    if (argc < 2)
        return JS_FALSE;

    double duration;
    if (JS_ToFloat64(ctx, &duration, argv[0]) < 0)
        return JS_EXCEPTION;

    // Get offset object (Vec2)
    JSValue offset_val = argv[1];
    double offset_x = 0, offset_y = 0;

    // Try to get x, y properties
    JSValue x_val = JS_GetPropertyStr(ctx, offset_val, "x");
    JSValue y_val = JS_GetPropertyStr(ctx, offset_val, "y");

    if (JS_IsNumber(x_val))
        JS_ToFloat64(ctx, &offset_x, x_val);
    if (JS_IsNumber(y_val))
        JS_ToFloat64(ctx, &offset_y, y_val);

    JS_FreeValue(ctx, x_val);
    JS_FreeValue(ctx, y_val);

    MoveBy* action = MoveBy::create(duration, Vec2(offset_x, offset_y));
    if (!action)
        return JS_NULL;

    JSValue moveByObj = JS_NewObjectClass(ctx, s_actionClassID);
    JS_SetOpaque(moveByObj, action);
    return moveByObj;
}

// View stubs
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

// sys object
static JSValue js_sys_get_os(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    return JS_NewInt32(ctx, 0);
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


void js_register_all_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_NewObject(ctx);

    JSRuntime* rt = JS_GetRuntime(ctx);
    ensureJsClassIDs(rt);

    JSClassDef spriteClassDef = {"AxSprite", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_spriteClassID, &spriteClassDef);
    JSClassDef sceneClassDef = {"AxScene", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_sceneClassID, &sceneClassDef);
    JSClassDef directorClassDef = {"AxDirector", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_directorClassID, &directorClassDef);
    JSClassDef textureClassDef = {"AxTexture", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_textureClassID, &textureClassDef);
    JSClassDef actionClassDef = {"AxAction", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_actionClassID, &actionClassDef);
    JSClassDef vec2ClassDef = {"AxVec2", nullptr, nullptr, nullptr, nullptr};
    JS_NewClass(rt, s_vec2ClassID, &vec2ClassDef);

    // Sprite class
    JSValue Sprite_class = JS_NewObject(ctx);
    JSValue sprite_create_fn = JS_NewCFunction(ctx, js_Sprite_create, "create", 1);
    JS_SetPropertyStr(ctx, Sprite_class, "create", sprite_create_fn);
    JS_SetPropertyStr(ctx, ax_ns, "Sprite", Sprite_class);

    // Sprite prototype methods
    JSValue sprite_proto = JS_NewObject(ctx);
    JSValue sprite_set_position_fn = JS_NewCFunction(ctx, js_Sprite_setPosition, "setPosition", 2);
    JSValue sprite_set_scale_fn = JS_NewCFunction(ctx, js_Sprite_setScale, "setScale", 1);
    JSValue sprite_set_anchor_fn = JS_NewCFunction(ctx, js_Sprite_setAnchorPoint, "setAnchorPoint", 2);
    JSValue sprite_set_opacity_fn = JS_NewCFunction(ctx, js_Sprite_setOpacity, "setOpacity", 1);
    JSValue sprite_get_texture_fn = JS_NewCFunction(ctx, js_Sprite_getTexture, "getTexture", 0);
    JSValue sprite_run_action_fn = JS_NewCFunction(ctx, js_Sprite_runAction, "runAction", 1);
    JS_SetPropertyStr(ctx, sprite_proto, "setPosition", sprite_set_position_fn);
    JS_SetPropertyStr(ctx, sprite_proto, "setScale", sprite_set_scale_fn);
    JS_SetPropertyStr(ctx, sprite_proto, "setAnchorPoint", sprite_set_anchor_fn);
    JS_SetPropertyStr(ctx, sprite_proto, "setOpacity", sprite_set_opacity_fn);
    JS_SetPropertyStr(ctx, sprite_proto, "getTexture", sprite_get_texture_fn);
    JS_SetPropertyStr(ctx, sprite_proto, "runAction", sprite_run_action_fn);
    JS_SetClassProto(ctx, s_spriteClassID, sprite_proto);

    // Scene prototype methods
    JSValue scene_proto = JS_NewObject(ctx);
    JSValue scene_add_child_fn = JS_NewCFunction(ctx, js_Scene_addChild, "addChild", 2);
    JSValue scene_on_enter_fn = JS_NewCFunction(ctx, js_Scene_onEnter, "onEnter", 0);
    JS_SetPropertyStr(ctx, scene_proto, "addChild", scene_add_child_fn);
    JS_SetPropertyStr(ctx, scene_proto, "onEnter", scene_on_enter_fn);

    // Scene constructor
    JSValue scene_constructor_fn = JS_NewCFunction2(ctx, js_Scene_constructor, "Scene", 0, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, scene_constructor_fn, scene_proto);
    JS_SetPropertyStr(ctx, ax_ns, "Scene", scene_constructor_fn);
    JS_SetClassProto(ctx, s_sceneClassID, scene_proto);

    // Director class
    JSValue Director_class = JS_NewObject(ctx);
    JSValue director_get_instance_fn = JS_NewCFunction(ctx, js_Director_getInstance, "getInstance", 0);
    JS_SetPropertyStr(ctx, Director_class, "getInstance", director_get_instance_fn);
    JS_SetPropertyStr(ctx, ax_ns, "Director", Director_class);

    // Director prototype methods
    JSValue director_proto = JS_NewObject(ctx);
    JSValue director_get_running_scene_fn = JS_NewCFunction(ctx, js_Director_getRunningScene, "getRunningScene", 0);
    JSValue director_run_scene_fn = JS_NewCFunction(ctx, js_Director_runScene, "runScene", 1);
    JS_SetPropertyStr(ctx, director_proto, "getRunningScene", director_get_running_scene_fn);
    JS_SetPropertyStr(ctx, director_proto, "runScene", director_run_scene_fn);
    JS_SetClassProto(ctx, s_directorClassID, director_proto);

    // Global functions - exported directly to global scope
    JSValue p_fn = JS_NewCFunction(ctx, js_p, "p", 2);
    JS_SetPropertyStr(ctx, global, "p", p_fn);

    JSValue moveBy_fn = JS_NewCFunction(ctx, js_moveBy, "moveBy", 2);
    JS_SetPropertyStr(ctx, global, "moveBy", moveBy_fn);

    // Director instance exported to global
    JSValue director_instance = JS_NewObjectClass(ctx, s_directorClassID);
    JS_SetOpaque(director_instance, Director::getInstance());
    JS_SetPropertyStr(ctx, global, "director", director_instance);

    // View object
    JSValue view_obj = JS_NewObject(ctx);
    JSValue view_get_design_res_fn = JS_NewCFunction(ctx, js_view_getDesignResolutionSize, "getDesignResolutionSize", 0);
    JSValue view_enable_retina_fn = JS_NewCFunction(ctx, js_view_enableRetina, "enableRetina", 1);
    JSValue view_adjust_viewport_fn = JS_NewCFunction(ctx, js_view_adjustViewPort, "adjustViewPort", 1);
    JSValue view_set_design_res_fn = JS_NewCFunction(ctx, js_view_setDesignResolutionSize, "setDesignResolutionSize", 3);
    JSValue view_resize_browser_fn = JS_NewCFunction(ctx, js_view_resizeWithBrowserSize, "resizeWithBrowserSize", 1);
    JS_SetPropertyStr(ctx, view_obj, "getDesignResolutionSize", view_get_design_res_fn);
    JS_SetPropertyStr(ctx, view_obj, "enableRetina", view_enable_retina_fn);
    JS_SetPropertyStr(ctx, view_obj, "adjustViewPort", view_adjust_viewport_fn);
    JS_SetPropertyStr(ctx, view_obj, "setDesignResolutionSize", view_set_design_res_fn);
    JS_SetPropertyStr(ctx, view_obj, "resizeWithBrowserSize", view_resize_browser_fn);
    JS_SetPropertyStr(ctx, global, "view", view_obj);

    // sys object
    JSValue sys_obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, sys_obj, "os", JS_NewInt32(ctx, 0));
    JS_SetPropertyStr(ctx, sys_obj, "OS_IOS", JS_NewInt32(ctx, 1));
    JS_SetPropertyStr(ctx, global, "sys", sys_obj);

    // ResolutionPolicy object
    JSValue res_policy_obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, res_policy_obj, "FIXED_HEIGHT", JS_NewInt32(ctx, 1));
    JS_SetPropertyStr(ctx, res_policy_obj, "FIXED_WIDTH", JS_NewInt32(ctx, 2));
    JS_SetPropertyStr(ctx, global, "ResolutionPolicy", res_policy_obj);

    // game object
    JSValue game_obj = JS_NewObject(ctx);
    JSValue game_run_fn = JS_NewCFunction(ctx, js_game_run, "run", 2);
    JS_SetPropertyStr(ctx, game_obj, "run", game_run_fn);
    JS_SetPropertyStr(ctx, global, "game", game_obj);

    // global object - already available but ensure it's accessible
    JS_SetPropertyStr(ctx, global, "global", JS_DupValue(ctx, global));

    // Register ax namespace in global
    JS_SetPropertyStr(ctx, global, "ax", ax_ns);

    // Free the global reference only. Other values are retained by the JS engine.
    JS_FreeValue(ctx, global);
}

}  // namespace ax
