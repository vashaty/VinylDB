#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QtDebug>
#include <QFileInfo>
#include <QMessageBox>

class Database
{
public:
    Database();
    QSqlDatabase db;
    void connClose();
    bool connOpen();

};

#endif // DATABASE_H
