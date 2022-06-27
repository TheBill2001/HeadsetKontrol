cmake_minimum_required(VERSION 3.20)

# Project info
project(HeadsetKontrol)

set(PROJECT_VERSION "1.0.0")

set(KF_MIN_VERSION "5.92.0")
set(QT_MIN_VERSION "5.15.3")

# Standard KDE project config
find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(KDECompilerSettings NO_POLICY_SCOPE)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX HEADSETKONTROL
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/headsetkontrolversion.h"
)

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Core QuickControls2 DBus)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS Kirigami2 I18n CoreAddons Config DBusAddons Notifications)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Subdir to sources
add_subdirectory(src)

## Source formating
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.hpp *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

include(KDEGitCommitHooks)
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION ${KDE_INSTALL_BINDIR})
install(DIRECTORY res/icons/ DESTINATION ${KDE_INSTALL_ICONDIR})
install(PROGRAMS res/desktop/com.github.headsetkontrol.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES res/knotifications5/headsetkontrol.notifyrc DESTINATION ${KDE_INSTALL_KNOTIFYRCDIR})
