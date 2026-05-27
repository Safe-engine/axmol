/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_director_manual.h"
#include "js-bindings/manual/JsBasicConversions.h"
#include "axmol/base/Director.h"
#include "axmol/scene/Scene.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

// Forward declaration - defined in JsBindings.cpp
extern JSClassID s_ax_Ref_class_id;

// Director.getInstance() binding
static JSValue js_Director_getInstance(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = Director::getInstance();
    if (!director)
        return JS_NULL;

    return object_to_jsval(ctx, director, "Director");
}

// Director.getRunningScene() binding
static JSValue js_Director_getRunningScene(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = (Director*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!director)
        return JS_EXCEPTION;

    Scene* scene = director->getRunningScene();
    if (!scene)
        return JS_NULL;

    return object_to_jsval(ctx, scene, "Scene");
}

// Director.runScene(scene) binding
static JSValue js_Director_runScene(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
{
    Director* director = (Director*)JS_GetOpaque(this_val, s_ax_Ref_class_id);
    if (!director)
        return JS_EXCEPTION;

    if (argc < 1)
        return JS_FALSE;

    Scene* scene = nullptr;
    if (!jsval_to_object(ctx, argv[0], &scene))
        return JS_EXCEPTION;

    director->replaceScene(scene);
    return JS_UNDEFINED;
}

void js_register_director_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue ax_ns = JS_GetPropertyStr(ctx, global, "ax");

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

    register_class_prototype(ctx, "Director", JS_DupValue(ctx, director_proto));

    // Director instance exported to global
    JSValue director_instance = JS_NewObjectClass(ctx, s_ax_Ref_class_id);
    JS_SetOpaque(director_instance, Director::getInstance());
    JS_SetPropertyStr(ctx, global, "director", director_instance);

    JS_FreeValue(ctx, director_proto);
    JS_FreeValue(ctx, ax_ns);
    JS_FreeValue(ctx, global);
}

}  // namespace ax
