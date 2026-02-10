# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.28)

project(HeadsetKontrol
    VERSION 3.0.0
    DESCRIPTION "Control your headsets with HeadsetControl."
    HOMEPAGE_URL "https://gitlab.com/TheBill2001/headsetkontrol"
    LANGUAGES CXX
)

include(CMakeDependentOption)

option(USE_VENDORED_LIB "Use vendored HeadsetControl library. This force static linking." OFF)
cmake_dependent_option(USE_KCONFIG_UPDATE "Build kconf_update config migration" ON "LINUX" OFF)

if(USE_VENDORED_LIB)
    set(USE_STATIC_LIB ON CACHE BOOL "Use static HeadsetControl library." FORCE)

    include(FetchContent)

    set(HEADSETCONTROL_HASH "b627b4b3ea950dd861673d6ebf0ff186e00521f7"
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

include(ECMDeprecationSettings)
include(ECMFindQmlModule)
include(ECMGenerateExportHeader)
include(ECMInstallIcons)
include(ECMQmlModule)
include(ECMQtDeclareLoggingCategory)
include(ECMSetupVersion)

if (BUILD_TESTING)
    list(APPEND QT_EXTRA_COMPONENTS Test)
endif()

find_package(Qt6 "${QT6_MIN_VERSION}" REQUIRED COMPONENTS
    Qml
    Quick
    Widgets
    ${QT_EXTRA_COMPONENTS}
)

find_package(KF6 "${KF6_MIN_VERSION}" REQUIRED COMPONENTS
    Config
    CoreAddons
    DBusAddons
    I18n
    Kirigami
    Notifications
    StatusNotifierItem
    WindowSystem
)

find_package(KF6KirigamiAddons "${KIRIGAMI_ADDONS_MIN_VERSION}" REQUIRED)
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

ecm_set_disabled_deprecation_versions(
    QT "${QT6_MIN_VERSION}"
    KF "${KF6_MIN_VERSION}"
)

ecm_find_qmlmodule(org.kde.kirigami "${KF6_MIN_VERSION}" REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.formcard "${KIRIGAMI_ADDONS_MIN_VERSION}" REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.settings "${KIRIGAMI_ADDONS_MIN_VERSION}" REQUIRED)
ecm_find_qmlmodule(org.kde.kirigamiaddons.statefulapp "${KIRIGAMI_ADDONS_MIN_VERSION}" REQUIRED)

set(HK_APP_ID "com.thebill2001.headsetkontrol")

add_definitions(
    -DHK_APP_ID="${HK_APP_ID}"
    -DTRANSLATION_DOMAIN="headsetkontrol"
)

add_subdirectory(src)

if(USE_KCONFIG_UPDATE)
    add_subdirectory(kconfig_update)
endif()

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES
    app/*.cpp
    app/*.hpp
    kconfig_update/*.cpp
    kconfig_update/*.hpp
)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)

ki18n_install(po)

ecm_qt_install_logging_categories(
    EXPORT HeadsetKontrolLog
    FILE "${HK_APP_ID}.categories"
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)

file(GLOB_RECURSE ICON_FILES icons/*.svg)
ecm_install_icons(ICONS ${ICON_FILES} DESTINATION "${KDE_INSTALL_ICONDIR}")

unset(HEADSETKONTROL_DESKTOP_ARGS)
configure_file("${HK_APP_ID}.desktop.in" "${HK_APP_ID}.desktop" @ONLY)
install(FILES "${HK_APP_ID}.desktop" DESTINATION "${KDE_INSTALL_APPDIR}")
install(FILES "${HK_APP_ID}.xml" DESTINATION "${KDE_INSTALL_METAINFODIR}")
install(FILES "headsetkontrol.notifyrc"  DESTINATION "${KDE_INSTALL_KNOTIFYRCDIR}")
