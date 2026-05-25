/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

extern "C" {
#include "quickjs.h"
}

#include "axmol/base/Object.h"
#include "axmol/math/Vec2.h"
#include "axmol/math/Vec3.h"
#include "axmol/math/Vec4.h"
#include "axmol/math/Rect.h"
#include "axmol/math/Color.h"
#include "js-bindings/manual/Js-BindingsExport.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ax
{

using Ref = Object;
using Color3B = Color32;
using Color4B = Color32;
using Color4F = Color;

// Global class ID for all Ref objects
AX_JS_DLL extern JSClassID s_ax_Ref_class_id;

// Prototype registry functions
AX_JS_DLL void register_class_prototype(JSContext* ctx, const char* typeName, JSValue proto);
AX_JS_DLL JSValue get_class_prototype(JSContext* ctx, const char* typeName);

// Association map functions
AX_JS_DLL void register_js_wrapper(JSContext* ctx, void* ptr, JSValue val);
AX_JS_DLL void unregister_js_wrapper(JSContext* ctx, void* ptr);
AX_JS_DLL JSValue lookup_js_wrapper(JSContext* ctx, void* ptr);

// Conversion functions: JSValue -> C++ primitive types
AX_JS_DLL bool jsval_to_int32(JSContext* ctx, JSValue val, int32_t* out);
AX_JS_DLL bool jsval_to_uint32(JSContext* ctx, JSValue val, uint32_t* out);
AX_JS_DLL bool jsval_to_bool(JSContext* ctx, JSValue val, bool* out);
AX_JS_DLL bool jsval_to_double(JSContext* ctx, JSValue val, double* out);
AX_JS_DLL bool jsval_to_float(JSContext* ctx, JSValue val, float* out);
AX_JS_DLL bool jsval_to_std_string(JSContext* ctx, JSValue val, std::string* out);

// Conversion functions: C++ primitive types -> JSValue
AX_JS_DLL JSValue int32_to_jsval(JSContext* ctx, int32_t val);
AX_JS_DLL JSValue uint32_to_jsval(JSContext* ctx, uint32_t val);
AX_JS_DLL JSValue bool_to_jsval(JSContext* ctx, bool val);
AX_JS_DLL JSValue double_to_jsval(JSContext* ctx, double val);
AX_JS_DLL JSValue float_to_jsval(JSContext* ctx, float val);
AX_JS_DLL JSValue std_string_to_jsval(JSContext* ctx, std::string_view val);

// Conversion functions: JSValue -> Axmol Math types
AX_JS_DLL bool jsval_to_vec2(JSContext* ctx, JSValue val, Vec2* out);
AX_JS_DLL bool jsval_to_vec3(JSContext* ctx, JSValue val, Vec3* out);
AX_JS_DLL bool jsval_to_vec4(JSContext* ctx, JSValue val, Vec4* out);
AX_JS_DLL bool jsval_to_size(JSContext* ctx, JSValue val, Size* out);
AX_JS_DLL bool jsval_to_rect(JSContext* ctx, JSValue val, Rect* out);
AX_JS_DLL bool jsval_to_color3b(JSContext* ctx, JSValue val, Color32* out);
AX_JS_DLL bool jsval_to_color4b(JSContext* ctx, JSValue val, Color32* out);
AX_JS_DLL bool jsval_to_color4f(JSContext* ctx, JSValue val, Color* out);

// Conversion functions: Axmol Math types -> JSValue
AX_JS_DLL JSValue vec2_to_jsval(JSContext* ctx, const Vec2& val);
AX_JS_DLL JSValue vec3_to_jsval(JSContext* ctx, const Vec3& val);
AX_JS_DLL JSValue vec4_to_jsval(JSContext* ctx, const Vec4& val);
AX_JS_DLL JSValue size_to_jsval(JSContext* ctx, const Size& val);
AX_JS_DLL JSValue rect_to_jsval(JSContext* ctx, const Rect& val);
AX_JS_DLL JSValue color3b_to_jsval(JSContext* ctx, const Color32& val);
AX_JS_DLL JSValue color4b_to_jsval(JSContext* ctx, const Color32& val);
AX_JS_DLL JSValue color4f_to_jsval(JSContext* ctx, const Color& val);

// Template conversion helper: JSValue -> C++ Object*
template <typename T>
bool jsval_to_object(JSContext* ctx, JSValue val, T** out)
{
    if (JS_IsNull(val) || JS_IsUndefined(val))
    {
        *out = nullptr;
        return true;
    }
    if (!JS_IsObject(val))
    {
        return false;
    }
    
    Ref* ref = (Ref*)JS_GetOpaque(val, s_ax_Ref_class_id);
    if (!ref)
    {
        return false;
    }
    
    T* casted = dynamic_cast<T*>(ref);
    if (!casted)
    {
        return false;
    }
    
    *out = casted;
    return true;
}

// Helper to convert C++ Object* -> JSValue
AX_JS_DLL JSValue object_to_jsval(JSContext* ctx, Ref* ptr, const char* typeName);

// Overload for non-Ref Axmol objects (e.g., SpriteFrameCache)
AX_JS_DLL JSValue object_to_jsval(JSContext* ctx, void* ptr, const char* typeName);

} // namespace ax
