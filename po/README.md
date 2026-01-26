# Translation

## Status
|  Locale  |  Lines  | % Done|
|----------|---------|-------|
| Template | 56 | |
| vi | 56/56 | 100% |

## New translations

One could manually fill out the `headsetkontrol.pot` template or using tools such as [Lokalize](https://apps.kde.org/lokalize/).
Either open a new issue with the translated file attached, or create a merge request yourself.
The file should be named `headsetkontrol.po` and placed in a subfolder with your language code as name, i.e. `en_US/headsetkontrol.po`.
The language codes for various languages can be found [here](https://i18ns.com/languagecode.html), with dashes replaced with underscores.

## Extracting and merging

During development, new messages can be extracted and merged with existing translations using CMake.
Configure the project with CMake as usual and invoke the target `potfile-merge`. `xgettext` executable must be found in `PATH`.
Otherwise, `XGETTEXT_EXECUTABLE` CMake variable must be set during configuration.
