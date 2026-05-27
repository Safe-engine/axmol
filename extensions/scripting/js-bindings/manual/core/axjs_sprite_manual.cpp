/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_sprite_manual.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "axmol/2d/Sprite.h"
#include "axmol/2d/Action.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Forward declaration - defined in JsBindings.cpp
extern JSClassID s_ax_Ref_class_id;

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

    return object_to_jsval(ctx, sprite, "Sprite");
}

// Sprite.createWithSpriteFrameName(name) binding
static JSValue js_Sprite_createWithSpriteFrameName(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    if (argc < 1)
        return JS_FALSE;

    const char* name = JS_ToCString(ctx, argv[0]);
    if (!name)
        return JS_EXCEPTION;

    Sprite* sprite = Sprite::createWithSpriteFrameName(name);
    JS_FreeCString(ctx, name);

    if (!sprite)
        return JS_NULL;

    return object_to_jsval(ctx, sprite, "Sprite");
}

// Sprite.setPosition(x, y) binding
static JSValue js_Sprite_setPosition(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
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
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!sprite)
        return JS_EXCEPTION;

    Texture2D* texture = sprite->getTexture();
    if (!texture)
        return JS_NULL;

    return object_to_jsval(ctx, texture, "Texture2D");
}

// Sprite.runAction(action) binding
static JSValue js_Sprite_runAction(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Sprite* sprite = (Sprite*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!sprite)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    Action* action = nullptr;
    if (!jsval_to_object(ctx, argv[0], &action))
        return JS_EXCEPTION;

    sprite->runAction(action);
    return JS_UNDEFINED;
}

void js_register_sprite_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    // JSValue ax_ns = JS_GetPropertyStr(ctx, global, "ax");

    // Sprite class - static methods
    // JSValue Sprite_class = JS_NewObject(ctx);
    // JSValue sprite_create_fn = JS_NewCFunction(ctx, js_Sprite_create, "create", 1);
    // JSValue sprite_create_frame_fn = JS_NewCFunction(ctx, js_Sprite_createWithSpriteFrameName, "createWithSpriteFrameName", 1);
    // JS_SetPropertyStr(ctx, Sprite_class, "create", sprite_create_fn);
    // JS_SetPropertyStr(ctx, Sprite_class, "createWithSpriteFrameName", sprite_create_frame_fn);
    // JS_SetPropertyStr(ctx, ax_ns, "Sprite", Sprite_class);

    // Sprite prototype methods
    // JSValue sprite_proto = JS_NewObject(ctx);
    // JSValue sprite_set_position_fn = JS_NewCFunction(ctx, js_Sprite_setPosition, "setPosition", 2);
    // JSValue sprite_set_scale_fn = JS_NewCFunction(ctx, js_Sprite_setScale, "setScale", 1);
    // JSValue sprite_set_anchor_fn = JS_NewCFunction(ctx, js_Sprite_setAnchorPoint, "setAnchorPoint", 2);
    // JSValue sprite_set_opacity_fn = JS_NewCFunction(ctx, js_Sprite_setOpacity, "setOpacity", 1);
    // JSValue sprite_get_texture_fn = JS_NewCFunction(ctx, js_Sprite_getTexture, "getTexture", 0);
    // JSValue sprite_run_action_fn = JS_NewCFunction(ctx, js_Sprite_runAction, "runAction", 1);

    // JS_SetPropertyStr(ctx, sprite_proto, "setPosition", sprite_set_position_fn);
    // JS_SetPropertyStr(ctx, sprite_proto, "setScale", sprite_set_scale_fn);
    // JS_SetPropertyStr(ctx, sprite_proto, "setAnchorPoint", sprite_set_anchor_fn);
    // JS_SetPropertyStr(ctx, sprite_proto, "setOpacity", sprite_set_opacity_fn);
    // JS_SetPropertyStr(ctx, sprite_proto, "getTexture", sprite_get_texture_fn);
    // JS_SetPropertyStr(ctx, sprite_proto, "runAction", sprite_run_action_fn);

    // register_class_prototype(ctx, "Sprite", JS_DupValue(ctx, sprite_proto));

    // JS_FreeValue(ctx, sprite_proto);
    // JS_FreeValue(ctx, ax_ns);
    // JS_FreeValue(ctx, global);
}

}  // namespace ax
