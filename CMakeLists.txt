# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.28)

project(HeadsetKontrol
    VERSION 2.1.0
    DESCRIPTION "Control your headsets with HeadsetControl."
    HOMEPAGE_URL "https://gitlab.com/TheBill2001/headsetkontrol"
    LANGUAGES CXX
)

option(USE_VENDORED_LIB "Use vendored HeadsetControl library. This force static linking." OFF)

if(USE_VENDORED_LIB)
    set(USE_STATIC_LIB ON CACHE BOOL "Use static HeadsetControl library." FORCE)

    include(FetchContent)

    set(HEADSETCONTROL_HASH "6fe0cec4f8baeae5e6441489df02c395e39c6ae2"
        CACHE STRING "HeadsetControl commit hash for vendored lib."
    )

    FetchContent_Declare(headsetcontrol
        GIT_REPOSITORY "https://github.com/Sapd/HeadsetControl.git"
        GIT_TAG "${HEADSETCONTROL_HASH}"
        EXCLUDE_FROM_ALL
        SYSTEM
    )

    FetchContent_MakeAvailable(headsetcontrol)
else()
    option(USE_STATIC_LIB "Use static HeadsetControl library." OFF)
endif()

include(FeatureSummary)

set(QT6_MIN_VERSION 6.9.0)
set(KF6_MIN_VERSION 6.21.0)
set(KIRIGAMI_ADDONS_MIN_VERSION 1.10.0)

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(FeatureSummary)

include(KDEInstallDirs6)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
include(KDEGitCommitHooks)

include(ECMFindQmlModule)
include(ECMQmlModule)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX HEADSETKONTROL
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/version-headsetkontrol.h"
)

if (BUILD_TESTING)
    list(APPEND QT_EXTRA_COMPONENTS Test)
endif()

find_package(Qt6 ${QT6_MIN_VERSION} REQUIRED COMPONENTS Qml Widgets ${QT_EXTRA_COMPONENTS})
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS Kirigami CoreAddons Config I18n)

find_package(KF6KirigamiAddons ${KIRIGAMI_ADDONS_MIN_VERSION} REQUIRED)
set_package_properties(KF6KirigamiAddons PROPERTIES
    TYPE REQUIRED
    PURPOSE "Required application components and runtime dependency"
)

if(USE_STATIC_LIB)
    find_package(HeadsetControl REQUIRED COMPONENTS HeadsetControl-static)

    set(HEADSETCONTROL_TARGET HeadsetControl::HeadsetControl-static)
else()
    find_package(HeadsetControl REQUIRED)

    set(HEADSETCONTROL_TARGET HeadsetControl::HeadsetControl)
endif()

ecm_find_qmlmodule(org.kde.kirigami ${QT6_MIN_VERSION} REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.formcard ${KIRIGAMI_ADDONS_MIN_VERSION} REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.statefulapp ${KIRIGAMI_ADDONS_MIN_VERSION} REQUIRED)

add_subdirectory(libs)
# add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

unset(HEADSETKONTROL_DESKTOP_ARGS)
configure_file(com.thebill2001.headsetkontrol.desktop.in com.thebill2001.headsetkontrol.desktop @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/com.thebill2001.headsetkontrol.desktop" DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES com.thebill2001.headsetkontrol.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
# ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES headsetcontrol/*.cpp headsetcontrol/*.hpp)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

ecm_qt_install_logging_categories(
    EXPORT HeadsetKontrolLog
    FILE com.thebill2001.headsetkontrol.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)
