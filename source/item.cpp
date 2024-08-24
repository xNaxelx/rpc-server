#include "Item.h"

Item::Item(const QString& id, QObject* parent)
    : QObject(parent), m_id(id)
{
}

QString Item::id() const
{
    return m_id;
}

QJsonObject Item::toJson() const
{
    QJsonObject json;
    json["id"] = m_id;
    QJsonObject props;
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
        props[it.key()] = QJsonValue::fromVariant(it.value());
    }
    json["properties"] = props;
    return json;
}

void Item::fromJson(const QJsonObject& json)
{
    m_id = json["id"].toString();
    QJsonObject props = json["properties"].toObject();
    for (auto it = props.begin(); it != props.end(); ++it) {
        m_properties[it.key()] = it.value().toVariant();
    }
}

void Item::setProperty(const QString& key, const QVariant& value)
{
    if (m_properties[key] != value) {
        m_properties[key] = value;
        emit propertyChanged(key, value);
    }
}
