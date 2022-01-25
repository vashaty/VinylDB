#include "database.h"

Database::Database()
{

}
void Database::connClose()
{
    db.close();
}
bool Database::connOpen(){
            db = QSqlDatabase::addDatabase("QSQLITE");
            //Connection data
            db.setDatabaseName("database.db"); //Set the name of the database to be used
            //Open the database
            if(!db.open())
            {
                qDebug() <<("Failed to open the database...");
                return false;//Open failed
            }
            QSqlQuery query;
            query.exec("CREATE TABLE IF NOT EXISTS  vinyl (id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "artist VARCHAR(30) NOT NULL,"
                       "albumName VARCHAR(50) NOT NULL,"
                       "year INTEGER NOT NULL,"
                       "songsCount INTEGER NOT NULL,"
                       "genre VARCHAR(20),"
                       "imageSrc VARCHAR(50))");
            return true;
}
