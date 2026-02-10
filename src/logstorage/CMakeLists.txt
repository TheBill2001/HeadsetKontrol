# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_library(HKLogStorageObjects OBJECT
    hklogstorage.cpp
    hklogstorage.hpp
)

qt6_extract_metatypes(HKLogStorageObjects)

target_include_directories(HKLogStorageObjects
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(HKLogStorageObjects
    PUBLIC
        HKCore
)

add_library(HKLogStorage INTERFACE)

target_link_libraries(HKLogStorage INTERFACE HKLogStorageObjects $<TARGET_OBJECTS:HKLogStorageObjects>)
