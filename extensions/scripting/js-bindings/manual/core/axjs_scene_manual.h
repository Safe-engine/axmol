/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

extern "C" {
#include "quickjs.h"
}

namespace ax
{

/**
 * Register Scene bindings to QuickJS context
 * Includes: constructor, addChild(), onEnter()
 */
void js_register_scene_bindings(JSContext* ctx);

}  // namespace ax
