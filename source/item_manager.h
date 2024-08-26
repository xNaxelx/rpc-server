#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include <QObject>
#include <QJsonArray>
#include <QMap>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include "item.h"

class ItemManager : public QObject
{
    Q_OBJECT
public:
    explicit ItemManager(QObject* parent = nullptr);

    QJsonArray getAllItems() const;
    Item* getItem(const QString& id) const;
    void addItem(Item* item);
    void removeItem(const QString& id);
    void updateItem(const QString& id, const QJsonObject& properties);

signals:
    void itemAdded(Item* item);
    void itemRemoved(const QString& id);
    void itemUpdated(const QString& id, const QJsonObject& properties);

private:
    void loadItemsFromFile();
    void saveItemsToFile() const;

    QMap<QString, Item*> m_items;
};

#endif // ITEMMANAGER_H
