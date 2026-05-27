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
 * Register Node bindings to QuickJS context
 * Base class bindings for common node operations
 */
void js_register_node_bindings(JSContext* ctx);

}  // namespace ax
