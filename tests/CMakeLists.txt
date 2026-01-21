# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

enable_testing(true)

add_executable(test-device test-device.cpp)

add_test(NAME test-device COMMAND test-device)

target_link_libraries(test-device PRIVATE
    Qt6::Test
    HeadsetKontrol::HeadsetControl
)
