cmake_minimum_required(VERSION 3.20)

set(APP_NAME js-tests)

project(${APP_NAME})

if(NOT DEFINED BUILD_ENGINE_DONE)
    if(XCODE)
        set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY TRUE)
    endif()

    set(_AX_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    set(CMAKE_MODULE_PATH ${_AX_ROOT}/cmake/Modules/)

    set(AX_ENABLE_EXT_JS ON CACHE BOOL "Build QuickJS libraries" FORCE)
    set(AX_ENABLE_EXT_LUA OFF CACHE BOOL "Build lua libraries" FORCE)

    include(AXBuildSet)
    add_subdirectory(${_AX_ROOT}/axmol ${ENGINE_BINARY_PATH}/axmol)
endif()

set(GAME_SOURCE
    Source/AppDelegate.cpp
    Source/QuickJSTests.cpp
)

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
    message(FATAL_ERROR "js-tests is not supported on Android yet")
elseif(APPLE)
    if(IOS)
        if(TVOS)
            set(APP_UI_RES
                proj.ios/LaunchScreenBackground.png
                proj.ios/targets/tvos/LaunchScreen.storyboard
                proj.ios/targets/tvos/Images.xcassets
            )
        else()
            set(APP_UI_RES
                proj.ios/LaunchScreenBackground.png
                proj.ios/targets/ios/LaunchScreen.storyboard
                proj.ios/targets/ios/Images.xcassets
            )
        endif()
        list(APPEND GAME_SOURCE
            proj.ios/main.mm
            proj.ios/Prefix.pch
            ${APP_UI_RES}
        )
    elseif(MACOSX)
        set(APP_UI_RES
            proj.mac/Icon.icns
            proj.mac/Info.plist
            proj.mac/Prefix.pch
            proj.mac/en.lproj/InfoPlist.strings
        )
        list(APPEND GAME_SOURCE
            proj.mac/main.cpp
            ${APP_UI_RES}
        )
    endif()
    list(APPEND GAME_SOURCE ${common_content_files})
endif()

add_executable(${APP_NAME} ${GAME_SOURCE})

target_link_libraries(${APP_NAME} axjs ${_AX_CORE_LIB})

target_include_directories(${APP_NAME} PRIVATE ${GAME_INC_DIRS})

ax_setup_app_config(${APP_NAME})

if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    set_xcode_property(${APP_NAME} INSTALL_PATH "$(LOCAL_APPS_DIR)")
    set_xcode_property(${APP_NAME} PRODUCT_BUNDLE_IDENTIFIER "dev.axmol.${APP_NAME}")

    if(MACOSX)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.mac/Info.plist")
    elseif(TVOS)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios/targets/tvos/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "Brand Assets")
    elseif(IOS)
        set_target_properties(${APP_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios/targets/ios/Info.plist")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    endif()
endif()

if((NOT APPLE) AND (NOT WINRT))
    ax_get_resource_path(APP_RES_DIR ${APP_NAME})
    ax_sync_target_res(${APP_NAME} LINK_TO ${APP_RES_DIR} FOLDERS ${content_folder} SYM_LINK 1)
endif()

if((NOT IOS) AND (NOT WINRT))
    target_precompile_headers(${APP_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:axmol/axmol.h>"
    )
endif()

ax_setup_app_props(${APP_NAME})
