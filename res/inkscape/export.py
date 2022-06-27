import subprocess
import os

from pathlib import Path


def inExclude(item, excludes):
    for exclude in excludes:
        if exclude in item:
            return True
    return False


def findSvgs(exclude):
    _file = []
    for files in (files for (_, _, files) in os.walk(".")):
        for file in files:
            if ".svg" in file and not inExclude(file, exclude):
                _file.append(file)
    return _file


SIZES = [1024, 512, 384, 310, 256, 192, 150,
         128, 96, 72, 64, 48, 44, 36, 32, 24, 22, 16]
EXCLUDE = ["base.svg"]
SVGS = findSvgs(EXCLUDE)

print("Export for these sizes:\n%s" % SIZES)
print("Export for these file:\n%s\n" % SVGS)

for svg in SVGS:
    print("Export file \"%s\"" % svg)

    filenames = svg.split("-")
    filePath = "../icons/" + filenames[0] + "/" + "strplchldr"

    for seg in filenames[1:-1]:
        filePath += "/" + seg

    for size in SIZES:
        sizeStr = str(size)
        _filePath = filePath.replace("strplchldr", sizeStr + "x" + sizeStr)

        Path(_filePath).mkdir(parents=True, exist_ok=True)

        _fileoutname = _filePath + "/" + filenames[-1].split(".")[0].replace("_", "-") + ".png"

        print("    Size %8d: \"%s\"" % (size, _fileoutname))

        subprocess.run(["inkscape", "--export-filename=" + _fileoutname, "--export-area-page",
                       "--export-width=" + sizeStr, "--export-height=" + sizeStr, "--export-overwrite", svg])

    filePath = filePath.replace("strplchldr", "scalable")

    Path(filePath).mkdir(parents=True, exist_ok=True)

    fileoutname = filePath + "/" + filenames[-1].replace("_", "-")

    print("    Size scalable: \"%s\"" % fileoutname)

    subprocess.run(["inkscape", "--export-filename=" + fileoutname, "--export-area-page",
                    "--export-width=" + sizeStr, "--export-height=" + sizeStr, "--export-overwrite", svg])
