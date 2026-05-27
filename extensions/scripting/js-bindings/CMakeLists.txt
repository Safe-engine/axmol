set(ax_root ${_AX_ROOT})

set(js_bindings_files
  manual/JsRuntime.cpp
  manual/JsEngine.cpp
  manual/JsBindings.cpp
  manual/JsBasicConversions.cpp
  manual/JsCallbacks.cpp
  manual/core/axjs_core_manual.cpp
  manual/core/axjs_sprite_manual.cpp
  manual/core/axjs_scene_manual.cpp
  manual/core/axjs_director_manual.cpp
  manual/core/axjs_action_manual.cpp
  manual/core/axjs_node_manual.cpp
  manual/JsRuntime.h
  manual/JsEngine.h
  manual/JsBindings.h
  manual/JsBasicConversions.h
  manual/JsCallbacks.h
  manual/core/axjs_core_manual.h
  manual/core/axjs_sprite_manual.h
  manual/core/axjs_scene_manual.h
  manual/core/axjs_director_manual.h
  manual/core/axjs_action_manual.h
  manual/core/axjs_node_manual.h
  manual/Js-BindingsExport.h
  # NOTE: auto-generated bindings disabled - use manual bindings for specific classes
)

set(_AX_JS_LIB axjs CACHE INTERNAL "The axmol quickjs bindings lib name")

add_library(${_AX_JS_LIB} STATIC ${js_bindings_files})

get_target_property(js_ext_depend 3rdparty AX_JS_DEPEND)
target_link_libraries(${_AX_JS_LIB} ${js_ext_depend})

setup_ax_extension_config(${_AX_JS_LIB} DNTLINK)

target_include_directories(${_AX_JS_LIB}
  PRIVATE ${ax_root}
  PRIVATE ${ax_root}/3rdparty/quickjs
  PUBLIC ${ax_root}/extensions/scripting
  INTERFACE ${ax_root}/extensions/scripting/js-bindings/manual
)

set_target_properties(${_AX_JS_LIB}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  FOLDER "Extensions"
)

if(XCODE OR VS)
  ax_mark_code_files("${_AX_JS_LIB}")
endif()
