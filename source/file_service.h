#pragma once

#include "qfile.h"
#include "qjsondocument.h"

class FileService
{
	bool OpenJsonFile(QJsonDocument& doc, QString fileName);
	bool WriteJsonFile(QJsonDocument& doc, QString fileName);
};
