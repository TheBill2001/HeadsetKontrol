# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_executable(headsetkontrol
    main.cpp
)

target_link_libraries(headsetkontrol
    PRIVATE
        HK::Application
        KF6::CoreAddons
        KF6::DBusAddons
        KF6::I18n
        KF6::I18nQml
        KF6::Notifications
        KF6::StatusNotifierItem
        KirigamiApp
        Qt6::Qml    
        Qt6::Quick
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

unset(HEADSETKONTROL_DESKTOP_ARGS)
configure_file("${HK_APP_ID}.desktop.in" "${HK_APP_ID}.desktop.template" @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${HK_APP_ID}.desktop" DESTINATION "${KDE_INSTALL_APPDIR}")

install(FILES "${HK_APP_ID}.xml" DESTINATION "${KDE_INSTALL_METAINFODIR}")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/headsetkontrol.notifyrc"  DESTINATION "${KDE_INSTALL_KNOTIFYRCDIR}")
