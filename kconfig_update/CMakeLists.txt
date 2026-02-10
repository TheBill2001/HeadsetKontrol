# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

install(FILES headsetkontrol.upd DESTINATION "${KDE_INSTALL_KCONFUPDATEDIR}")

add_library(HKConfigUpdateCommon OBJECT)

add_library(HK::ConfigUpdateCommon ALIAS HKConfigUpdateCommon)

ecm_qt_declare_logging_category(HKConfigUpdateCommon
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/hkconfupd_logging.hpp"
    IDENTIFIER HKCONFUPD_LOGGING
    CATEGORY_NAME "${HK_APP_ID}.confupd"
    DESCRIPTION "HeadsetKontrol configuration migration"
    EXPORT HeadsetKontrolLog
)

target_include_directories(HKConfigUpdateCommon PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(HKConfigUpdateCommon PUBLIC
    Qt6::Core
)

### headsetkontrol-2.1.0-main-config

add_executable(headsetkontrol-2.1.0-main-config headsetkontrol-2.1.0-main-config.cpp)

target_link_libraries(headsetkontrol-2.1.0-main-config PRIVATE
    KF6::ConfigCore
    HK::ConfigUpdateCommon
)

install(TARGETS headsetkontrol-2.1.0-main-config DESTINATION "${KDE_INSTALL_LIBDIR}/kconf_update_bin")
