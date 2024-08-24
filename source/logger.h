#pragma once

#include "qstring.h"

class Logger
{
public:
    Logger() { }
    virtual ~Logger() { }

    virtual void logDebug(const QString& message) = 0;
    virtual void logInfo(const QString& message) = 0;
    virtual void logWarning(const QString& message) = 0;
    virtual void logError(const QString& message) = 0;
};

