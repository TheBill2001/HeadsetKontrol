# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(headsetkontrol)

ecm_setup_version("${PROJECT_VERSION}"
    VARIABLE_PREFIX HK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/headsetkontrol_version.hpp"
)

ecm_qt_declare_logging_category(headsetkontrol
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hk_logging_p.hpp"
    IDENTIFIER HK_LOGGING
    CATEGORY_NAME "${HK_APP_ID}"
    DESCRIPTION "HeadsetKontrol application"
    EXPORT HeadsetKontrolLog
)

ecm_add_qml_module(headsetkontrol
    URI "${HK_APP_ID}"
    VERSION "${PROJECT_VERSION}"
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

target_sources(headsetkontrol PRIVATE
    main.cpp

    src/hkapplication.cpp
    src/hkapplication.hpp
    src/hkdbusactivationevent.cpp
    src/hkdbusactivationevent.hpp
    src/hkheadsetcontrolextension.cpp
    src/hkheadsetcontrolextension.hpp
    src/hkheadsetlistmodel.cpp
    src/hkheadsetlistmodel.hpp
    src/hkutils.cpp
    src/hkutils.hpp
    src/types.cpp
    src/types.hpp
)

ecm_target_qml_sources(headsetkontrol SOURCES
    qml/AppDrawer.qml
    qml/FormProgressBarDelegate.qml
    qml/GeneralConfigPage.qml
    qml/HeadsetControlConfigPage.qml
    qml/HeadsetPage.qml
    qml/Main.qml
    qml/UserInterfaceConfigPage.qml
)

target_include_directories(headsetkontrol PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/src"
)

target_link_libraries(headsetkontrol
    PRIVATE
        HK::Common
        HK::Config
        HK::HeadsetControl
        KF6::CoreAddons
        KF6::DBusAddons
        KF6::I18n
        KF6::I18nQml
        KF6::Notifications
        KF6::StatusNotifierItem
        KF6::WindowSystem
        KirigamiAddonsStatefulApp
        KirigamiApp
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
