// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETLISTMODEL_HPP
#define HKHEADSETLISTMODEL_HPP

#include <QAbstractListModel>

#include <QtQmlIntegration/qqmlintegration.h>

class HKHeadsetListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit HKHeadsetListModel(QObject *parent = nullptr);
};

#endif // HKHEADSETLISTMODEL_HPP
