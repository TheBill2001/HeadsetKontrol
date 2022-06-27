# HeadsetKontrol
Interface for [HeadsetControl](https://github.com/Sapd/HeadsetControl) written with Kirigami and KDE Framework.

* Monitor headset information.
* Battery level notification.
* Set headset specific settings.

# Building and installing
The program is only tested on Linux (Arch to be specific). However, all libraries used are available on Mac OS and Windows. You might need to build them yourself, though.

## Requirement
Qt version 5.15.3 or above with these modules:

* Qt Core
* QuickControls2
* DBus

KDE Framework 5.92.0 or above with these modules:

* ECM
* Kirigami 2
* KI18n
* KCoreAddons
* KConfig
* KDBusAddons
* KNotifications

## Installing dependencies
1. Arch Linux.

   These package should pull in all needed Qt and other packages.

   ```
   pacman -S extra-cmake-modules kirigami2 ki18n kconfig kdbusaddons knotifications kcoreaddons
   ```
## Build and install
### General steps.
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

The install it.

```
make install
```

Uinstall.

```
make uninstall
```

### Platform specific.
1. Arch Linux.

   HeadsetKontrol can be install from the [AUR]().

# License
GPLV3
