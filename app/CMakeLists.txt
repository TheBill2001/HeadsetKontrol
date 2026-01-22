# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(headsetkontrol
    src/main.cpp

    src/hkapplication.cpp
    src/hkapplication.hpp
    src/hkdbusactivationevent.cpp
    src/hkdbusactivationevent.hpp
)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX HK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hkversion_p.hpp"
)

ecm_qt_declare_logging_category(headsetkontrol
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hklogging_p.hpp"
    IDENTIFIER HK_LOGGING
    CATEGORY_NAME "${HK_APP_ID}"
    DESCRIPTION "HeadsetKontrol application"
    EXPORT HeadsetKontrolLog
)

ecm_add_qml_module(headsetkontrol
    URI "${HK_APP_ID}"
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    DEPENDENCIES
        QtQuick
        org.kde.ki18n
        org.kde.kirigami
        org.kde.kirigamiaddons.formcard
        org.kde.kirigamiaddons.statefulapp
        "${HK_APP_ID}.headsetkontrol"
)

ecm_target_qml_sources(headsetkontrol SOURCES
    qml/Main.qml
)

target_include_directories(headsetkontrol PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_BINARY_DIR}/src"
)

target_link_libraries(headsetkontrol PRIVATE
    Qt6::Widgets
    Qt6::Quick
    KF6::CoreAddons
    KF6::DBusAddons
    KF6::I18n
    KF6::I18nQml
    KF6::WindowSystem
    KirigamiAddonsStatefulApp
    KirigamiApp
    HeadsetKontrol::HeadsetControl
    HeadsetKontrol::HeadsetControlPlugin
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
