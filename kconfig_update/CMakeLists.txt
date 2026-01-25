# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

install(FILES headsetkontrol.upd DESTINATION "${KDE_INSTALL_KCONFUPDATEDIR}")

### headsetkontrol-2.1.0-main-config

add_executable(headsetkontrol-2.1.0-main-config headsetkontrol-2.1.0-main-config.cpp)

target_link_libraries(headsetkontrol-2.1.0-main-config PRIVATE
    HeadsetKontrol::HeadsetKontrol
)

install(TARGETS headsetkontrol-2.1.0-main-config DESTINATION "${KDE_INSTALL_LIBDIR}/kconf_update_bin")
