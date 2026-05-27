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
 * Register Sprite bindings to QuickJS context
 * Includes: create(), createWithSpriteFrameName(), setPosition(), setScale(),
 *           setAnchorPoint(), setOpacity(), getTexture(), runAction()
 */
void js_register_sprite_bindings(JSContext* ctx);

}  // namespace ax
