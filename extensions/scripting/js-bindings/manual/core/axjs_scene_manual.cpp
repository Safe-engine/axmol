/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_scene_manual.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "axmol/scene/Scene.h"
#include "axmol/scene/Node.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Forward declaration - defined in JsBindings.cpp
extern JSClassID s_ax_Ref_class_id;

// Scene constructor
static JSValue js_Scene_constructor(JSContext* ctx, JSValueConst new_target, int argc, JSValueConst* argv)
{
    Scene* scene = Scene::create();
    if (!scene)
        return JS_NULL;

    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto))
        return JS_EXCEPTION;

    JSValue obj = JS_NewObjectProtoClass(ctx, proto, s_ax_Ref_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        return JS_EXCEPTION;

    JS_SetOpaque(obj, scene);
    return obj;
}

// Scene.addChild(node, zorder) binding
static JSValue js_Scene_addChild(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Scene* scene = (Scene*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!scene)
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

    scene->addChild(child, zorder);
    return JS_UNDEFINED;
}

// Scene.onEnter() - to be called by native code
static JSValue js_Scene_onEnter(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Scene* scene = (Scene*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!scene)
        return JS_EXCEPTION;

    scene->onEnter();
    return JS_UNDEFINED;
}

void js_register_scene_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_GetPropertyStr(ctx, global, "ax");

    // Scene prototype methods
    JSValue scene_proto = JS_NewObject(ctx);
    JSValue scene_add_child_fn = JS_NewCFunction(ctx, js_Scene_addChild, "addChild", 2);
    JSValue scene_on_enter_fn = JS_NewCFunction(ctx, js_Scene_onEnter, "onEnter", 0);
    JS_SetPropertyStr(ctx, scene_proto, "addChild", scene_add_child_fn);
    JS_SetPropertyStr(ctx, scene_proto, "onEnter", scene_on_enter_fn);

    register_class_prototype(ctx, "Scene", JS_DupValue(ctx, scene_proto));

    // Scene constructor
    JSValue scene_constructor_fn = JS_NewCFunction2(ctx, js_Scene_constructor, "Scene", 0, JS_CFUNC_constructor, 0);
    JS_SetConstructor(ctx, scene_constructor_fn, scene_proto);
    JS_SetPropertyStr(ctx, ax_ns, "Scene", scene_constructor_fn);

    JS_FreeValue(ctx, scene_proto);
    JS_FreeValue(ctx, ax_ns);
    JS_FreeValue(ctx, global);
}

}  // namespace ax
