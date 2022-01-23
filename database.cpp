#include "database.h"

Database::Database()
{

//    //Add database
//        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//        //Connection data
//        db.setDatabaseName("database.db"); //Set the name of the database to be used
//        //Open the database
//        if(!db.open())
//        {
//            QMessageBox::warning(this, "error", db.lastError().text());
//            return ;//Open failed
//        }
//        QSqlQuery query;
//        query.exec("CREATE TABLE IF NOT EXISTS  vinyl (id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                   "artist VARCHAR(30) NOT NULL,"
//                   "albumName VARCHAR(50) NOT NULL,"
//                   "year INTEGER NOT NULL,"
//                   "songsCount INTEGER NOT NULL,"
//                   "genre VARCHAR(20),"
//                   "imageSrc VARCHAR(50))");
//    //    for (int i = 0; i< 3; ++i)
//    //    {
//    //        query.prepare("INSERT INTO people (id, name, age) VALUES (:id, :name, :age)");
//    //        query.bindValue(":name", QString("smith_%1").arg(i+1));
//    //        query.bindValue(":age", 20+i*5);
//    //        query.exec();
//    //    }
////        if (query.exec("insert into vinyl(artist, albumName, year, songsCount) values('ahoj', 'ahoj', 125, 12);"))
////        {
////            qDebug()<<"insert success/n";
////        }
////        else{
////            qDebug()<<"insert failed";
////        }
////        query.exec("SELECT * FROM vinyl");
////        while (query.next())//The result of executing the search statement
////        {
////            qDebug()<<"people:"<<query.value(0).toInt()<<"    name:"<<query.value(1).toString()<<"  age:"<<query.value(2).toInt();
////        }
//        db.close();
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
