# HeadsetKontrol
Interface for [HeadsetControl](https://github.com/Sapd/HeadsetControl) written with Kirigami and KDE Framework.

* Monitor headset information.
* Battery level notification.
* Set headset specific settings.

![Screenshot](/uploads/c9f8ce9af82116dff5e155fece36571f/Screenshot_20230301_034343.png)

# Installing
1. Arch Linux

   HeadsetKontrol can be install from the [AUR](https://aur.archlinux.org/packages/headsetkontrol).

# Building
The program is only tested on Linux (Arch to be specific). However, all libraries used are available on Mac OS and Windows. You might need to build them yourself, though.

## Requirement
Qt version 5.15.3 or above with these modules:

* Qt Quick
* Qt QuickControls2
* Qt DBus

KDE Framework 5.92.0 or above with these modules:

* ECM (for make)
* Kirigami 2
* KI18n
* KCoreAddons
* KConfig
* KDBusAddons
* KNotifications

## Installing dependencies
1. Arch Linux

   ```
   pacman -S extra-cmake-modules kirigami2 ki18n kconfig kdbusaddons knotifications kcoreaddons
   ```

   These packages should pull in all needed Qt and other packages.

## Build steps
Create and enter build directory.

```
mkdir build
cd build
```

Build the program.

```
cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE=Release ..
make
```

## Install

```
make install
```

## Unstall.

```
make uninstall
```

# Development

Feel free to report bugs and open pull request to any improvement. Do note that I am still learning and can be quite slow sometime.

[Translation progress](translation/Progress.md) can be view here. To add a new translation, copy the `template.pot` file and rename it to `<language>.po`. The language name must follow i18n language codes.

# License
The program is licensed under [GPL V3](LICENSE)
