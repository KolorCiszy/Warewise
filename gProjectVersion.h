#ifndef GPROJECTVERSION_H
#define GPROJECTVERSION_H

#include <QString>
#define APP_MAJOR_VERSION 0
#define APP_MINOR_VERSION 1

static const QString& GetVersionAsString()
{
    static QString VersionString = QString("Wersja %1.%2").arg(APP_MAJOR_VERSION).arg(APP_MINOR_VERSION);
    return VersionString;
};
#endif // GPROJECTVERSION_H
