# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

if(TARGET headsetcontrol_lib)
    add_library(HeadsetControl::HeadsetControl-static ALIAS headsetcontrol_lib)

    set(HeadsetControl_HeadsetControl-static_FOUND TRUE)

    if(TARGET headsetcontrol_shared)
        add_library(HeadsetControl::HeadsetControl-shared ALIAS headsetcontrol_shared)
        set(HeadsetControl_HeadsetControl-shared_FOUND TRUE)
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(HeadsetControl
        HANDLE_COMPONENTS
    )

    if(HeadsetControl_FOUND)
        get_target_property(HeadsetControl_INCLUDE_DIRS headsetcontrol_lib INCLUDE_DIRECTORIES)

        if(NOT TARGET HeadsetControl::HeadsetControl)
            if(TARGET headsetcontrol_shared)
                add_library(HeadsetControl::HeadsetControl ALIAS headsetcontrol_shared)
            elseif(TARGET headsetcontrol_lib)
                add_library(HeadsetControl::HeadsetControl ALIAS headsetcontrol_lib)
            endif()
        endif()
    endif()
else()
    # Find headers
    find_path(HeadsetControl_INCLUDE_DIR
        NAMES
            headsetcontrol.hpp
            headsetcontrol_c.h
        PATH_SUFFIXES headsetcontrol
    )

    # Find static library
    if(MSVC)
        set(_headsetcontrol_static_lib_name headsetcontrol_static.lib)
    else()
        set(_headsetcontrol_static_lib_name libheadsetcontrol.a)
    endif()

    find_library(HeadsetControl_STATIC_LIBRARY
        NAMES
            "${_headsetcontrol_static_lib_name}"
    )

    find_package(hidapi REQUIRED)

    if(HeadsetControl_STATIC_LIBRARY AND NOT TARGET HeadsetControl::HeadsetControl-static)
        add_library(HeadsetControl::HeadsetControl-static STATIC IMPORTED)
        
        set_target_properties(HeadsetControl::HeadsetControl-static PROPERTIES
            IMPORTED_LOCATION "${HeadsetControl_STATIC_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${HeadsetControl_INCLUDE_DIR}"
        )
        
        set_property(TARGET HeadsetControl::HeadsetControl-static APPEND PROPERTY 
            INTERFACE_LINK_LIBRARIES "hidapi::hidapi;m"
        )

        set(HeadsetControl_HeadsetControl-static_FOUND TRUE)
    endif()

    # Find shared library
    find_library(HeadsetControl_SHARED_LIBRARY
        NAMES
            libheadsetcontrol.so
            libheadsetcontrol.dylib
            headsetcontrol.dll
    )

    if(WIN32 OR MSVC)
        find_library(HeadsetControl_IMPORT_LIBRARY
            NAMES
                headsetcontrol.dll.a
                headsetcontrol.lib
        )
    endif()

    if(HeadsetControl_SHARED_LIBRARY AND NOT TARGET HeadsetControl::HeadsetControl-shared)
        add_library(HeadsetControl::HeadsetControl-shared SHARED IMPORTED)
        
        set_target_properties(HeadsetControl::HeadsetControl-shared PROPERTIES
            IMPORTED_LOCATION "${HeadsetControl_SHARED_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${HeadsetControl_INCLUDE_DIR}"
        )
        
        if((WIN32 OR MSVC) AND HeadsetControl_IMPORT_LIBRARY)
            set_target_properties(HeadsetControl::HeadsetControl-shared PROPERTIES
                IMPORTED_IMPLIB "${HeadsetControl_IMPORT_LIBRARY}"
            )
        endif()
        
        target_link_libraries(HeadsetControl::HeadsetControl-shared INTERFACE hidapi::include)
        
        set(HeadsetControl_HeadsetControl-shared_FOUND TRUE)
    endif()

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(HeadsetControl
        REQUIRED_VARS HeadsetControl_INCLUDE_DIR
        HANDLE_COMPONENTS
    )

    if(HeadsetControl_FOUND)
        set(HeadsetControl_INCLUDE_DIRS "${HeadsetControl_INCLUDE_DIR}")

        if(NOT TARGET HeadsetControl::HeadsetControl)
            if(TARGET HeadsetControl::HeadsetControl-shared)
                set(HeadsetControl_LIBRARIES "${HeadsetControl_SHARED_LIBRARY}")
                add_library(HeadsetControl::HeadsetControl INTERFACE IMPORTED)
                target_link_libraries(HeadsetControl::HeadsetControl INTERFACE HeadsetControl::HeadsetControl-shared)
            elseif(TARGET HeadsetControl::HeadsetControl-static)
                set(HeadsetControl_LIBRARIES "${HeadsetControl_STATIC_LIBRARY}")
                add_library(HeadsetControl::HeadsetControl INTERFACE IMPORTED)
                target_link_libraries(HeadsetControl::HeadsetControl INTERFACE HeadsetControl::HeadsetControl-static)
            endif()
        endif()
    endif()
endif()



