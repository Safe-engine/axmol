/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

/* AX_JS_API */
#if defined(_WIN32)
#    if defined(AX_JS_DLLEXPORT)
#        define AX_JS_API __declspec(dllexport)
#    elif defined(AX_JS_DLLIMPORT)
#        define AX_JS_API __declspec(dllimport)
#    endif
#elif defined(AX_JS_DLLEXPORT) || defined(AX_JS_DLLIMPORT)
#    define AX_JS_API __attribute__((visibility("default")))
#endif
#ifndef AX_JS_API
#    define AX_JS_API
#endif

#define AX_JS_DLL AX_JS_API
