# Borrow from inkscape
find_package(Gettext REQUIRED)
file(GLOB LANGUAGES *.po)
foreach(language ${LANGUAGES})
    string(REGEX REPLACE "(.+(\\\\|/))+" "" language ${language})
    string(REGEX REPLACE "\\.po$" "" language ${language})
    string(MAKE_C_IDENTIFIER "${language}" language_code_escaped)

    set(pofile ${CMAKE_CURRENT_SOURCE_DIR}/${language}.po)
    set(gmofile ${CMAKE_CURRENT_BINARY_DIR}/${language}.gmo)

    GETTEXT_PROCESS_PO_FILES(${language} ALL PO_FILES ${pofile})

    install(FILES ${gmofile}
        DESTINATION "${KDE_INSTALL_LOCALEDIR}/${language}/LC_MESSAGES/"
        RENAME ${CMAKE_PROJECT_NAME}.mo
    )
endforeach(language)
