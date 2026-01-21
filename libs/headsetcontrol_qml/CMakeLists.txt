# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

add_library(HKHeadsetControlPlugin STATIC)
ecm_add_qml_module(HKHeadsetControlPlugin
    URI com.thebill2001.headsetkontrol.headsetcontrol
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    NAMESPACE HeadsetKontrol
    CLASS_NAME HeadsetControlPlugin
    PLUGIN_TARGET HKHeadsetControlPlugin
    GENERATE_PLUGIN_SOURCE
    SOURCES
        types.hpp
        types.cpp
)

add_library(HeadsetKontrol::HeadsetControlPlugin ALIAS HKHeadsetControlPlugin)

target_link_libraries(HKHeadsetControlPlugin PRIVATE
    Qt6::Qml
    HeadsetKontrol::HeadsetControl
)
