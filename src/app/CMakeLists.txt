# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(headsetkontrol
    main.cpp

    hkheadsetlistmodel.cpp
    hkheadsetlistmodel.hpp
    hkkirigamiapplication.cpp
    hkkirigamiapplication.hpp
    hkqmltypes.cpp
    hkqmltypes.hpp
    hkutils_qml.cpp
    hkutils_qml.hpp
)

kconfig_add_kcfg_files(headsetkontrol GENERATE_MOC hkconfig.kcfgc)

set(HEADSETKONTROL_DESKTOP_ARGS " --minimized")
configure_file("${PROJECT_SOURCE_DIR}/${HK_APP_ID}.desktop.in" "${PROJECT_BINARY_DIR}/${HK_APP_ID}-autostart.desktop" @ONLY)
set_source_files_properties("${PROJECT_BINARY_DIR}/${HK_APP_ID}-autostart.desktop" PROPERTIES
    QT_RESOURCE_ALIAS "${HK_APP_ID}.desktop"
)
qt_add_resources(headsetkontrol "resources"
    PREFIX /
    FILES "${PROJECT_BINARY_DIR}/${HK_APP_ID}-autostart.desktop"
)

ecm_add_qml_module(headsetkontrol
    URI "${HK_APP_ID}"
    VERSION "${PROJECT_VERSION}"
    DEPENDENCIES
        QtQuick
        QtQuick.Controls
        QtQuick.Layouts
        org.kde.ki18n
        org.kde.kirigami
        org.kde.kirigamiaddons.formcard
        org.kde.kirigamiaddons.settings
        org.kde.kirigamiaddons.statefulapp
)

ecm_target_qml_sources(headsetkontrol SOURCES
    qml/AppDrawer.qml
    qml/ErrorDialog.qml
    qml/FormProgressBarDelegate.qml
    qml/GeneralConfigPage.qml
    qml/HeadsetControlConfigPage.qml
    qml/HeadsetPage.qml
    qml/Main.qml
    qml/UserInterfaceConfigPage.qml
)

target_link_libraries(headsetkontrol PRIVATE
    Qt6::Widgets
    KF6::CoreAddons
    KF6::DBusAddons
    KF6::I18n
    KF6::I18nQml
    KF6::Notifications
    KF6::StatusNotifierItem
    KF6::WindowSystem
    KirigamiAddonsStatefulApp
    KirigamiApp
    HKHeadsetControl
    HKLogStorage
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
