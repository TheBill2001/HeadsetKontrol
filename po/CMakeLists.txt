# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

set(XGETTEXT_EXECUTABLE "xgettext" CACHE STRING "xgettext executable")

### Create template files and merging translation files
add_custom_target(potfile-merge
    VERBATIM
    COMMENT "Extracting translate template..."
    COMMAND ${CMAKE_COMMAND}
            -DBASE_DIRECTORY=${PROJECT_SOURCE_DIR}
            -DVERSION=${PROJECT_VERSION}
            -DXGETTEXT_EXECUTABLE=${XGETTEXT_EXECUTABLE}
            -DGETTEXT_MSGMERGE_EXECUTABLE=${GETTEXT_MSGMERGE_EXECUTABLE}
            -P ${PROJECT_SOURCE_DIR}/cmake/potfile-merge.cmake
)

### Translate Desktop files
add_custom_target(pofiles-desktopfiles
    ALL VERBATIM
    COMMENT "Translate desktop files..."
    COMMAND ${CMAKE_COMMAND}
            -DBASE_DIRECTORY=${PROJECT_SOURCE_DIR}
            -DBASE_BUILD_DIRECTORY=${PROJECT_BINARY_DIR}
            -DGETTEXT_MSGFMT_EXECUTABLE=${GETTEXT_MSGFMT_EXECUTABLE}
            -DHK_APP_ID=${HK_APP_ID}
            -P ${PROJECT_SOURCE_DIR}/cmake/pofiles-desktopfiles.cmake
)

### Install .po files
ki18n_install("${CMAKE_CURRENT_SOURCE_DIR}")
