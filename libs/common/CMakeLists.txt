# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_library(HKCommon INTERFACE)

add_library(HK::Common ALIAS HKCommon)

target_include_directories(HKCommon INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_sources(HKCommon INTERFACE
    include/hkclampednumber.hpp
    include/hkcommonmacros.hpp
    include/hkcommonutils.hpp
    include/hkobjectprivate.hpp
)

target_link_libraries(HKCommon INTERFACE Qt6::Core)
