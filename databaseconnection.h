// databaseconnection.h
#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <QSqlDatabase>

class DatabaseConnection
{
public:
    static QSqlDatabase getDatabaseConnection();
};

#endif // DATABASECONNECTION_H
