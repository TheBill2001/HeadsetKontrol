# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

enable_testing(true)

### Test device

add_executable(test-device test-device.cpp)

add_test(NAME test-device COMMAND test-device)

target_link_libraries(test-device PRIVATE
    Qt6::Test
    HKHeadsetControl
)

### Log storage

add_executable(log-storage log-storage.cpp)

add_test(NAME log-storage COMMAND log-storage)

target_compile_definitions(log-storage PRIVATE
    HK_TEST
)

target_link_libraries(log-storage PRIVATE
    Qt6::Test
    HKLogStorage
)
