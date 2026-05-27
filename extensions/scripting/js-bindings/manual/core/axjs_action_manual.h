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
 * Register Action bindings to QuickJS context
 * Includes: moveBy(), p() helper for creating vectors
 */
void js_register_action_bindings(JSContext* ctx);

}  // namespace ax
