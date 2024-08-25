#ifndef WEATHERFORECAST_H
#define WEATHERFORECAST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class WeatherForecast : public QObject
{
    Q_OBJECT
public:
    explicit WeatherForecast(QObject* parent = nullptr);

    void getWeather(const QString& city, const QString& requestId);

signals:
    void weatherUpdated(const QJsonObject& weatherData, const QString& requestId);
    void errorOccurred(const QString& error, const QString& requestId);

private slots:
    void onWeatherReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_networkAccessManager;
    QString m_apiKey;

    void loadApiKey();
    void createConfigFile();
};

#endif // WEATHERFORECAST_H
