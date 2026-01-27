// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#ifndef HKHEADSETLISTMODEL_HPP
#define HKHEADSETLISTMODEL_HPP

#include "headsetkontrol_export.hpp"

#include <QAbstractListModel>

#include <QtQmlIntegration/qqmlintegration.h>

class HKHeadset;
class HK_NO_EXPORT HKHeadsetListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Role : quint16 {
        Headset = Qt::UserRole,
        Id,
        Name,
        Capabilities,
        Battery,
        ChatMix
    };
    Q_ENUM(Role)

    explicit HKHeadsetListModel(QObject *parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

private:
    QList<HKHeadset *> m_headsets;

    void onHeadsetsAdded(const QList<HKHeadset *> &newHeadsets);
    void onHeadsetsRemoved(const QList<HKHeadset *> &oldHeadsets);
};

#endif // HKHEADSETLISTMODEL_HPP
