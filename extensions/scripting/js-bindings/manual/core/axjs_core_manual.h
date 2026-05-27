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
void js_register_core_bindings(JSContext* ctx);

// /**
//  * Individual binding registrations
//  */
// void js_register_sprite_bindings(JSContext* ctx);
// void js_register_scene_bindings(JSContext* ctx);
// void js_register_director_bindings(JSContext* ctx);
// void js_register_action_bindings(JSContext* ctx);
// void js_register_node_bindings(JSContext* ctx);

}  // namespace ax
