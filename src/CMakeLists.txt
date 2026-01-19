# SPDX-License-Identifier: BSD-2-Clause
# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>

add_subdirectory(settings)

qt_add_library(headsetkontrol_static STATIC)
ecm_add_qml_module(headsetkontrol_static
    URI org.kde.headsetkontrol
    GENERATE_PLUGIN_SOURCE
    QML_FILES
        qml/Main.qml
    DEPENDENCIES
        org.kde.kirigamiaddons.formcard
        org.kde.kirigamiaddons.statefulapp
        org.kde.kirigami
        org.kde.headsetkontrol.settings
)

target_sources(headsetkontrol_static PRIVATE
    headsetkontrolapplication.cpp
    headsetkontrolapplication.h
)

target_link_libraries(headsetkontrol_static PUBLIC
    Qt6::Quick
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigGui
    KirigamiAddonsStatefulApp
)
target_include_directories(headsetkontrol_static PUBLIC ${CMAKE_BINARY_DIR})

if (ANDROID)
    kirigami_package_breeze_icons(ICONS
        list-add
        help-about
        application-exit
        applications-graphics
    )
else()
    target_link_libraries(headsetkontrol_static PUBLIC Qt::Widgets)
endif()

kconfig_add_kcfg_files(headsetkontrol_static GENERATE_MOC headsetkontrolconfig.kcfgc)

add_executable(headsetkontrol main.cpp)
target_link_libraries(headsetkontrol PUBLIC
    KirigamiApp
    headsetkontrol_static
    headsetkontrol_staticplugin
    headsetkontrol_settings
    headsetkontrol_settingsplugin
)
install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
ecm_add_android_apk(headsetkontrol ANDROID_DIR ${CMAKE_SOURCE_DIR}/android)
