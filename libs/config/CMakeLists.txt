# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_library(HKConfig)

add_library(HK::Config ALIAS HKConfig)

qt6_extract_metatypes(HKConfig)

ecm_generate_export_header(HKConfig
    VERSION "${PROJECT_VERSION}"
    BASE_NAME HKCONFIG
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/include/hkconfig_export.hpp"
)

ecm_qt_declare_logging_category(HKConfig
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/hkconfig_logging_p.hpp"
    IDENTIFIER HKCONFIG_LOGGING
    CATEGORY_NAME "${HK_APP_ID}.config"
    DESCRIPTION "HeadsetKontrol's Config module"
    EXPORT HeadsetKontrolLog
)

kconfig_add_kcfg_files(HKConfig GENERATE_MOC USE_RELATIVE_PATH kconfig/hkconfig.kcfgc)

target_sources(HKConfig PRIVATE
    include/hkconfigupdate.hpp
    src/hkconfigupdate.cpp
    src/update_2_1_0_main_config.cpp
)

target_include_directories(HKConfig
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/include"
        "${CMAKE_CURRENT_BINARY_DIR}/kconfig"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}/src"
)

target_compile_definitions(HKConfig
    PUBLIC
        USE_KCONFIG_UPDATE
)

target_link_libraries(HKConfig
    PUBLIC
        Qt6::Core
        KF6::ConfigGui
)

install(TARGETS HKConfig ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
