# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_library(HKHeadsetControlObjects OBJECT
    hkbattery.cpp
    hkbattery.hpp
    hkchatmix.cpp
    hkchatmix.hpp
    hkheadset_p.hpp
    hkheadset.cpp
    hkheadset.hpp
    hkheadsetcontrol_p.hpp
    hkheadsetcontrol.cpp
    hkheadsetcontrol.hpp
    hkheadseterror.cpp
    hkheadseterror.hpp
    hkheadsetid.cpp
    hkheadsetid.hpp
)

qt6_extract_metatypes(HKHeadsetControlObjects)

target_include_directories(HKHeadsetControlObjects
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(HKHeadsetControlObjects
    PUBLIC
        Qt6::QmlIntegration
        HKCore
    PRIVATE
        KF6::I18n
        "${HEADSETCONTROL_TARGET}"
)

add_library(HKHeadsetControl INTERFACE)

target_link_libraries(HKHeadsetControl INTERFACE HKHeadsetControlObjects $<TARGET_OBJECTS:HKHeadsetControlObjects>)
