// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKAPPLICATION_HPP
#define HKAPPLICATION_HPP

#include "headsetkontrol_export.hpp"

#include <QObjectBindableProperty>
#include <QPointer>

#include <AbstractKirigamiApplication>

class QQuickWindow;
class KStatusNotifierItem;

class HK_NO_EXPORT HKApplication : public AbstractKirigamiApplication
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QObject *startAction READ startAction CONSTANT FINAL)
    Q_PROPERTY(QObject *stopAction READ stopAction CONSTANT FINAL)
    Q_PROPERTY(QObject *refreshAction READ refreshAction CONSTANT FINAL)
    Q_PROPERTY(QObject *quitAction READ quitAction CONSTANT FINAL)
    Q_PROPERTY(QObject *commandBarAction READ commandBarAction CONSTANT FINAL)
    Q_PROPERTY(QObject *aboutAppAction READ aboutAppAction CONSTANT FINAL)
    Q_PROPERTY(QObject *keyBindingsAction READ keyBindingsAction CONSTANT FINAL)
    Q_PROPERTY(QObject *configureAction READ configureAction CONSTANT FINAL)
    Q_PROPERTY(QObject *reportBugAction READ reportBugAction CONSTANT FINAL)
    Q_PROPERTY(QQuickWindow *window READ window WRITE setWindow BINDABLE bindableWindow NOTIFY windowChanged FINAL)
public:
    explicit HKApplication(QObject *parent = nullptr);
    ~HKApplication() override;

    [[nodiscard]] QAction *startAction();
    [[nodiscard]] QAction *stopAction();
    [[nodiscard]] QAction *refreshAction();
    [[nodiscard]] QAction *quitAction();
    [[nodiscard]] QAction *commandBarAction();
    [[nodiscard]] QAction *aboutAppAction();
    [[nodiscard]] QAction *keyBindingsAction();
    [[nodiscard]] QAction *configureAction();
    [[nodiscard]] QAction *reportBugAction();

    [[nodiscard]] QBindable<QQuickWindow *> bindableWindow();
    [[nodiscard]] QQuickWindow *window() const;
    void setWindow(QQuickWindow *window);

    QList<KirigamiActionCollection *> actionCollections() const override;

    bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
    void windowChanged(QQuickWindow *);

protected:
    void setupActions() override;

private:
    KirigamiActionCollection *m_headsetControlActionCollection = nullptr;

    QQuickWindow *m_previousWindow = nullptr;
    Q_OBJECT_BINDABLE_PROPERTY(HKApplication, QQuickWindow *, m_window, &HKApplication::windowChanged)

    KStatusNotifierItem *m_statusNotifierItem = nullptr;

    void onWindowChanged();
    void showWindow();
    void setupStatusNotifierItem();
};

Q_MOC_INCLUDE(<QQuickWindow>)
Q_MOC_INCLUDE(<QUrl>)

#endif // HKAPPLICATION_HPP
