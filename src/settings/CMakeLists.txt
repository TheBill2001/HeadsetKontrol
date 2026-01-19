# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

qt_add_library(headsetkontrol_settings STATIC)
ecm_add_qml_module(headsetkontrol_settings
    GENERATE_PLUGIN_SOURCE
    URI org.kde.headsetkontrol.settings
    QML_FILES
        HeadsetKontrolConfigurationView.qml
        GeneralPage.qml
)
