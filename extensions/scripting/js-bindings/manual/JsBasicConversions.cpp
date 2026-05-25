/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/JsBasicConversions.h"
#include <string>

namespace ax
{

JSClassID s_ax_Ref_class_id = 0;

static std::unordered_map<std::string, JSValue> s_class_prototypes;
static std::unordered_map<void*, JSValue> s_js_wrapper_map;

void register_class_prototype(JSContext* ctx, const char* typeName, JSValue proto)
{
    s_class_prototypes[typeName] = JS_DupValue(ctx, proto);
}

JSValue get_class_prototype(JSContext* ctx, const char* typeName)
{
    auto it = s_class_prototypes.find(typeName);
    if (it != s_class_prototypes.end())
    {
        return JS_DupValue(ctx, it->second);
    }
    return JS_UNDEFINED;
}

void register_js_wrapper(JSContext* ctx, void* ptr, JSValue val)
{
    s_js_wrapper_map[ptr] = val;
}

void unregister_js_wrapper(JSContext* ctx, void* ptr)
{
    s_js_wrapper_map.erase(ptr);
}

JSValue lookup_js_wrapper(JSContext* ctx, void* ptr)
{
    auto it = s_js_wrapper_map.find(ptr);
    if (it != s_js_wrapper_map.end())
    {
        return it->second;
    }
    return JS_UNDEFINED;
}

// Primitive Conversions
bool jsval_to_int32(JSContext* ctx, JSValue val, int32_t* out)
{
    return JS_ToInt32(ctx, out, val) >= 0;
}

bool jsval_to_uint32(JSContext* ctx, JSValue val, uint32_t* out)
{
    // QuickJS-NG has JS_ToUint32
    return JS_ToUint32(ctx, out, val) >= 0;
}

bool jsval_to_bool(JSContext* ctx, JSValue val, bool* out)
{
    *out = (bool)JS_ToBool(ctx, val);
    return true;
}

bool jsval_to_double(JSContext* ctx, JSValue val, double* out)
{
    return JS_ToFloat64(ctx, out, val) >= 0;
}

bool jsval_to_float(JSContext* ctx, JSValue val, float* out)
{
    double temp;
    if (JS_ToFloat64(ctx, &temp, val) < 0)
    {
        return false;
    }
    *out = (float)temp;
    return true;
}

bool jsval_to_std_string(JSContext* ctx, JSValue val, std::string* out)
{
    const char* str = JS_ToCString(ctx, val);
    if (!str)
    {
        return false;
    }
    *out = str;
    JS_FreeCString(ctx, str);
    return true;
}

JSValue int32_to_jsval(JSContext* ctx, int32_t val)
{
    return JS_NewInt32(ctx, val);
}

JSValue uint32_to_jsval(JSContext* ctx, uint32_t val)
{
    // QuickJS-NG has JS_NewUint32
    return JS_NewUint32(ctx, val);
}

JSValue bool_to_jsval(JSContext* ctx, bool val)
{
    return JS_NewBool(ctx, val);
}

JSValue double_to_jsval(JSContext* ctx, double val)
{
    return JS_NewFloat64(ctx, val);
}

JSValue float_to_jsval(JSContext* ctx, float val)
{
    return JS_NewFloat64(ctx, (double)val);
}

JSValue std_string_to_jsval(JSContext* ctx, std::string_view val)
{
    return JS_NewStringLen(ctx, val.data(), val.length());
}

// Math Conversions
bool jsval_to_vec2(JSContext* ctx, JSValue val, Vec2* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue x_val = JS_GetPropertyStr(ctx, val, "x");
    JSValue y_val = JS_GetPropertyStr(ctx, val, "y");
    double x = 0, y = 0;
    bool ok = JS_ToFloat64(ctx, &x, x_val) >= 0 && JS_ToFloat64(ctx, &y, y_val) >= 0;
    JS_FreeValue(ctx, x_val);
    JS_FreeValue(ctx, y_val);
    if (ok)
    {
        out->set((float)x, (float)y);
    }
    return ok;
}

JSValue vec2_to_jsval(JSContext* ctx, const Vec2& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, val.x));
    JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, val.y));
    return obj;
}

bool jsval_to_vec3(JSContext* ctx, JSValue val, Vec3* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue x_val = JS_GetPropertyStr(ctx, val, "x");
    JSValue y_val = JS_GetPropertyStr(ctx, val, "y");
    JSValue z_val = JS_GetPropertyStr(ctx, val, "z");
    double x = 0, y = 0, z = 0;
    bool ok = JS_ToFloat64(ctx, &x, x_val) >= 0 && 
              JS_ToFloat64(ctx, &y, y_val) >= 0 &&
              JS_ToFloat64(ctx, &z, z_val) >= 0;
    JS_FreeValue(ctx, x_val);
    JS_FreeValue(ctx, y_val);
    JS_FreeValue(ctx, z_val);
    if (ok)
    {
        out->set((float)x, (float)y, (float)z);
    }
    return ok;
}

JSValue vec3_to_jsval(JSContext* ctx, const Vec3& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, val.x));
    JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, val.y));
    JS_SetPropertyStr(ctx, obj, "z", JS_NewFloat64(ctx, val.z));
    return obj;
}

bool jsval_to_vec4(JSContext* ctx, JSValue val, Vec4* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue x_val = JS_GetPropertyStr(ctx, val, "x");
    JSValue y_val = JS_GetPropertyStr(ctx, val, "y");
    JSValue z_val = JS_GetPropertyStr(ctx, val, "z");
    JSValue w_val = JS_GetPropertyStr(ctx, val, "w");
    double x = 0, y = 0, z = 0, w = 0;
    bool ok = JS_ToFloat64(ctx, &x, x_val) >= 0 && 
              JS_ToFloat64(ctx, &y, y_val) >= 0 &&
              JS_ToFloat64(ctx, &z, z_val) >= 0 &&
              JS_ToFloat64(ctx, &w, w_val) >= 0;
    JS_FreeValue(ctx, x_val);
    JS_FreeValue(ctx, y_val);
    JS_FreeValue(ctx, z_val);
    JS_FreeValue(ctx, w_val);
    if (ok)
    {
        out->set((float)x, (float)y, (float)z, (float)w);
    }
    return ok;
}

JSValue vec4_to_jsval(JSContext* ctx, const Vec4& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, val.x));
    JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, val.y));
    JS_SetPropertyStr(ctx, obj, "z", JS_NewFloat64(ctx, val.z));
    JS_SetPropertyStr(ctx, obj, "w", JS_NewFloat64(ctx, val.w));
    return obj;
}

bool jsval_to_size(JSContext* ctx, JSValue val, Size* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue w_val = JS_GetPropertyStr(ctx, val, "width");
    JSValue h_val = JS_GetPropertyStr(ctx, val, "height");
    double w = 0, h = 0;
    bool ok = JS_ToFloat64(ctx, &w, w_val) >= 0 && JS_ToFloat64(ctx, &h, h_val) >= 0;
    JS_FreeValue(ctx, w_val);
    JS_FreeValue(ctx, h_val);
    if (ok)
    {
        out->setSize((float)w, (float)h);
    }
    return ok;
}

JSValue size_to_jsval(JSContext* ctx, const Size& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "width", JS_NewFloat64(ctx, val.width));
    JS_SetPropertyStr(ctx, obj, "height", JS_NewFloat64(ctx, val.height));
    return obj;
}

bool jsval_to_rect(JSContext* ctx, JSValue val, Rect* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue x_val = JS_GetPropertyStr(ctx, val, "x");
    JSValue y_val = JS_GetPropertyStr(ctx, val, "y");
    JSValue w_val = JS_GetPropertyStr(ctx, val, "width");
    JSValue h_val = JS_GetPropertyStr(ctx, val, "height");
    double x = 0, y = 0, w = 0, h = 0;
    bool ok = JS_ToFloat64(ctx, &x, x_val) >= 0 && 
              JS_ToFloat64(ctx, &y, y_val) >= 0 &&
              JS_ToFloat64(ctx, &w, w_val) >= 0 && 
              JS_ToFloat64(ctx, &h, h_val) >= 0;
    JS_FreeValue(ctx, x_val);
    JS_FreeValue(ctx, y_val);
    JS_FreeValue(ctx, w_val);
    JS_FreeValue(ctx, h_val);
    if (ok)
    {
        out->setRect((float)x, (float)y, (float)w, (float)h);
    }
    return ok;
}

JSValue rect_to_jsval(JSContext* ctx, const Rect& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "x", JS_NewFloat64(ctx, val.origin.x));
    JS_SetPropertyStr(ctx, obj, "y", JS_NewFloat64(ctx, val.origin.y));
    JS_SetPropertyStr(ctx, obj, "width", JS_NewFloat64(ctx, val.size.width));
    JS_SetPropertyStr(ctx, obj, "height", JS_NewFloat64(ctx, val.size.height));
    return obj;
}

bool jsval_to_color3b(JSContext* ctx, JSValue val, Color32* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue r_val = JS_GetPropertyStr(ctx, val, "r");
    JSValue g_val = JS_GetPropertyStr(ctx, val, "g");
    JSValue b_val = JS_GetPropertyStr(ctx, val, "b");
    int32_t r = 0, g = 0, b = 0;
    bool ok = JS_ToInt32(ctx, &r, r_val) >= 0 && 
              JS_ToInt32(ctx, &g, g_val) >= 0 &&
              JS_ToInt32(ctx, &b, b_val) >= 0;
    JS_FreeValue(ctx, r_val);
    JS_FreeValue(ctx, g_val);
    JS_FreeValue(ctx, b_val);
    if (ok)
    {
        out->r = (uint8_t)r;
        out->g = (uint8_t)g;
        out->b = (uint8_t)b;
        out->a = 255;
    }
    return ok;
}

JSValue color3b_to_jsval(JSContext* ctx, const Color32& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "r", JS_NewInt32(ctx, val.r));
    JS_SetPropertyStr(ctx, obj, "g", JS_NewInt32(ctx, val.g));
    JS_SetPropertyStr(ctx, obj, "b", JS_NewInt32(ctx, val.b));
    return obj;
}

bool jsval_to_color4b(JSContext* ctx, JSValue val, Color32* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue r_val = JS_GetPropertyStr(ctx, val, "r");
    JSValue g_val = JS_GetPropertyStr(ctx, val, "g");
    JSValue b_val = JS_GetPropertyStr(ctx, val, "b");
    JSValue a_val = JS_GetPropertyStr(ctx, val, "a");
    int32_t r = 0, g = 0, b = 0, a = 255;
    bool ok = JS_ToInt32(ctx, &r, r_val) >= 0 && 
              JS_ToInt32(ctx, &g, g_val) >= 0 &&
              JS_ToInt32(ctx, &b, b_val) >= 0;
    if (ok && !JS_IsUndefined(a_val))
    {
        ok &= JS_ToInt32(ctx, &a, a_val) >= 0;
    }
    JS_FreeValue(ctx, r_val);
    JS_FreeValue(ctx, g_val);
    JS_FreeValue(ctx, b_val);
    JS_FreeValue(ctx, a_val);
    if (ok)
    {
        out->r = (uint8_t)r;
        out->g = (uint8_t)g;
        out->b = (uint8_t)b;
        out->a = (uint8_t)a;
    }
    return ok;
}

JSValue color4b_to_jsval(JSContext* ctx, const Color32& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "r", JS_NewInt32(ctx, val.r));
    JS_SetPropertyStr(ctx, obj, "g", JS_NewInt32(ctx, val.g));
    JS_SetPropertyStr(ctx, obj, "b", JS_NewInt32(ctx, val.b));
    JS_SetPropertyStr(ctx, obj, "a", JS_NewInt32(ctx, val.a));
    return obj;
}

bool jsval_to_color4f(JSContext* ctx, JSValue val, Color* out)
{
    if (!JS_IsObject(val)) return false;
    JSValue r_val = JS_GetPropertyStr(ctx, val, "r");
    JSValue g_val = JS_GetPropertyStr(ctx, val, "g");
    JSValue b_val = JS_GetPropertyStr(ctx, val, "b");
    JSValue a_val = JS_GetPropertyStr(ctx, val, "a");
    double r = 0, g = 0, b = 0, a = 1.0;
    bool ok = JS_ToFloat64(ctx, &r, r_val) >= 0 && 
              JS_ToFloat64(ctx, &g, g_val) >= 0 &&
              JS_ToFloat64(ctx, &b, b_val) >= 0;
    if (ok && !JS_IsUndefined(a_val))
    {
        ok &= JS_ToFloat64(ctx, &a, a_val) >= 0;
    }
    JS_FreeValue(ctx, r_val);
    JS_FreeValue(ctx, g_val);
    JS_FreeValue(ctx, b_val);
    JS_FreeValue(ctx, a_val);
    if (ok)
    {
        out->r = (float)r;
        out->g = (float)g;
        out->b = (float)b;
        out->a = (float)a;
    }
    return ok;
}

JSValue color4f_to_jsval(JSContext* ctx, const Color& val)
{
    JSValue obj = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "r", JS_NewFloat64(ctx, val.r));
    JS_SetPropertyStr(ctx, obj, "g", JS_NewFloat64(ctx, val.g));
    JS_SetPropertyStr(ctx, obj, "b", JS_NewFloat64(ctx, val.b));
    JS_SetPropertyStr(ctx, obj, "a", JS_NewFloat64(ctx, val.a));
    return obj;
}

// Object conversion helper
JSValue object_to_jsval(JSContext* ctx, Ref* ptr, const char* typeName)
{
    if (!ptr)
    {
        return JS_NULL;
    }

    JSValue cached = lookup_js_wrapper(ctx, ptr);
    if (!JS_IsUndefined(cached))
    {
        return JS_DupValue(ctx, cached);
    }

    JSValue proto = get_class_prototype(ctx, typeName);
    if (JS_IsUndefined(proto))
    {
        proto = get_class_prototype(ctx, "ax::Ref");
    }

    JSValue obj = JS_NewObjectProtoClass(ctx, proto, s_ax_Ref_class_id);
    JS_SetOpaque(obj, ptr);

    ptr->retain();
    register_js_wrapper(ctx, ptr, obj);

    JS_FreeValue(ctx, proto);
    return obj;
}

JSValue object_to_jsval(JSContext* ctx, void* ptr, const char* typeName)
{
    if (!ptr)
    {
        return JS_NULL;
    }

    JSValue cached = lookup_js_wrapper(ctx, ptr);
    if (!JS_IsUndefined(cached))
    {
        return JS_DupValue(ctx, cached);
    }

    JSValue proto = get_class_prototype(ctx, typeName);
    
    // We use a different class ID or just a generic object for non-Ref types
    // For now, let's just use a plain object if it's a singleton like SpriteFrameCache
    JSValue obj = JS_NewObjectProtoClass(ctx, proto, 0); // 0 means plain object or use a specific ID if we had one
    JS_SetOpaque(obj, ptr);

    register_js_wrapper(ctx, ptr, obj);

    JS_FreeValue(ctx, proto);
    return obj;
}

} // namespace ax
