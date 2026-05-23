set(QUICKJS_SRC
    QuickJSEngine.h
    QuickJSEngine.cpp
)

# Prefer engine helper if available, otherwise fall back to defining
# the variables expected by the parent CMakeLists ("_AX_QUICKJS_HEADER" and
# "_AX_QUICKJS_SRC"). This makes the module usable when included from
# external projects that don't provide `ax_add_module_sources`.
if(COMMAND ax_add_module_sources)
    ax_add_module_sources(quickjs SRCS ${QUICKJS_SRC})
else()
    set(_AX_QUICKJS_HEADER)
    set(_AX_QUICKJS_SRC)
    foreach(_f ${QUICKJS_SRC})
        # compute absolute path for the file so it's valid when collected by root CMakeLists
        if(IS_ABSOLUTE ${_f})
            set(_abs ${_f})
        else()
            set(_abs "${CMAKE_CURRENT_SOURCE_DIR}/${_f}")
        endif()
        get_filename_component(_abs ABSOLUTE "${_abs}")

        get_filename_component(_ext ${_f} EXT)
        if(_ext STREQUAL ".h" OR _ext STREQUAL ".hpp" OR _ext STREQUAL ".hh" OR _ext STREQUAL ".hxx")
            list(APPEND _AX_QUICKJS_HEADER ${_abs})
        else()
            list(APPEND _AX_QUICKJS_SRC ${_abs})
        endif()
    endforeach()
endif()
