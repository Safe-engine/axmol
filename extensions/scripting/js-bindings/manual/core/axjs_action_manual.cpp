/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_action_manual.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "axmol/2d/ActionInterval.h"
#include "axmol/math/Vec2.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Forward declaration - defined in JsBindings.cpp
extern JSClassID s_ax_Ref_class_id;
extern JSClassID s_actionClassID;

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

    return object_to_jsval(ctx, action, "MoveBy");
}

void js_register_action_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_GetPropertyStr(ctx, global, "ax");
    // Global functions - exported directly to global scope
    JSValue p_fn = JS_NewCFunction(ctx, js_p, "p", 2);
    JS_SetPropertyStr(ctx, ax_ns, "p", p_fn);

    JSValue moveBy_fn = JS_NewCFunction(ctx, js_moveBy, "moveBy", 2);
    JS_SetPropertyStr(ctx, ax_ns, "moveBy", moveBy_fn);

    JS_FreeValue(ctx, ax_ns);
    JS_FreeValue(ctx, global);
}

}  // namespace ax
