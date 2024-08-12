add_executable(headsetkontrol)

ecm_add_qml_module(headsetkontrol
    URI headsetkontrol
    VERSION 1.1
)

ecm_target_qml_sources(headsetkontrol
    SOURCES
        contents/ui/Main.qml
        contents/ui/MainPage.qml
        contents/ui/AppComponents/GroupBox.qml
        contents/ui/SettingsPage/AboutSettingsPage.qml
        contents/ui/SettingsPage/GeneralSettingsPage.qml
        contents/ui/SettingsPage/SettingsPage.qml       
)

target_sources(headsetkontrol PUBLIC
    main.cpp
    headsetcontrol.h
    headsetcontrol.cpp
    processqueue.h
    processqueue.cpp
    trayicon.h
    trayicon.cpp
    appcontroller.h
    appcontroller.cpp
)

kconfig_add_kcfg_files(headsetkontrol GENERATE_MOC headsetkontrolconfig.kcfgc)

target_link_libraries(headsetkontrol
    PRIVATE
        # Qt stuffs
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
        # Qt6::Widgets
        Qt6::DBus

        # KDE stuffs
        KF6::Kirigami
        KF6::I18n
        KF6::CoreAddons
        KF6::ConfigCore
        KF6::ConfigGui
        KF6::DBusAddons
        KF6::Notifications
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
