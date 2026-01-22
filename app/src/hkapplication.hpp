// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKAPPLICATION_HPP
#define HKAPPLICATION_HPP

#include <QObjectBindableProperty>
#include <QPointer>

#include <AbstractKirigamiApplication>

class HKApplication : public AbstractKirigamiApplication
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QObject *quitAction READ quitAction CONSTANT FINAL)
    Q_PROPERTY(QWindow *window READ window WRITE setWindow BINDABLE bindableWindow NOTIFY windowChanged FINAL)
public:
    explicit HKApplication(QObject *parent = nullptr);

    [[nodiscard]] QAction *quitAction();

    [[nodiscard]] QBindable<QWindow *> bindableWindow();
    [[nodiscard]] QWindow *window() const;
    void setWindow(QWindow *window);

    bool event(QEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

Q_SIGNALS:
    void windowChanged(QWindow *);

protected:
    void setupActions() override;

private:
    Q_OBJECT_BINDABLE_PROPERTY(HKApplication, QPointer<QWindow>, m_window, &HKApplication::windowChanged)

    void detectWindow();
    void showWindow();
};

Q_MOC_INCLUDE(<QWindow>)
Q_MOC_INCLUDE(<QUrl>)

#endif // HKAPPLICATION_HPP
