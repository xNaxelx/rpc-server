#include "weather_forecast.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrlQuery>

WeatherForecast::WeatherForecast(QObject* parent)
    : QObject(parent)
{
    // Загрузка ключа API из JSON файла
    QFile file("config.json");
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject json = doc.object();
        m_apiKey = json["weatherApiKey"].toString();
        if (m_apiKey.isEmpty()) {
            qWarning() << "API key is missing in config.json";
        }
    }
    else {
        qWarning() << "Failed to open config.json";
    }

    connect(&m_networkAccessManager, &QNetworkAccessManager::finished,
        this, &WeatherForecast::onWeatherReplyFinished);
}

void WeatherForecast::getWeather(const QString& city)
{
    if (m_apiKey.isEmpty()) {
        emit errorOccurred("API key is not set");
        return;
    }

    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery query;
    query.addQueryItem("q", city);
    query.addQueryItem("appid", m_apiKey);
    query.addQueryItem("units", "metric");  // Выбор единиц измерения температуры
    url.setQuery(query);

    QNetworkRequest request(url);
    m_networkAccessManager.get(request);
}

void WeatherForecast::onWeatherReplyFinished(QNetworkReply* reply)
{
    if (reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            emit weatherUpdated(doc.object());
        }
        else {
            emit errorOccurred(reply->errorString());
        }
        reply->deleteLater();
    }
}
