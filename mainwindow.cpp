#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonLoad_clicked()
{
    Database conn;
    QSqlQueryModel * model = new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery * qry = new QSqlQuery(conn.db);

    qry->prepare("select * from vinyl");

    qry->exec();
    model->setQuery(*qry);
    ui->tableView->setModel(model);

    conn.connClose();
    qDebug() << (model->rowCount());

}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QString selected= index.sibling(index.row(), 0).data().toString();
    Database conn;
    if(!conn.connOpen()){
        qDebug()<<"Failed to open database";
        return;
    }

    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("select * from vinyl where id='"+selected+"'");

    if(qry.exec()){
        while(qry.next()){
            ui->lineEditId->setText(qry.value(0).toString());
            ui->lineEditArtist->setText(qry.value(1).toString());
            ui->lineEditAlbum->setText(qry.value(2).toString());
            ui->lineEditYear->setText(qry.value(3).toString());
            ui->lineEditSongs->setText(qry.value(4).toString());
            ui->lineEditGenre->setText(qry.value(5).toString());
        }
        conn.connClose();
    }
    else{
        QMessageBox::critical(this,tr("error::"),qry.lastError().text());
    }
}


void MainWindow::on_pushButtonUpdate_clicked()
{
   Database conn;

   QString id, artist, album, year, songs, genre;
   id = ui->lineEditId->text();
   artist = ui->lineEditArtist->text();
   album = ui->lineEditAlbum->text();
   year = ui->lineEditYear->text();
   songs = ui->lineEditSongs->text();
   genre = ui->lineEditGenre->text();

   if(!conn.connOpen()){
       qDebug()<<"Failed to open database";
       return;
   }
   conn.connOpen();
   QSqlQuery qry;
   qry.prepare("update vinyl set artist ='"+artist+"', albumName ='"+album+"', year ='"+year+"', songsCount ='"+songs+"', genre ='"+genre+"' where id ='"+id+"'");
   if(qry.exec()){
       QMessageBox::critical(this,tr("Edit"), tr("Updated"));
       conn.connClose();
   }else{
       QMessageBox::critical(this,tr("error:"), qry.lastError().text());
   }
   MainWindow::on_pushButtonLoad_clicked();
}


void MainWindow::on_pushButtonAddNew_clicked()
{
    Database conn;

    QString artist, album, year, songs, genre;
    artist = ui->lineEditArtist->text();
    album = ui->lineEditAlbum->text();
    year = ui->lineEditYear->text();
    songs = ui->lineEditSongs->text();
    genre = ui->lineEditGenre->text();

    if(!conn.connOpen()){
        qDebug()<<"Failed to open database";
        return;
    }
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("insert into vinyl (artist, albumName, year, songsCount, genre) values ('"+artist+"','"+album+"','"+year+"','"+songs+"','"+genre+"')");
    if(qry.exec()){
        QMessageBox::critical(this,tr("Edit"), tr("Added"));
        conn.connClose();
    }else{
        QMessageBox::critical(this,tr("error:"), qry.lastError().text());
    }
    MainWindow::on_pushButtonLoad_clicked();

    ui->lineEditArtist->clear();
    ui->lineEditAlbum->clear();
    ui->lineEditGenre->clear();
    ui->lineEditId->clear();
    ui->lineEditSongs->clear();
    ui->lineEditYear->clear();
}


void MainWindow::on_pushButtonRemove_clicked()
{
    Database conn;

    QString id = ui->lineEditId->text();

    if(!conn.connOpen()){
        qDebug()<<"Failed to open database";
        return;
    }
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("delete from vinyl where id='"+id+"'");
    if(qry.exec()){
        QMessageBox::critical(this,tr("Edit"), tr("Added"));
        conn.connClose();
    }else{
        QMessageBox::critical(this,tr("error:"), qry.lastError().text());
    }
    MainWindow::on_pushButtonLoad_clicked();

    ui->lineEditArtist->clear();
    ui->lineEditAlbum->clear();
    ui->lineEditGenre->clear();
    ui->lineEditId->clear();
    ui->lineEditSongs->clear();
    ui->lineEditYear->clear();
}

