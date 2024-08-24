#include "item_manager.h"

ItemManager::ItemManager(QObject* parent) : QObject(parent)
{
}

QJsonArray ItemManager::getAllItems() const
{
    QJsonArray array;
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        array.append(it.value()->toJson());
    }
    return array;
}

Item* ItemManager::getItem(const QString& id) const
{
    return m_items.value(id, nullptr);
}

void ItemManager::addItem(Item* item)
{
    if (!m_items.contains(item->id())) {
        m_items[item->id()] = item;
        emit itemAdded(item);
    }
}

void ItemManager::removeItem(const QString& id)
{
    if (m_items.contains(id)) {
        Item* item = m_items.take(id);
        emit itemRemoved(id);
        delete item;
    }
}

void ItemManager::updateItem(const QString& id, const QJsonObject& properties)
{
    Item* item = getItem(id);
    if (item) {
        for (auto it = properties.begin(); it != properties.end(); ++it) {
            item->setProperty(it.key(), it.value().toVariant());
        }
        emit itemUpdated(id, properties);
    }
}
