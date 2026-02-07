# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

### C++ library

add_library(HKHeadsetControl)

add_library(HK::HeadsetControl ALIAS HKHeadsetControl)

qt6_extract_metatypes(HKHeadsetControl)

ecm_generate_export_header(HKHeadsetControl
    VERSION "${PROJECT_VERSION}"
    BASE_NAME HKHC
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/hkhc_export.hpp"
)

ecm_qt_declare_logging_category(HKHeadsetControl
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hkhc_logging_p.hpp"
    IDENTIFIER HKHC_LOGGING
    CATEGORY_NAME "${HK_APP_ID}.headsetcontrol"
    DESCRIPTION "HeadsetKontrol's HeadsetControl module"
    EXPORT HeadsetKontrolLog
)

target_sources(HKHeadsetControl PRIVATE
    include/hkbattery.hpp
    include/hkchatmix.hpp
    include/hkheadset.hpp
    include/hkheadsetcontrol.hpp
    include/hkheadseterror.hpp
    include/hkheadsetid.hpp
    src/hkbattery.cpp
    src/hkchatmix.cpp
    src/hkheadset_p.hpp
    src/hkheadset.cpp
    src/hkheadsetcontrol_p.hpp
    src/hkheadsetcontrol.cpp
    src/hkheadseterror.cpp
    src/hkheadsetid.cpp
)

target_include_directories(HKHeadsetControl
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/src"
)

target_link_libraries(HKHeadsetControl
    PUBLIC
        Qt6::Core
    PRIVATE
        HK::Common
        "${HEADSETCONTROL_TARGET}"
)

install(TARGETS HKHeadsetControl ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

### QML plugin

ecm_add_qml_module(HKHeadsetControlPlugin
    URI "${HK_APP_ID}.headsetcontrol"
    VERSION "${PROJECT_VERSION}"
    CLASS_NAME HKHeadsetControlPlugin
    PLUGIN_TARGET HKHeadsetControlPlugin
    GENERATE_PLUGIN_SOURCE
)

add_library(HK::HeadsetControlPlugin ALIAS HKHeadsetControlPlugin)

target_sources(HKHeadsetControlPlugin PRIVATE
    qml/hkheadsetcontrolextension.cpp
    qml/hkheadsetcontrolextension.hpp
    qml/types.cpp
    qml/types.hpp
)

target_include_directories(HKHeadsetControlPlugin
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/qml"
)

target_link_libraries(HKHeadsetControlPlugin PRIVATE
    HK::HeadsetControl
)

ecm_finalize_qml_module(HKHeadsetControlPlugin DESTINATION "${KDE_INSTALL_QMLDIR}")
