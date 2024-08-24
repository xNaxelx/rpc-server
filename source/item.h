#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QJsonObject>
#include <QString>
#include <QMap>

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(const QString& id, QObject* parent = nullptr);
    QString id() const;
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);

signals:
    void propertyChanged(const QString& property, const QVariant& value);

public slots:
    void setProperty(const QString& key, const QVariant& value);

private:
    QString m_id;
    QMap<QString, QVariant> m_properties;
};

#endif // ITEM_H
