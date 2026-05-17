/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsBindings.h"

#include "axmol/2d/Sprite.h"
#include "axmol/scene/Scene.h"
#include "axmol/scene/Node.h"
#include "axmol/base/Director.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

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

    // Wrap the sprite pointer in a JS object
    JSValue obj = JS_NewObject(ctx);
    JS_SetOpaque(obj, sprite);
    return obj;
}

// Sprite.setPosition(x, y) binding
static JSValue js_Sprite_setPosition(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, NULL);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, NULL);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, NULL);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, NULL);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, NULL);
    if (!sprite)
        return JS_EXCEPTION;

    Texture2D* texture = sprite->getTexture();
    if (!texture)
        return JS_NULL;

    // Return object wrapper
    JSValue obj = JS_NewObject(ctx);
    JS_SetOpaque(obj, texture);
    return obj;
}

// Scene.addChild(node, zorder) binding
static JSValue js_Scene_addChild(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Scene* scene = (Scene*)JS_GetOpaque(this_val, NULL);
    if (!scene)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    Node* child = (Node*)JS_GetOpaque(argv[0], NULL);
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

    JSValue obj = JS_NewObject(ctx);
    JS_SetOpaque(obj, director);
    return obj;
}

// Director.getRunningScene() binding
static JSValue js_Director_getRunningScene(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = (Director*)JS_GetOpaque(this_val, NULL);
    if (!director)
        return JS_EXCEPTION;

    Scene* scene = director->getRunningScene();
    if (!scene)
        return JS_NULL;

    JSValue obj = JS_NewObject(ctx);
    JS_SetOpaque(obj, scene);
    return obj;
}

void js_register_all_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_NewObject(ctx);

    // Sprite class
    JSValue Sprite_class = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, Sprite_class, "create", JS_NewCFunction(ctx, js_Sprite_create, "create", 1));
    JS_SetPropertyStr(ctx, ax_ns, "Sprite", Sprite_class);

    // Sprite prototype methods
    JSValue sprite_proto = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, sprite_proto, "setPosition", JS_NewCFunction(ctx, js_Sprite_setPosition, "setPosition", 2));
    JS_SetPropertyStr(ctx, sprite_proto, "setScale", JS_NewCFunction(ctx, js_Sprite_setScale, "setScale", 1));
    JS_SetPropertyStr(ctx, sprite_proto, "setAnchorPoint", JS_NewCFunction(ctx, js_Sprite_setAnchorPoint, "setAnchorPoint", 2));
    JS_SetPropertyStr(ctx, sprite_proto, "setOpacity", JS_NewCFunction(ctx, js_Sprite_setOpacity, "setOpacity", 1));
    JS_SetPropertyStr(ctx, sprite_proto, "getTexture", JS_NewCFunction(ctx, js_Sprite_getTexture, "getTexture", 0));
    JS_FreeValue(ctx, sprite_proto);

    // Scene prototype methods  
    JSValue scene_proto = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, scene_proto, "addChild", JS_NewCFunction(ctx, js_Scene_addChild, "addChild", 2));
    JS_FreeValue(ctx, scene_proto);

    // Director class
    JSValue Director_class = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, Director_class, "getInstance", JS_NewCFunction(ctx, js_Director_getInstance, "getInstance", 0));
    JS_SetPropertyStr(ctx, ax_ns, "Director", Director_class);

    // Director prototype methods
    JSValue director_proto = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, director_proto, "getRunningScene", JS_NewCFunction(ctx, js_Director_getRunningScene, "getRunningScene", 0));
    JS_FreeValue(ctx, director_proto);

    // Register ax namespace in global
    JS_SetPropertyStr(ctx, global, "ax", ax_ns);

    // Do not free `ax_ns` here — freeing it can cause a crash on some
    // QuickJS builds (use-after-free when the global still references it).
    // Let the runtime cleanup handle it when the global object is freed.
    JS_FreeValue(ctx, global);
}

}  // namespace ax
