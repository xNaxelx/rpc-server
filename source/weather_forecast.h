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

    void getWeather(const QString& city);

signals:
    void weatherUpdated(const QJsonObject& weatherData);
    void errorOccurred(const QString& error);

private slots:
    void onWeatherReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager m_networkAccessManager;
    QString m_apiKey;

    void loadApiKey();
    void createConfigFile();
};

#endif // WEATHERFORECAST_H
