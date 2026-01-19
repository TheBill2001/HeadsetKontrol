// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2026 Trần Nam Tuấn <tuantran1632001@gmail.com>

#include "headsetkontrolapplication.h"
#include <KAuthorized>
#include <KLocalizedString>

using namespace Qt::StringLiterals;

HeadsetKontrolApplication::HeadsetKontrolApplication(QObject *parent)
    : AbstractKirigamiApplication(parent)
{
    setupActions();
}

void HeadsetKontrolApplication::setupActions()
{
    AbstractKirigamiApplication::setupActions();

    auto actionName = "increment_counter"_L1;
    if (KAuthorized::authorizeAction(actionName)) {
        auto action = mainCollection()->addAction(actionName, this, &HeadsetKontrolApplication::incrementCounter);
        action->setText(i18nc("@action:inmenu", "Increment"));
        action->setIcon(QIcon::fromTheme(u"list-add-symbolic"_s));
        KirigamiActionCollection::setDefaultShortcut(action, Qt::CTRL | Qt::Key_I);
    }

    readSettings();
}

#include "moc_headsetkontrolapplication.cpp"
