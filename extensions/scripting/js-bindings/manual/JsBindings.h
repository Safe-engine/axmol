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
 * Register all Axmol bindings to QuickJS context
 */
void js_register_all_bindings(JSContext* ctx);

}  // namespace ax
