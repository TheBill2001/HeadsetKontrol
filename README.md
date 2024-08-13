# HeadsetKontrol

Interface for [HeadsetControl](https://github.com/Sapd/HeadsetControl) written with Kirigami and KDE Framework.

* Monitor headset information.
* Battery level notification.
* Set headset specific settings.

![Screenshot](https://gitlab.com/TheBill2001/HeadsetKontrol/uploads/c9f8ce9af82116dff5e155fece36571f/Screenshot_20230301_034343.png)

# Rewriting for Plasma 6

- [x] Refactoring CMake and project structure for ECM 6.0.0
- [ ] Porting to Qt6/KF6 - UI
   - [x] Porting Kirigami.CategorizedConfig to KirigamiAddons.ConfigurationView
   - [x] Add devices list page
   - [ ] Porting device configuration page to KirigamiAddons.FormCard

- [ ] Porting HeadsetControl wrapper class to new output format.
   - [x] Query actions.
   - [ ] Configuring actions.
      - [ ] Save per-device configuration (might skip, HeadsetControl currently doesn't support this).

- [ ] Rewrite notifications
- [ ] Update icons (might skip)
- [ ] Update README images

## 1. Installing
1. Arch Linux

   HeadsetKontrol can be install from the [AUR](https://aur.archlinux.org/packages/headsetkontrol).

## 2. Building
### 2.1 Requirement
Qt version 6.5.0 or above with these modules:

* Core
* Gui
* Quick
* Qml
* QuickControls2
* Widgets
* DBus

KDE Framework 6.0.0 or above with these modules:

* ECM (for CMake)
* Kirigami
* KirigamiAddons
* I18n
* CoreAddons
* Config
* DBusAddons
* Notifications
* IconThemes

### 2.2 Installing dependencies

_TODO_

### 2.3 Build steps
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

### 2.4 Install

   ```
   make install
   ```

### 2.5 Uninstall.
   ```
   make uninstall
   ```

## 3. Development

Feel free to report bugs and open pull request to any improvement. Do note that I am still learning and can be quite slow sometime.

[Translation progress](translation/Progress.md) can be view here. To add a new translation, copy the `template.pot` file and rename it to `<language>.po`. The language name must follow [i18n language codes](https://i18ns.com/languagecode.html).

## 4. License
The program is licensed under [GPL V3](LICENSE)
