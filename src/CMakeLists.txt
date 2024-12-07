qt_add_executable(headsetkontrol)

set_source_files_properties(../headsetkontrol.desktop PROPERTIES
    QT_RESOURCE_ALIAS headsetkontrol.desktop
)

qt_add_resources(headsetkontrol "resources"
    PREFIX "/resources"
    FILES
        ../headsetkontrol.desktop
)

ecm_add_qml_module(headsetkontrol
    URI com.gitlab.thebill2001.headsetkontrol
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    IMPORTS
        org.kde.kirigami
        org.kde.kirigamiaddons.settings
        org.kde.kirigamiaddons.formcard
)

ecm_target_qml_sources(headsetkontrol
    SOURCES
        contents/ui/Main.qml
        contents/ui/DevicesPage.qml
        contents/ui/HeadsetControlSettingsPage.qml
        contents/ui/GeneralSettingsPage.qml
        contents/ui/SettingsView.qml
        contents/ui/DeviceDetailPage.qml
        contents/ui/FormTextFieldWithButtonDelegate.qml
        contents/ui/FormSliderDelegate.qml
)

target_sources(headsetkontrol PRIVATE
    main.cpp
    processqueue.h processqueue.cpp
    headsetcontrol.h headsetcontrol.cpp
    headsetkontrolconfig.h headsetkontrolconfig.cpp
    headsetkontrolnotifieritem.h headsetkontrolnotifieritem.cpp
    headsetcontroldevice.h headsetcontroldevice.cpp
    headsetcontroldevicebattery.h headsetcontroldevicebattery.cpp
    headsetkontrolapplication.h headsetkontrolapplication.cpp
)

kconfig_add_kcfg_files(headsetkontrol GENERATE_MOC headsetkontrolconfigbase.kcfgc)

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
        KF6::DBusAddons
        KF6::Notifications
        KF6::IconThemes
        KF6::StatusNotifierItem
        KF6::NotifyConfig

        KirigamiAddonsStatefulApp
)

install(TARGETS headsetkontrol ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
