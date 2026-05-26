/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_core_manual.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "axmol/2d/Sprite.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Sprite constructor binding to support: new Sprite() or new Sprite(filename)
static JSValue js_Sprite_constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
    Sprite* sprite = nullptr;
    if (argc == 0)
    {
        sprite = Sprite::create();
    }
    else if (argc > 0)
    {
        if (JS_IsString(argv[0]))
        {
            const char* filename = JS_ToCString(ctx, argv[0]);
            if (filename)
            {
                sprite = Sprite::create(filename);
                JS_FreeCString(ctx, filename);
            }
        }
        else
        {
            sprite = Sprite::create();
        }
    }

    if (!sprite)
        return JS_NULL;

    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        return JS_EXCEPTION;

    JSValue obj = JS_NewObjectProtoClass(ctx, proto, s_ax_Ref_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        return JS_EXCEPTION;

    JS_SetOpaque(obj, sprite);
    
    // Retain and register wrapper
    sprite->retain();
    register_js_wrapper(ctx, sprite, obj);

    return obj;
}

void js_register_core_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_GetPropertyStr(ctx, global, "ax");
    
    if (JS_IsUndefined(ax_ns) || JS_IsNull(ax_ns))
    {
        JS_FreeValue(ctx, global);
        return;
    }

    // Get existing Sprite object to copy static methods
    JSValue old_sprite = JS_GetPropertyStr(ctx, ax_ns, "Sprite");
    
    // Get Sprite prototype
    JSValue sprite_proto = get_class_prototype(ctx, "Sprite");
    if (JS_IsUndefined(sprite_proto))
    {
        JS_FreeValue(ctx, old_sprite);
        JS_FreeValue(ctx, ax_ns);
        JS_FreeValue(ctx, global);
        return;
    }

    // Create the constructor function
    JSValue sprite_ctor = JS_NewCFunction2(ctx, js_Sprite_constructor, "Sprite", 0, JS_CFUNC_constructor, 0);
    
    // Associate prototype
    JS_SetConstructor(ctx, sprite_ctor, sprite_proto);
    
    // Copy static methods from the old Sprite object (like create, createWithSpriteFrameName)
    if (JS_IsObject(old_sprite))
    {
        JSPropertyEnum* ptab = nullptr;
        uint32_t len = 0;
        if (JS_GetOwnPropertyNames(ctx, &ptab, &len, old_sprite, JS_GPN_STRING_MASK) >= 0)
        {
            for (uint32_t i = 0; i < len; ++i)
            {
                JSAtom atom = ptab[i].atom;
                JSValue val = JS_GetProperty(ctx, old_sprite, atom);
                JS_SetProperty(ctx, sprite_ctor, atom, val);
            }
            js_free(ctx, ptab);
        }
    }
    
    // Register the new constructor in ax namespace
    JS_SetPropertyStr(ctx, ax_ns, "Sprite", JS_DupValue(ctx, sprite_ctor));
    
    // Register the constructor globally for syntax convenience: new Sprite()
    JS_SetPropertyStr(ctx, global, "Sprite", sprite_ctor);

    JS_FreeValue(ctx, sprite_proto);
    JS_FreeValue(ctx, old_sprite);
    JS_FreeValue(ctx, ax_ns);
    JS_FreeValue(ctx, global);
}

} // namespace ax
