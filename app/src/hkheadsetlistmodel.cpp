// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>
// SPDX-License-Identifier: GPL-3.0-later

#include "hkheadsetlistmodel.hpp"

#include "hkbattery.hpp" // IWYU pragma: keep
#include "hkchatmix.hpp" // IWYU pragma: keep
#include "hkheadset.hpp"
#include "hkheadsetcontrol.hpp"
#include "hkheadsetid.hpp" // IWYU pragma: keep

using namespace Qt::StringLiterals;

HKHeadsetListModel::HKHeadsetListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    connect(HKHeadsetControl::instance(), &HKHeadsetControl::headsetsAdded, this, &HKHeadsetListModel::onHeadsetsAdded);
    connect(HKHeadsetControl::instance(), &HKHeadsetControl::headsetsRemoved, this, &HKHeadsetListModel::onHeadsetsRemoved);

    onHeadsetsAdded(HKHeadsetControl::headsets());
}

int HKHeadsetListModel::rowCount(const QModelIndex & /*parent*/) const
{
    return int(m_headsets.size());
}

QVariant HKHeadsetListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > m_headsets.size()) {
        return {};
    }

    auto *headset = m_headsets.at(index.row());
    switch (role) {
    case Headset:
        return QVariant::fromValue(headset);
    case Id:
        return QVariant::fromValue(headset->id());
    case Name:
        return headset->name();
    case Capabilities:
        return QVariant::fromValue<HKHeadset::Capabilities::Int>(headset->capabilities());
    case Battery:
        return QVariant::fromValue(headset->battery());
    case ChatMix:
        return QVariant::fromValue(headset->chatMix());
    default:
        break;
    }
    return {};
}

QHash<int, QByteArray> HKHeadsetListModel::roleNames() const
{
    return {{Headset, "headset"_ba},
            {Id, "headsetId"_ba},
            {Name, "headsetName"_ba},
            {Capabilities, "headsetCapabilities"_ba},
            {Battery, "headsetBattery"_ba},
            {ChatMix, "headsetChatMix"_ba}};
}

void HKHeadsetListModel::onHeadsetsAdded(const QList<HKHeadset *> &newHeadsets)
{
    if (newHeadsets.isEmpty()) {
        return;
    }

    beginResetModel();
    m_headsets << newHeadsets;
    std::ranges::sort(m_headsets, [](HKHeadset *lhs, HKHeadset *rhs) {
        return QString::localeAwareCompare(lhs->name(), rhs->name()) < 0;
    });
    endResetModel();
}

void HKHeadsetListModel::onHeadsetsRemoved(const QList<HKHeadset *> &oldHeadsets)
{
    if (oldHeadsets.isEmpty()) {
        return;
    }

    for (auto *oldHeadset : oldHeadsets) {
        while (m_headsets.contains(oldHeadset)) {
            const auto index = m_headsets.indexOf(oldHeadset);
            beginRemoveRows({}, int(index), int(index));
            m_headsets.removeOne(oldHeadset);
            endRemoveRows();
        }
    }
}
