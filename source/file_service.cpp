#include "file_service.h"

bool FileService::OpenJsonFile(QJsonDocument& doc, QString fileName)
{
    if (!fileName.endsWith(".json", Qt::CaseInsensitive))
    {
        qWarning("Incorrect file format.");
        return false;
    }

    QFile loadFile(fileName);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    doc = QJsonDocument::fromJson(saveData);

    return true;
}

bool FileService::WriteJsonFile(QJsonDocument& doc, QString fileName)
{
    if (!fileName.endsWith(".json", Qt::CaseInsensitive))
    {
        qWarning("Incorrect file format.");
        return false;
    }

    QFile saveFile(fileName);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    saveFile.write(doc.toJson());

    return true;
}