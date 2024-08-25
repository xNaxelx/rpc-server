#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "item_manager.h"
#include "calculator.h"
#include "weather_forecast.h"

class Server : public QObject
{
    Q_OBJECT
public:
    Server();
    ~Server();

    void startServer();

private slots:
    void onNewConnection();
    void onSocketDisconnected();
    void handleWeatherUpdate(const QJsonObject& weatherData);
    void handleWeatherError(const QString& error);

private:
    QWebSocketServer* m_server;
    QList<QWebSocket*> m_clients;
    ItemManager m_itemManager;
    Calculator m_calculator;
    WeatherForecast* m_weatherForecast;  // ќбратите внимание на использование указател€

    QStringList m_availableMethods;

    void loadData();
    void saveData(); 
    void handleRequest(const QString& requestString);
    QJsonObject processRequest(const QJsonObject& request);
};

#endif // MYSERVER_H
