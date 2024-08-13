qt_add_executable(headsetkontrol)

ecm_add_qml_module(headsetkontrol
    URI com.gitlab.thebill2001.headsetkontrol
    VERSION 1.1
    IMPORTS
        org.kde.kirigami
        org.kde.kirigamiaddons.settings
        org.kde.kirigamiaddons.formcard
)

ecm_target_qml_sources(headsetkontrol
    SOURCES
        contents/ui/Main.qml
        contents/ui/DevicesPage.qml
        contents/ui/AboutSettingsPage.qml
        contents/ui/HeadsetControlSettingsPage.qml
        contents/ui/GeneralSettingsPage.qml
        contents/ui/SettingsView.qml
        contents/ui/DeviceDetailPage.qml
        contents/ui/FormTextFieldWithButtonDelegate.qml
        contents/ui/FormSliderDelegate.qml
)

target_sources(headsetkontrol PRIVATE
    main.cpp
    config.h config.cpp
    trayicon.h trayicon.cpp
    headsetcontrol.h headsetcontrol.cpp
    processqueue.h processqueue.cpp
    # appcontroller.h appcontroller.cpp
)

kconfig_add_kcfg_files(headsetkontrol GENERATE_MOC headsetkontrolconfig.kcfgc)

target_link_libraries(headsetkontrol
    PRIVATE
        # Qt stuffs
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Qml
        Qt6::QuickControls2
        Qt6::Widgets
        Qt6::DBus

        # KDE stuffs
        KF6::Kirigami
        KF6::I18n
        KF6::CoreAddons
        KF6::ConfigCore
        KF6::ConfigGui
        KF6::DBusAddons
        KF6::Notifications
        KF6::IconThemes
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
