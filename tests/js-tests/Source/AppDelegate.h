/****************************************************************************
 Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).

 https://axmol.dev/
 ****************************************************************************/

#pragma once

#include "axmol/axmol.h"

class AppDelegate : private ax::Application
{
public:
    void initContextAttrs() override;
    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
};
