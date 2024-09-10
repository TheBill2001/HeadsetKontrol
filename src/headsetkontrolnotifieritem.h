#ifndef HEADSETKONTROLNOTIFIERITEM_H
#define HEADSETKONTROLNOTIFIERITEM_H

#include <QQmlEngine>

#include <KStatusNotifierItem>

class KActionCollection;

class HeadsetKontrolApplication;
class HeadsetKontrolNotifierItem : public KStatusNotifierItem
{
    Q_OBJECT
public:
    HeadsetKontrolNotifierItem(HeadsetKontrolApplication *parent = nullptr);
    ~HeadsetKontrolNotifierItem();

private:
    HeadsetKontrolApplication *m_parent;

    QMenu *m_menu;
};

#endif // HEADSETKONTROLNOTIFIERITEM_H
