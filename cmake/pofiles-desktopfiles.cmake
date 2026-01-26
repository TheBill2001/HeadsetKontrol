# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

file(GLOB_RECURSE ALL_PO_FILES "${BASE_DIRECTORY}/po/*.po")

foreach(pofile IN LISTS ALL_PO_FILES)
    cmake_path(RELATIVE_PATH pofile BASE_DIRECTORY "${BASE_DIRECTORY}/po" OUTPUT_VARIABLE rel_path)
    cmake_path(GET rel_path STEM filename)
    cmake_path(GET rel_path PARENT_PATH filedir)
    list(APPEND LINGUAS "${filedir}/${filename}")
endforeach()

string(JOIN "\n" LINGUAS_CONTENT ${LINGUAS})
file(WRITE "${BASE_DIRECTORY}/po/LINGUAS" "${LINGUAS_CONTENT}\n")

execute_process(
    WORKING_DIRECTORY "${BASE_DIRECTORY}"
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
    COMMAND_ERROR_IS_FATAL ANY
    COMMAND "${GETTEXT_MSGFMT_EXECUTABLE}" --desktop
            "--template=${BASE_BUILD_DIRECTORY}/${HK_APP_ID}.desktop.template"
            -d "po"
            "--output=${BASE_BUILD_DIRECTORY}/${HK_APP_ID}.desktop"
)

execute_process(
    WORKING_DIRECTORY "${BASE_DIRECTORY}"
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
    COMMAND_ERROR_IS_FATAL ANY
    COMMAND "${GETTEXT_MSGFMT_EXECUTABLE}" --desktop
            "--template=headsetkontrol.notifyrc"
            -d "po"
            "--output=${BASE_BUILD_DIRECTORY}/headsetkontrol.notifyrc"
)

### Fixup lang name
set(OUT_DESKTOP_FILES
    "${BASE_BUILD_DIRECTORY}/${HK_APP_ID}.desktop"
    "${BASE_BUILD_DIRECTORY}/headsetkontrol.notifyrc"
)

foreach(outdesktopfile IN LISTS OUT_DESKTOP_FILES)
    file(READ "${outdesktopfile}" content)
    foreach(lang_file IN LISTS LINGUAS)
        cmake_path(GET lang_file PARENT_PATH lang)
        string(REPLACE "[${lang_file}]" "[${lang}]" content "${content}")
    endforeach()
    file(WRITE "${outdesktopfile}" "${content}")
endforeach()

file(REMOVE "${BASE_DIRECTORY}/po/LINGUAS")
