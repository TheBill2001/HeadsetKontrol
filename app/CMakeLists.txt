# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(headsetkontrol
    src/main.cpp
)

target_include_directories(headsetkontrol PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
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
