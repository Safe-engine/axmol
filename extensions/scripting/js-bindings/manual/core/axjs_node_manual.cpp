/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_node_manual.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "axmol/scene/Node.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Forward declaration - defined in JsBindings.cpp
extern JSClassID s_ax_Ref_class_id;

// Node.addChild(child, zorder) binding
static JSValue js_Node_addChild(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Node* node = (Node*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!node)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    Node* child = nullptr;
    if (!jsval_to_object(ctx, argv[0], &child))
        return JS_EXCEPTION;

    int zorder = 0;
    if (argc > 1)
    {
        if (JS_ToInt32(ctx, &zorder, argv[1]) < 0)
            return JS_EXCEPTION;
    }

    node->addChild(child, zorder);
    return JS_UNDEFINED;
}

// Node.setPosition(x, y) binding
static JSValue js_Node_setPosition(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Node* node = (Node*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!node)
        return JS_EXCEPTION;

    if (argc < 2)
        return JS_FALSE;

    double x, y;
    if (JS_ToFloat64(ctx, &x, argv[0]) < 0 || JS_ToFloat64(ctx, &y, argv[1]) < 0)
        return JS_EXCEPTION;

    node->setPosition(x, y);
    return JS_UNDEFINED;
}

// Node.setScale(scale) binding
static JSValue js_Node_setScale(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Node* node = (Node*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!node)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    double scale;
    if (JS_ToFloat64(ctx, &scale, argv[0]) < 0)
        return JS_EXCEPTION;

    node->setScale(scale);
    return JS_UNDEFINED;
}

void js_register_node_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_GetPropertyStr(ctx, global, "ax");

    // Node prototype methods
    JSValue node_proto = JS_NewObject(ctx);
    JSValue node_add_child_fn = JS_NewCFunction(ctx, js_Node_addChild, "addChild", 2);
    JSValue node_set_position_fn = JS_NewCFunction(ctx, js_Node_setPosition, "setPosition", 2);
    JSValue node_set_scale_fn = JS_NewCFunction(ctx, js_Node_setScale, "setScale", 1);

    JS_SetPropertyStr(ctx, node_proto, "addChild", node_add_child_fn);
    JS_SetPropertyStr(ctx, node_proto, "setPosition", node_set_position_fn);
    JS_SetPropertyStr(ctx, node_proto, "setScale", node_set_scale_fn);

    register_class_prototype(ctx, "Node", JS_DupValue(ctx, node_proto));

    JS_FreeValue(ctx, node_proto);
    JS_FreeValue(ctx, ax_ns);
    JS_FreeValue(ctx, global);
}

}  // namespace ax
