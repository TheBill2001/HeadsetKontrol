# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_library(HKHeadsetControl STATIC
    include/hkbattery.hpp
    include/hkchatmix.hpp
    include/hkheadset.hpp
    include/hkheadsetcontrol.hpp
    include/hkheadseterror.hpp
    include/hkheadsetid.hpp
    src/hkbattery.cpp
    src/hkchatmix.cpp
    src/hkclampednumber_p.hpp
    src/hkhcutils_p.hpp
    src/hkheadset_p.hpp
    src/hkheadset.cpp
    src/hkheadsetcontrol_p.hpp
    src/hkheadsetcontrol.cpp
    src/hkheadseterror.cpp
    src/hkheadsetid.cpp
    src/hkmacros_p.hpp
)

qt_extract_metatypes(HKHeadsetControl)

ecm_add_qml_module(HKHeadsetControl
    URI "${HK_APP_ID}.headsetcontrol"
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    CLASS_NAME HKHeadsetControlPlugin
    PLUGIN_TARGET HKHeadsetControlPlugin
    GENERATE_PLUGIN_SOURCE
    SOURCES
        qml/types.hpp
        qml/types.cpp
)

add_library(HeadsetKontrol::HeadsetControl ALIAS HKHeadsetControl)

add_library(HeadsetKontrol::HeadsetControlPlugin ALIAS HKHeadsetControlPlugin)

ecm_qt_declare_logging_category(HKHeadsetControl
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hkhclogging_p.hpp"
    IDENTIFIER HKHC_LOGGING
    CATEGORY_NAME "${HK_APP_ID}.headsetcontrol"
    DESCRIPTION "HeadsetKontrol's HeadsetControl module"
    EXPORT HeadsetKontrolLog
)

target_include_directories(HKHeadsetControl
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/qml"
        "${CMAKE_CURRENT_BINARY_DIR}/src"
)

target_link_libraries(HKHeadsetControl
    PUBLIC
        Qt6::Core
    PRIVATE
        Qt6::Qml
        KF6::I18n
        "${HEADSETCONTROL_TARGET}"
)
