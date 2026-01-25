# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

### C++ library

add_library(HeadsetKontrolLib)

ecm_add_qml_module(HeadsetKontrolLib
    URI "${HK_APP_ID}"
    VERSION "${PROJECT_VERSION}"
    CLASS_NAME HeadsetKontrolQmlPlugin
    PLUGIN_TARGET HeadsetKontrolQmlPlugin
    GENERATE_PLUGIN_SOURCE
    DEPENDENCIES
        QtQml
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        org.kde.ki18n
        org.kde.kirigami
        org.kde.kirigamiaddons.formcard
        org.kde.kirigamiaddons.settings
        org.kde.kirigamiaddons.statefulapp
)

add_library(HeadsetKontrol::HeadsetKontrol ALIAS HeadsetKontrolLib)

add_library(HeadsetKontrol::HeadsetKontrolQmlPlugin ALIAS HeadsetKontrolQmlPlugin)

set_target_properties(HeadsetKontrolLib PROPERTIES
    OUTPUT_NAME HeadsetKontrol
)

ecm_setup_version("${PROJECT_VERSION}"
    VARIABLE_PREFIX HK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/headsetkontrol_version.hpp"
)

ecm_generate_export_header(HeadsetKontrolLib
    VERSION "${PROJECT_VERSION}"
    BASE_NAME HK
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/headsetkontrol_export.hpp"
    USE_VERSION_HEADER "\"headsetkontrol_version.hpp\""
)

kconfig_add_kcfg_files(HeadsetKontrolLib GENERATE_MOC USE_RELATIVE_PATH kconfig/hkconfig.kcfgc)

target_sources(HeadsetKontrolLib PRIVATE
    include/hkbattery.hpp
    include/hkchatmix.hpp
    include/hkconfigupdate.hpp
    include/hkdbusactivationevent.hpp
    include/hkheadset.hpp
    include/hkheadsetcontrol.hpp
    include/hkheadseterror.hpp
    include/hkheadsetid.hpp
    include/hklogging.hpp
    qml/hkapplication.cpp
    qml/hkapplication.hpp
    qml/hkqmlutils.cpp
    qml/hkqmlutils.hpp
    qml/types.cpp
    qml/types.hpp
    src/hkbattery.cpp
    src/hkchatmix.cpp
    src/hkclampednumber_p.hpp
    src/hkconfigupdate.cpp
    src/hkconfigupdate/update_2_1_0_main_config.cpp
    src/hkdbusactivationevent.cpp
    src/hkhcutils_p.hpp
    src/hkheadset_p.hpp
    src/hkheadset.cpp
    src/hkheadsetcontrol_p.hpp
    src/hkheadsetcontrol.cpp
    src/hkheadseterror.cpp
    src/hkheadsetid.cpp
    src/hklogging.cpp
    src/hkmacros_p.hpp
    src/hkobjectprivate_p.cpp
    src/hkobjectprivate_p.hpp
)

ecm_target_qml_sources(HeadsetKontrolLib SOURCES
    qml/ui/AppDrawer.qml
    qml/ui/GeneralConfigPage.qml
    qml/ui/HeadsetControlConfigPage.qml
    qml/ui/Main.qml
    qml/ui/UserInterfaceConfigPage.qml
)

target_include_directories(HeadsetKontrolLib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/kconfig"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/qml"
)

target_link_libraries(HeadsetKontrolLib
    PUBLIC
        Qt6::Core
        KF6::ConfigGui
    PRIVATE
        KF6::CoreAddons
        KF6::I18n
        KF6::Notifications
        KF6::StatusNotifierItem
        KF6::WindowSystem
        KirigamiAddonsStatefulApp
        "${HEADSETCONTROL_TARGET}"
)

ecm_qt_export_logging_category(
    IDENTIFIER HK_LOGGING
    CATEGORY_NAME "${HK_APP_ID}"
    DESCRIPTION "HeadsetKontrol's logging category"
    EXPORT HeadsetKontrolLog
)

ecm_finalize_qml_module(HeadsetKontrolLib DESTINATION "${KDE_INSTALL_QMLDIR}")

install(TARGETS HeadsetKontrolLib ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
