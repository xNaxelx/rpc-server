#include "Server.h"
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonValue>

Server::Server()
    : m_server(new QWebSocketServer(QStringLiteral("JSON-RPC Server"),
        QWebSocketServer::NonSecureMode, this)),
    m_availableMethods({ "getAllItems", "getItem", "createItem", "updateItem", "deleteItem",
                        "add", "subtract", "multiply", "divide",
                        "getWeather" })
{
    connect(m_server, &QWebSocketServer::newConnection,
        this, &Server::onNewConnection);

    // Создание WeatherForecast без ключа API
    m_weatherForecast = new WeatherForecast(this);

    connect(m_weatherForecast, &WeatherForecast::weatherUpdated,
        this, &Server::handleWeatherUpdate);
    connect(m_weatherForecast, &WeatherForecast::errorOccurred,
        this, &Server::handleWeatherError);
}

Server::~Server()
{
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void Server::startServer()
{
    if (m_server->listen(QHostAddress::Any, 1234)) {
        qDebug() << "Server listening on port 1234 " + m_server->serverAddress().toString();
    }
    else {
        qDebug() << "Error: " << m_server->errorString();
    }
}

void Server::onNewConnection()
{
    QWebSocket* socket = m_server->nextPendingConnection();
    connect(socket, &QWebSocket::textMessageReceived,
        this, &Server::handleRequest);
    connect(socket, &QWebSocket::disconnected,
        this, &Server::onSocketDisconnected);
    m_clients.append(socket);
}

void Server::handleRequest(const QString& requestString)
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    QJsonDocument requestDoc = QJsonDocument::fromJson(requestString.toUtf8());
    if (requestDoc.isArray()) {
        // Обработка batch-запроса
        QJsonArray requests = requestDoc.array();
        QJsonArray responses;
        for (const QJsonValue& value : requests) {
            if (value.isObject()) {
                QJsonObject request = value.toObject();
                QJsonObject response = processRequest(request);
                responses.append(response);
            }
        }
        QJsonDocument responseDoc(responses);
        socket->sendTextMessage(responseDoc.toJson(QJsonDocument::Compact));
    }
    else if (requestDoc.isObject()) {
        // Обработка одиночного запроса
        QJsonObject request = requestDoc.object();
        QJsonObject response = processRequest(request);
        QJsonDocument responseDoc(response);
        socket->sendTextMessage(responseDoc.toJson(QJsonDocument::Compact));
    }
    else {
        // Неверный формат запроса
        QJsonObject errorResponse;
        errorResponse["jsonrpc"] = "2.0";
        errorResponse["error"] = QJsonObject{ {"code", -32700}, {"message", "Parse error"} };
        errorResponse["id"] = QJsonValue::Null;
        QJsonDocument responseDoc(errorResponse);
        socket->sendTextMessage(responseDoc.toJson(QJsonDocument::Compact));
    }
}

QJsonObject Server::processRequest(const QJsonObject& request)
{
    QString method = request["method"].toString();
    QJsonObject params = request["params"].toObject();
    QJsonObject response;

    if (!m_availableMethods.contains(method)) {
        response["jsonrpc"] = "2.0";
        response["error"] = QJsonObject{ {"code", -32601}, {"message", "Method not found"} };
        response["id"] = request["id"];
    }
    else {
        if (method == "getAllItems") {
            response["jsonrpc"] = "2.0";
            response["result"] = m_itemManager.getAllItems();
            response["id"] = request["id"];
        }
        else if (method == "getItem") {
            QString id = params["id"].toString();
            Item* item = m_itemManager.getItem(id);
            if (item) {
                response["jsonrpc"] = "2.0";
                response["result"] = item->toJson();
                response["id"] = request["id"];
            }
            else {
                response["jsonrpc"] = "2.0";
                response["error"] = QJsonObject{ {"code", -32602}, {"message", "Item not found"} };
                response["id"] = request["id"];
            }
        }
        else if (method == "createItem") {
            QString id = params["id"].toString();
            Item* item = new Item(id);
            item->fromJson(params["properties"].toObject());
            m_itemManager.addItem(item);
            response["jsonrpc"] = "2.0";
            response["result"] = "Item created";
            response["id"] = request["id"];
        }
        else if (method == "updateItem") {
            QString id = params["id"].toString();
            m_itemManager.updateItem(id, params["properties"].toObject());
            response["jsonrpc"] = "2.0";
            response["result"] = "Item updated";
            response["id"] = request["id"];
        }
        else if (method == "deleteItem") {
            QString id = params["id"].toString();
            m_itemManager.removeItem(id);
            response["jsonrpc"] = "2.0";
            response["result"] = "Item deleted";
            response["id"] = request["id"];
        }
        else if (method == "add") {
            double a = params["a"].toDouble();
            double b = params["b"].toDouble();
            response["jsonrpc"] = "2.0";
            response["result"] = m_calculator.add(a, b);
            response["id"] = request["id"];
        }
        else if (method == "subtract") {
            double a = params["a"].toDouble();
            double b = params["b"].toDouble();
            response["jsonrpc"] = "2.0";
            response["result"] = m_calculator.subtract(a, b);
            response["id"] = request["id"];
        }
        else if (method == "multiply") {
            double a = params["a"].toDouble();
            double b = params["b"].toDouble();
            response["jsonrpc"] = "2.0";
            response["result"] = m_calculator.multiply(a, b);
            response["id"] = request["id"];
        }
        else if (method == "divide") {
            double a = params["a"].toDouble();
            double b = params["b"].toDouble();
            try {
                response["jsonrpc"] = "2.0";
                response["result"] = m_calculator.divide(a, b);
            }
            catch (const std::invalid_argument& e) {
                response["jsonrpc"] = "2.0";
                response["error"] = QJsonObject{ {"code", -32602}, {"message", e.what()} };
            }
            response["id"] = request["id"];
        }
        else if (method == "getWeather") {
            QString city = params["city"].toString();
            QString id = request["id"].toString();
            // Устанавливаем идентификатор запроса
            response["jsonrpc"] = "2.0";
            response["result"] = "Weather request sent";
            response["id"] = id;
            m_weatherForecast->getWeather(city);  // Передаем идентификатор
            return response;  // Возвращаем ответ сразу, чтобы обработка batch-запросов продолжалась
        }
    }

    return response;
}

void Server::handleWeatherUpdate(const QJsonObject& weatherData)
{
    for (QWebSocket* socket : m_clients) {
        QJsonObject response;
        response["jsonrpc"] = "2.0";
        response["result"] = weatherData;
        socket->sendTextMessage(QJsonDocument(response).toJson(QJsonDocument::Compact));
    }
}

void Server::handleWeatherError(const QString& error)
{
    for (QWebSocket* socket : m_clients) {
        QJsonObject response;
        response["jsonrpc"] = "2.0";
        response["error"] = QJsonObject{ {"code", -32000}, {"message", error} };
        socket->sendTextMessage(QJsonDocument(response).toJson(QJsonDocument::Compact));
    }
}

void Server::onSocketDisconnected()
{
    QWebSocket* socket = qobject_cast<QWebSocket*>(sender());
    if (socket) {
        m_clients.removeAll(socket);
        socket->deleteLater();
    }
}
