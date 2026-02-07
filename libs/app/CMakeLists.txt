# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

### C++ library

add_library(HKApplication)

add_library(HK::Application ALIAS HKApplication)

qt6_extract_metatypes(HKApplication)

ecm_setup_version("${PROJECT_VERSION}"
    VARIABLE_PREFIX HK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/include/hkapp_version.hpp"
)

ecm_generate_export_header(HKApplication
    VERSION "${PROJECT_VERSION}"
    BASE_NAME HKAPP
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/hkapp_export.hpp"
)

ecm_qt_export_logging_category(
    IDENTIFIER HKAPP_LOGGING
    CATEGORY_NAME "${HK_APP_ID}"
    DESCRIPTION "HeadsetKontrol application"
    EXPORT HeadsetKontrolLog
)

kconfig_add_kcfg_files(HKApplication GENERATE_MOC USE_RELATIVE_PATH kconfig/hkconfig.kcfgc)

target_sources(HKApplication PRIVATE
    include/hkapp_logging.hpp
    include/hklogstorage.hpp
    include/hkutils.hpp
    src/hkapp_logging.cpp
    src/hklogstorage.cpp
    src/hkutils.cpp
)

target_include_directories(HKApplication
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"    
        "${CMAKE_CURRENT_BINARY_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/kconfig"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(HKApplication
    PUBLIC
        HK::HeadsetControl
        KF6::ConfigGui
    PRIVATE
        HK::Common
        KF6::I18n
        KF6::WindowSystem
)

install(TARGETS HKApplication ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

### QML library

ecm_add_qml_module(HKApplicationPlugin
    URI "${HK_APP_ID}"
    VERSION "${PROJECT_VERSION}"
    CLASS_NAME HKApplicationPlugin
    PLUGIN_TARGET HKApplicationPlugin
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
    IMPORTS
        "${HK_APP_ID}.headsetcontrol"
)

add_library(HK::ApplicationPlugin ALIAS HKApplicationPlugin)

ecm_target_qml_sources(HKApplicationPlugin SOURCES
    qml/Main.qml

    qml/AppDrawer.qml
    qml/ErrorDialog.qml
    qml/FormProgressBarDelegate.qml
    qml/GeneralConfigPage.qml
    qml/HeadsetControlConfigPage.qml
    qml/HeadsetPage.qml
    qml/UserInterfaceConfigPage.qml
)

target_sources(HKApplicationPlugin PRIVATE
    qml/hkapplication.cpp
    qml/hkapplication.hpp
    qml/hkheadsetlistmodel.cpp
    qml/hkheadsetlistmodel.hpp
    qml/hkutils_qml.cpp
    qml/hkutils_qml.hpp
    qml/types.cpp
    qml/types.hpp
)

target_include_directories(HKApplicationPlugin
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/qml"
)

target_link_libraries(HKApplicationPlugin PRIVATE
    HK::Application
    KF6::CoreAddons
    KF6::I18n
    KirigamiAddonsStatefulApp
)

ecm_finalize_qml_module(HKApplicationPlugin DESTINATION "${KDE_INSTALL_QMLDIR}")

