# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

### HeadsetKontrol library

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
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/headsetkontrol_version.hpp"
)

ecm_generate_export_header(HeadsetKontrolLib
    VERSION "${PROJECT_VERSION}"
    BASE_NAME HK
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/src/headsetkontrol_export.hpp"
    USE_VERSION_HEADER "\"headsetkontrol_version.hpp\""
)

kconfig_add_kcfg_files(HeadsetKontrolLib GENERATE_MOC USE_RELATIVE_PATH kconfig/hkconfig.kcfgc)

target_sources(HeadsetKontrolLib PRIVATE
    qml/types.cpp
    qml/types.hpp
    src/hkapplication.cpp
    src/hkapplication.hpp
    src/hkbattery.cpp
    src/hkbattery.hpp
    src/hkchatmix.cpp
    src/hkchatmix.hpp
    src/hkclampednumber_p.hpp
    src/hkconfigupdate.cpp
    src/hkconfigupdate.hpp
    src/hkconfigupdate/update_2_1_0_main_config.cpp
    src/hkdbusactivationevent.cpp
    src/hkdbusactivationevent.hpp
    src/hkhcutils_p.hpp
    src/hkheadset_p.hpp
    src/hkheadset.cpp
    src/hkheadset.hpp
    src/hkheadsetcontrol_p.hpp
    src/hkheadsetcontrol.cpp
    src/hkheadsetcontrol.hpp
    src/hkheadseterror.cpp
    src/hkheadseterror.hpp
    src/hkheadsetid.cpp
    src/hkheadsetid.hpp
    src/hklogging.cpp
    src/hklogging.hpp
    src/hkmacros_p.hpp
    src/hkobjectprivate_p.cpp
    src/hkobjectprivate_p.hpp
    src/hkqmlutils.cpp
    src/hkqmlutils.hpp
)

ecm_target_qml_sources(HeadsetKontrolLib SOURCES
    qml/AppDrawer.qml
    qml/GeneralConfigPage.qml
    qml/HeadsetControlConfigPage.qml
    qml/Main.qml
    qml/UserInterfaceConfigPage.qml
)

target_include_directories(HeadsetKontrolLib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/kconfig"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/qml"
)

target_link_libraries(HeadsetKontrolLib
    PUBLIC
        Qt6::Core
        Qt6::QmlIntegration
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

### HeadsetKontrol executable

add_executable(headsetkontrol
    main.cpp
)

target_link_libraries(headsetkontrol PRIVATE
    Qt6::Quick
    Qt6::Widgets
    KF6::CoreAddons
    KF6::DBusAddons
    KF6::I18n
    KF6::I18nQml
    KirigamiApp
    HeadsetKontrol::HeadsetKontrol
)

get_target_property(HeadsetKontrolQmlPlugin_TYPE HeadsetKontrol::HeadsetKontrolQmlPlugin TYPE)
if(HeadsetKontrolQmlPlugin_TYPE STREQUAL "STATIC_LIBRARY")
    target_link_libraries(headsetkontrol PRIVATE
        HeadsetKontrol::HeadsetKontrolQmlPlugin
    )
endif()

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
