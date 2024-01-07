// databaseconnection.cpp
#include "databaseconnection.h"
#include <QSqlError>
#include <QDebug>

QSqlDatabase DatabaseConnection::getDatabaseConnection()
{
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("C:/Users/Administrator/Desktop/GestionStock1.db");

    if (!database.open())
    {
        qDebug() << "Error opening database:" << database.lastError().text();
        // You might want to handle this error more gracefully in a production application
    }

    return database;
}
