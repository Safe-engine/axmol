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
 * Register Director bindings to QuickJS context
 * Includes: getInstance(), getRunningScene(), runScene()
 */
void js_register_director_bindings(JSContext* ctx);

}  // namespace ax
