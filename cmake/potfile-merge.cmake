# SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

### Extract messages from source code into template file
file(GLOB_RECURSE _ALL_TRANSLATE_SOURCE_FILES
    "${BASE_DIRECTORY}/app/*.cpp"
    "${BASE_DIRECTORY}/app/*.hpp"
    "${BASE_DIRECTORY}/app/*.qml"
)

foreach(sourcefile IN LISTS _ALL_TRANSLATE_SOURCE_FILES)
    cmake_path(RELATIVE_PATH sourcefile BASE_DIRECTORY "${BASE_DIRECTORY}" OUTPUT_VARIABLE rel_path)
    list(APPEND ALL_TRANSLATE_SOURCE_FILES ${rel_path})
endforeach()

set(XGETTEXT_CREDIT_ARGS
    --package-name=headsetkontrol
    "--package-version=${VERSION}"
    "--copyright-holder=Trần Nam Tuấn"
    "--msgid-bugs-address=tuantran1632001@gmail.com"
)

execute_process(
    WORKING_DIRECTORY "${BASE_DIRECTORY}"
    COMMAND "${XGETTEXT_EXECUTABLE}" --c++ --kde --from-code=UTF-8 -c=i18n --width=160
            -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3
            -ki18nd:2 -ki18ndc:2c,3 -ki18ndp:2,3 -ki18ndcp:2c,3,4
            -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3
            -kki18nd:2 -kki18ndc:2c,3 -kki18ndp:2,3 -kki18ndcp:2c,3,4
            -kxi18n:1 -kxi18nc:1c,2 -kxi18np:1,2 -kxi18ncp:1c,2,3
            -kxi18nd:2 -kxi18ndc:2c,3 -kxi18ndp:2,3 -kxi18ndcp:2c,3,4
            -kkxi18n:1 -kkxi18nc:1c,2 -kkxi18np:1,2 -kkxi18ncp:1c,2,3
            -kkxi18nd:2 -kkxi18ndc:2c,3 -kkxi18ndp:2,3 -kkxi18ndcp:2c,3,4
            -kkli18n:1 -kkli18nc:1c,2 -kkli18np:1,2 -kkli18ncp:1c,2,3
            -kklxi18n:1 -kklxi18nc:1c,2 -kklxi18np:1,2 -kklxi18ncp:1c,2,3
            -kI18N_NOOP:1 -kI18NC_NOOP:1c,2
            -kI18N_NOOP2:1c,2 -kI18N_NOOP2_NOSTRIP:1c,2
            -ktr2i18n:1 -ktr2xi18n:1
            "--output=po/headsetkontrol.pot"
            ${XGETTEXT_CREDIT_ARGS}
            ${ALL_TRANSLATE_SOURCE_FILES}
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
    COMMAND_ERROR_IS_FATAL ANY
)

execute_process(
    WORKING_DIRECTORY "${BASE_DIRECTORY}"
    COMMAND "${XGETTEXT_EXECUTABLE}" --join-existing --language=Desktop
        "--output=po/headsetkontrol.pot"
        ${XGETTEXT_CREDIT_ARGS}
        com.thebill2001.headsetkontrol.desktop.in
        headsetkontrol.notifyrc
    ECHO_OUTPUT_VARIABLE
    ECHO_ERROR_VARIABLE
    COMMAND_ERROR_IS_FATAL ANY
)

### Merge template file into existing translations
file(GLOB_RECURSE ALL_PO_FILES "${BASE_DIRECTORY}/po/*.po")

foreach(pofile IN LISTS ALL_PO_FILES)
    cmake_path(RELATIVE_PATH pofile BASE_DIRECTORY "${BASE_DIRECTORY}" OUTPUT_VARIABLE rel_path)
    message(STATUS "Merging ${rel_path}")
    execute_process(
        WORKING_DIRECTORY "${BASE_DIRECTORY}"
        COMMAND "${GETTEXT_MSGMERGE_EXECUTABLE}" -q --update --backup=none --previous "${pofile}" "${BASE_DIRECTORY}/po/headsetkontrol.pot"
        ECHO_OUTPUT_VARIABLE
        ECHO_ERROR_VARIABLE
        COMMAND_ERROR_IS_FATAL ANY
    )
endforeach()

### Create progress report

# Count template message
file(READ "${BASE_DIRECTORY}/po/headsetkontrol.pot" template_content)
string(REGEX MATCHALL "msgstr \"\"\n(\n|$)" template_messages "${template_content}")
list(LENGTH template_messages template_message_count)

set(progress_lines
    "|  Locale  |  Lines  | % Done|"
    "|----------|---------|-------|"
    "| Template | ${template_message_count} | |"
)

foreach(pofile IN LISTS ALL_PO_FILES)
    file(READ "${pofile}" po_content)
    string(REGEX MATCHALL "msgstr \"\"\n(\n|$)" po_empty_messages "${po_content}")
    list(LENGTH po_empty_messages po_empty_messages_count)
    math(EXPR po_messages_count "${template_message_count} - ${po_empty_messages_count}")
    math(EXPR po_messages_percent "100*${po_messages_count}/${template_message_count}")

    cmake_path(RELATIVE_PATH pofile BASE_DIRECTORY "${BASE_DIRECTORY}/po" OUTPUT_VARIABLE lang_file)
    cmake_path(GET lang_file PARENT_PATH lang)
    list(APPEND progress_lines "| ${lang} | ${po_messages_count}/${template_message_count} | ${po_messages_percent}% |")
endforeach()

# Write report
string(JOIN "\n" PROGRESS_TABLE ${progress_lines})
configure_file("${BASE_DIRECTORY}/cmake/po-README.md.in" "${BASE_DIRECTORY}/po/README.md" @ONLY)
