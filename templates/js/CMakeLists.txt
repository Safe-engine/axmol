# /****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
# Copyright (c) 2019-present Axmol Engine contributors (see AUTHORS.md).
#
# https://axmol.dev/
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/

cmake_minimum_required(VERSION 3.22...4.1)

set(APP_NAME Dummy)

project(${APP_NAME})

if(XCODE)
  set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
endif()

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules
)

# Important: QuickJS is required for JavaScript support
set(_AX_JS_PROJ TRUE)
set(_AX_USE_PREBUILT FALSE)

# NOTE: The order of the cmake module "include(AXGame...)" statements matters
include(AXGameEngineOptions)
include(AXGameEngineSetup)

# The common cross-platforms source files and header files
file(GLOB_RECURSE GAME_HEADER
  Source/*.h Source/*.hpp
)

file(GLOB_RECURSE GAME_SOURCE
  Source/*.cpp Source/*.c
)

set(GAME_INC_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

# The content_folder refers to the folder contain res
set(content_folder ${CMAKE_CURRENT_SOURCE_DIR}/Content)

# common cc_common_res files
file(GLOB cc_common_res_files
  RELATIVE ${content_folder}
  ${content_folder}/res/*
)
set(common_content_files "")
foreach(cc_res_file ${cc_common_res_files})
  list(APPEND GAME_SOURCE ${content_folder}/${cc_res_file})
endforeach()

# Config use lua, it will load js engine
set(GAME_RES_FOLDER
  "${content_folder}/res"
  "${content_folder}/src"
  CACHE INTERNAL "res folder"
)

include(AXGameSourceSetup)

# mark app compile info and libs info
set(APP_SOURCES
  ${GAME_HEADER}
  ${GAME_SOURCE}
)

include(AXGameTargetSetup)
include(AXGameFinalSetup)
