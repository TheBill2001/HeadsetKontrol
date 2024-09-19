cmake_minimum_required(VERSION 3.20)

# Project info
project(HeadsetKontrol VERSION 2.0.3 LANGUAGES CXX)

set(QT6_MIN_VERSION "6.5.0")
set(KF6_MIN_VERSION "6.0.0")

# Standard KDE project config
find_package(ECM 6.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMFindQmlModule)
include(ECMQmlModule)
include(ECMPoQmTools)
include(ECMInstallIcons)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX HEADSETKONTROL
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/headsetkontrolversion.h"
)

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
    QuickControls2
    Widgets
    DBus
)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    Kirigami
    I18n
    CoreAddons
    Config
    DBusAddons
    Notifications
    IconThemes
    StatusNotifierItem
    XmlGui
    NotifyConfig
)

find_package(KF6 1.3.0 REQUIRED COMPONENTS KirigamiAddons)

qt_policy(SET QTP0001 NEW)

ecm_find_qmlmodule(org.kde.kirigami REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.settings REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.formcard REQUIRED)

# Subdir to sources
add_subdirectory(src)

## Source formating
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.hpp *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(KDEGitCommitHooks)
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

include(FeatureSummary)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# Build translation
add_subdirectory(translation)

# Install rules
install(PROGRAMS headsetkontrol.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES headsetkontrol.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})

file(GLOB_RECURSE ICON_FILES *.svg)
ecm_install_icons(ICONS ${ICON_FILES} DESTINATION ${KDE_INSTALL_ICONDIR})
