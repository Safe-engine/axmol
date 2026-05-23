cmake_minimum_required(VERSION 3.20)

set(APP_NAME js-tests)

project(${APP_NAME})

if(NOT DEFINED BUILD_ENGINE_DONE)
  if(XCODE)
    set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
  endif()

  set(_AX_ROOT "$ENV{AX_ROOT}")

  if(NOT(_AX_ROOT STREQUAL ""))
    file(TO_CMAKE_PATH ${_AX_ROOT} _AX_ROOT)
    message(STATUS "Using system env var _AX_ROOT=${_AX_ROOT}")
  else()
    set(_AX_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
  endif()

  set(CMAKE_MODULE_PATH ${_AX_ROOT}/cmake/Modules/)

  include(AXBuildSet)
  add_subdirectory(${_AX_ROOT}/axmol ${ENGINE_BINARY_PATH}/axmol)
endif()

file(GLOB GAME_SOURCE Source/*.cpp)
file(GLOB GAME_HEADER Source/*.h)

set(GAME_INC_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)

set(content_folder
  "${CMAKE_CURRENT_SOURCE_DIR}/Content"
)

if(APPLE)
  ax_mark_multi_resources(common_content_files RES_TO "Resources" FOLDERS ${content_folder})
elseif(WINDOWS)
  ax_mark_multi_resources(common_content_files RES_TO "Content" FOLDERS ${content_folder})
endif()

if(ANDROID)
  list(APPEND GAME_SOURCE
    proj.android/app/jni/main.cpp
  )
elseif(LINUX)
  list(APPEND GAME_SOURCE
    proj.linux/main.cpp
  )
  list(APPEND GAME_SOURCE ${common_content_files})
elseif(WASM)
  list(APPEND GAME_SOURCE
    proj.wasm/main.cpp
  )
  list(APPEND GAME_SOURCE ${common_content_files})
elseif(WINDOWS)
  if(NOT WINRT)
    list(APPEND GAME_HEADER
      proj.win32/main.h
    )
    list(APPEND GAME_SOURCE
      proj.win32/main.cpp
      ${common_content_files}
    )
  else()
    ax_setup_winrt_sources()
  endif()

  list(APPEND GAME_SOURCE ${common_content_files})
elseif(APPLE)
  if(IOS)
    list(APPEND GAME_SOURCE
      proj.ios/main.mm
      proj.ios/Prefix.pch
    )
  elseif(MACOSX)
    list(APPEND GAME_SOURCE
      proj.mac/main.cpp
    )
  endif()

  list(APPEND GAME_SOURCE ${common_content_files})
endif()

add_executable(${APP_NAME} ${GAME_SOURCE} ${GAME_HEADER})

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS} ${_AX_ROOT}/axmol)
target_compile_definitions(${APP_NAME} PRIVATE AX_WITH_QUICKJS=1)
target_link_libraries(${APP_NAME} PRIVATE ${_AX_CORE_LIB})
