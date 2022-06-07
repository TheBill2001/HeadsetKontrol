add_executable(${PROJECT_NAME}
    main.cpp
    about.h
    about.cpp
    resources.qrc
)

target_link_libraries(${PROJECT_NAME}
    # Qt stuffs
    Qt5::Quick
    Qt5::Qml
    Qt5::Gui
    Qt5::QuickControls2
    Qt5::Widgets

    # KDE stuffs
    KF5::Kirigami2
    KF5::I18n
    KF5::CoreAddons
)
