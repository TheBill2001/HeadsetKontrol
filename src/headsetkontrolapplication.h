// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <AbstractKirigamiApplication>
#include <QQmlEngine>

using namespace Qt::StringLiterals;

class HeadsetKontrolApplication : public AbstractKirigamiApplication
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit HeadsetKontrolApplication(QObject *parent = nullptr);

Q_SIGNALS:
    void incrementCounter();

private:
    void setupActions() override;
};
