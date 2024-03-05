# NOTE: Rewriting for Plasma 6 in progress. I'm very busy at the moment, so bear with me.

# HeadsetKontrol

Interface for [HeadsetControl](https://github.com/Sapd/HeadsetControl) written with Kirigami and KDE Framework.

* Monitor headset information.
* Battery level notification.
* Set headset specific settings.

![Screenshot](https://gitlab.com/TheBill2001/HeadsetKontrol/uploads/c9f8ce9af82116dff5e155fece36571f/Screenshot_20230301_034343.png)

## 1. Installing
1. Arch Linux

   HeadsetKontrol can be install from the [AUR](https://aur.archlinux.org/packages/headsetkontrol).

## 2. Running
### Access without root

Appropriate udev rules need to set for accessing without root. From [HeadsetControl instruction](https://github.com/Sapd/HeadsetControl#access-without-root):

>Also in Linux, you need udev rules if you don't want to start the application with root. Those rules are generated via `headsetcontrol -u`. Typing `make install` on Linux generates and writes them automatically to /etc/udev/rules.d/.
>
>You can reload udev configuration without reboot via `sudo udevadm control --reload-rules && sudo udevadm trigger`

## 3. Building
### Requirement
Qt version 5.15.3 or above with these modules:

* Qt Quick
* Qt QuickControls2
* Qt DBus

KDE Framework 5.92.0 or above with these modules:

* ECM (for CMake)
* Kirigami 2
* KI18n
* KCoreAddons
* KConfig
* KDBusAddons
* KNotifications

### Installing dependencies
1. Arch Linux

   ```
   pacman -S extra-cmake-modules kirigami2 ki18n kconfig kdbusaddons knotifications kcoreaddons
   ```

2. Fedora Linux

   ```
   sudo dnf install extra-cmake-modules kf5-kirigami2-devel kf5-ki18n-devel kf5-kconfig-devel kf5-kdbusaddons-devel kf5-knotifications-devel kf5-kcoreaddons-devel qt5-qtbase-devel
   ```

These packages should pull in all needed Qt and other packages.

### Build steps
- Create and enter build directory.

   ```
   mkdir build
   cd build
   ```

- Build the program.

   ```
   cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE=Release ..
   make
   ```

### Install

   ```
   make install
   ```

### Uninstall.
   ```
   make uninstall
   ```

## 4. Development

Feel free to report bugs and open pull request to any improvement. Do note that I am still learning and can be quite slow sometime.

[Translation progress](translation/Progress.md) can be view here. To add a new translation, copy the `template.pot` file and rename it to `<language>.po`. The language name must follow [i18n language codes](https://i18ns.com/languagecode.html).

## 5. License
The program is licensed under [GPL V3](LICENSE)
