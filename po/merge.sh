#!/usr/bin/env bash

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
PROJECT_DIR=$(dirname -- "${SCRIPT_DIR}")
PROJECT_VERSION=$(grep -A1  "project(HeadsetKontrol" "${PROJECT_DIR}/CMakeLists.txt" | grep VERSION | awk '{print $2}')

pushd "${PROJECT_DIR}" > /dev/null

XGETTEXT_CREDIT_ARGS=(
    --package-name=headsetkontrol
    --package-version="${VERSION}"
    --copyright-holder='Trần Nam Tuấn'
    --msgid-bugs-address='tuantran1632001@gmail.com'
)

XGETTEXT_COMMON_ARGS=(
    "${XGETTEXT_CREDIT_ARGS[@]}"
    --width=160
    --add-location=file
)

PROGRESS_TABLE=(
    "|  Locale  |  Lines  | % Done|"
    "|----------|---------|-------|"
)

PROGRESS_ENTRY_FORMAT="| %-8s | %7s | %5s |"

DESKTOP_FILES=(
    com.thebill2001.headsetkontrol.desktop.in
    headsetkontrol.notifyrc
)

mapfile -d '' SOURCE_FILES < <(find src \( -name "*.cpp" -o -name "*.hpp" -o -name "*.qml" \) -print0 | sort -z)

mapfile -d '' PO_FILES < <(find po -name "*.po" -print0 | sort -z)

function extract_source {
    for file in "$@"; do
        if [[ -f "${SCRIPT_DIR}/headsetkontrol.pot" ]]; then
            join_existing=--join-existing
        else
            join_existing=""
        fi

        echo "-- Extracting: $(realpath -- "${file}")"

        xgettext "${XGETTEXT_COMMON_ARGS[@]}" --c++ --kde --from-code=UTF-8 -c=i18n ${join_existing} \
            -ki18n:1 -ki18nc:1c,2 -ki18np:1,2 -ki18ncp:1c,2,3 \
            -ki18nd:2 -ki18ndc:2c,3 -ki18ndp:2,3 -ki18ndcp:2c,3,4 \
            -kki18n:1 -kki18nc:1c,2 -kki18np:1,2 -kki18ncp:1c,2,3 \
            -kki18nd:2 -kki18ndc:2c,3 -kki18ndp:2,3 -kki18ndcp:2c,3,4 \
            -kxi18n:1 -kxi18nc:1c,2 -kxi18np:1,2 -kxi18ncp:1c,2,3 \
            -kxi18nd:2 -kxi18ndc:2c,3 -kxi18ndp:2,3 -kxi18ndcp:2c,3,4 \
            -kkxi18n:1 -kkxi18nc:1c,2 -kkxi18np:1,2 -kkxi18ncp:1c,2,3 \
            -kkxi18nd:2 -kkxi18ndc:2c,3 -kkxi18ndp:2,3 -kkxi18ndcp:2c,3,4 \
            -kkli18n:1 -kkli18nc:1c,2 -kkli18np:1,2 -kkli18ncp:1c,2,3 \
            -kklxi18n:1 -kklxi18nc:1c,2 -kklxi18np:1,2 -kklxi18ncp:1c,2,3 \
            -kI18N_NOOP:1 -kI18NC_NOOP:1c,2 \
            -kI18N_NOOP2:1c,2 -kI18N_NOOP2_NOSTRIP:1c,2 \
            -ktr2i18n:1 -ktr2xi18n:1 \
            --output="${SCRIPT_DIR}/headsetkontrol.pot" \
            "${file}"
    done
}

function extract_desktop {
    for file in "$@"; do
        if [[ -f "${SCRIPT_DIR}/headsetkontrol.pot" ]]; then
            join_existing=--join-existing
        else
            join_existing=""
        fi

        echo "-- Extracting: $(realpath -- "${file}")"

        xgettext "${XGETTEXT_COMMON_ARGS[@]}" --language=Desktop ${join_existing} \
            --output="${SCRIPT_DIR}/headsetkontrol.pot" \
            "${file}"
    done
}

rm -f "${SCRIPT_DIR}/headsetkontrol.pot"

echo "Extract .desktop files..."

extract_desktop "${DESKTOP_FILES[@]}"

echo -e "\nExtract source files..."

extract_source "${SOURCE_FILES[@]}"

sed -i 's/"Content-Type: text\/plain; charset=CHARSET\\n"/"Content-Type: text\/plain; charset=UTF-8\\n"/' "${SCRIPT_DIR}/headsetkontrol.pot"
sed -i 's/# SOME DESCRIPTIVE TITLE./# Translation of HeadsetKontrol in LANGUAGE./' "${SCRIPT_DIR}/headsetkontrol.pot"
sed -i 's/# Copyright (C) YEAR Trần Nam Tuấn/'"# Copyright (C) $(date +%Y)"' Trần Nam Tuấn/' "${SCRIPT_DIR}/headsetkontrol.pot"

potMessageCount=$(grep -Pzo 'msgstr ""\n(\n|$)' "${SCRIPT_DIR}/headsetkontrol.pot" | grep -c 'msgstr ""')
PROGRESS_TABLE+=("$(printf "${PROGRESS_ENTRY_FORMAT}" "Template" "${potMessageCount}" "")")

echo -e "\nMerging translation files..."

for po_file in "${PO_FILES[@]}"; do
    echo "-- Merging: ${po_file}"

    locale=$(basename -- "$(dirname -- "${po_file}")")

    msgmerge  --width=160 \
        --add-location=file \
        --no-fuzzy-matching \
        -q --update --backup=none \
        --previous "${po_file}" \
        "${SCRIPT_DIR}/headsetkontrol.pot"

    sed -i 's/"Content-Type: text\/plain; charset=CHARSET\\n"/"Content-Type: text\/plain; charset=UTF-8\\n"/' "${po_file}"
    sed -i 's/# SOME DESCRIPTIVE TITLE./# Translation of HeadsetKontrol in '"${locale}"'./' "${po_file}"
    sed -i 's/# Copyright (C) YEAR THE PACKAGE'"'"'S COPYRIGHT HOLDER/'"# Copyright (C) $(date +%Y)"' Trần Nam Tuấn/' "${po_file}"

    poEmptyMessageCount=$(grep -Pzo 'msgstr ""\n(\n|$)' "${po_file}" | grep -c 'msgstr ""')
    poMessagesDoneCount=$(($potMessageCount - $poEmptyMessageCount))
    poCompletion=$(($poMessagesDoneCount * 100 / $potMessageCount))
    PROGRESS_TABLE+=("$(printf "${PROGRESS_ENTRY_FORMAT}" "${locale}" "${poMessagesDoneCount}/${potMessageCount}" "$(printf '%d%%' "${poCompletion}")")")
done

echo "$(printf "%s\n" "${PROGRESS_TABLE[@]}")" >> progress_table.tmp
sed -e '/@PROGRESS_TABLE@/{r progress_table.tmp' -e 'd}' "${SCRIPT_DIR}/README.md.in" > "${SCRIPT_DIR}/README.md"
rm progress_table.tmp

echo -e "\nUpdate .desktop files..."

if [[ -d po_tmp ]]; then
    rm -rf po_tmp
fi
mkdir po_tmp

for po_file in "${PO_FILES[@]}"; do
    locale=$(basename -- "$(dirname -- "${po_file}")")
    
    cp "${po_file}" "po_tmp/${locale}.po"
    echo "${locale}" >> "po_tmp/LINGUAS"
done

for file in "${DESKTOP_FILES[@]}"; do
    echo "-- Updating: $(realpath -- "${file}")"

    sed '/^Name\[/ d; /^GenericName\[/ d; /^Comment\[/ d; /^Keywords\[/ d' "${file}" > "${file}.stripped"

    msgfmt --desktop --template="${file}.stripped" \
	    -d "po_tmp" \
	    -o "${file}.new"

    mv "${file}.new" "${file}"
    rm "${file}.stripped"
done

rm -rf po_tmp

popd > /dev/null
