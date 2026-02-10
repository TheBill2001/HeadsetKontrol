# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

ecm_setup_version("${PROJECT_VERSION}"
    VARIABLE_PREFIX HK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/hkversion.hpp"
)

add_library(HKCoreObjects OBJECT
    hkclampednumber.hpp
    hkglobal.hpp
    hkobjectprivate.hpp
    hkutils.hpp
)

ecm_qt_declare_logging_category(HKCoreObjects
    HEADER "${CMAKE_CURRENT_BINARY_DIR}/hklogging.hpp"
    IDENTIFIER HK_LOGGING
    CATEGORY_NAME "${HK_APP_ID}"
    DESCRIPTION "HeadsetKontrol"
    EXPORT HeadsetKontrolLog
)

target_include_directories(HKCoreObjects
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(HKCoreObjects
    PUBLIC
        Qt6::Core
)

add_library(HKCore INTERFACE)

target_link_libraries(HKCore INTERFACE HKCoreObjects $<TARGET_OBJECTS:HKCoreObjects>)
