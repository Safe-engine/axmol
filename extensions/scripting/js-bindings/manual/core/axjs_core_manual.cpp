/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#include "js-bindings/manual/core/axjs_core_manual.h"
#include "js-bindings/manual/core/axjs_sprite_manual.h"
#include "js-bindings/manual/core/axjs_scene_manual.h"
#include "js-bindings/manual/core/axjs_director_manual.h"
#include "js-bindings/manual/core/axjs_action_manual.h"
#include "js-bindings/manual/core/axjs_node_manual.h"

extern "C" {
#include "quickjs.h"
}

namespace ax
{

void js_register_core_bindings(JSContext* ctx)
{
    if (!ctx)
        return;

    // Register all individual binding groups
    js_register_sprite_bindings(ctx);
    js_register_scene_bindings(ctx);
    js_register_director_bindings(ctx);
    js_register_action_bindings(ctx);
    js_register_node_bindings(ctx);
}

}  // namespace ax
