#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTableView();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadTableView(){
    Database conn;
    QSqlQueryModel * model = new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery * qry = new QSqlQuery(conn.db);

    qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Cover path' from vinyl");

    qry->exec();
    model->setQuery(*qry);
    ui->tableView->setModel(model);

    conn.connClose();
//    qDebug() << (model->rowCount());
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_pushButtonLoad_clicked()
{
    loadTableView();
    ui->lineEditSearch->clear();

}

void MainWindow::drawImage(){
    QPixmap image(ui->lineEditImage->text());
    ui->labelShowImage->setPixmap(image.scaled(ui->labelShowImage->width(),
                                               ui->labelShowImage->height(),
                                               Qt::KeepAspectRatio,Qt::SmoothTransformation));
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
            ui->lineEditGenre->setText(qry.value(5).toString());
            ui->lineEditImage->setText(qry.value(6).toString());
            ui->spinBoxYear->setValue(qry.value(3).toInt());
            ui->spinBoxSongs->setValue(qry.value(4).toInt());
        }
        conn.connClose();
    }
    else{
        QMessageBox::critical(this,tr("Error"),qry.lastError().text());
    }
    drawImage();
}

void MainWindow::clearLineEdits(){
    ui->lineEditArtist->clear();
    ui->lineEditAlbum->clear();
    ui->lineEditGenre->clear();
    ui->lineEditId->clear();
    ui->spinBoxSongs->displayIntegerBase();
    ui->spinBoxYear->displayIntegerBase();
    ui->lineEditImage->clear();
    ui->labelShowImage->clear();
}

void MainWindow::on_pushButtonAddNew_clicked()
{
    if(checkIfLineEditsEmpty()) return;
    Database conn;

    QString artist, album, year, songs, genre, fileName;
    artist = ui->lineEditArtist->text();
    album = ui->lineEditAlbum->text();
    year = ui->spinBoxYear->text();
    songs = ui->spinBoxSongs->text();
    genre = ui->lineEditGenre->text();
    fileName = ui->lineEditImage->text();

    if(!conn.connOpen()){
        qDebug()<<"Failed to open database";
        return;
    }

    if(!fileName.isEmpty()){
        QString random = QUuid::createUuid().toString();
        random.remove(QRegularExpression("{|}|-"));
        if(!QDir("Covers").exists()){
            QDir().mkdir("Covers");
        }
        QString newFilePath = "Covers/"+ random + ".bmp";
        if (QFile::exists(newFilePath))
        {
            QFile::remove(newFilePath);
        }
        QFile::copy(fileName,newFilePath);
        ui->lineEditImage->setText(newFilePath);
        fileName = newFilePath;
    }

    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("insert into vinyl (artist, albumName, year, songsCount, genre, imageSrc) values ('"+artist+"','"+album+"','"+year+"','"+songs+"','"+genre+"','"+fileName+"')");
    if(qry.exec()){
        loadTableView();
        QMessageBox::information(this,tr("Add new"), tr("Added"));
        conn.connClose();
    }else{
        QMessageBox::critical(this,tr("error:"), qry.lastError().text());
    }
    clearLineEdits();

}

void MainWindow::on_pushButtonUpdate_clicked()
{
   if(getSelectedRow() == -1){
       QMessageBox::warning(this,tr("Update"), tr("Choose item to update!"));
       return;
   }
   if(checkIfLineEditsEmpty()) return;

   Database conn;

   QString id, artist, album, year, songs, genre, fileName;
   id = ui->lineEditId->text();
   artist = ui->lineEditArtist->text();
   album = ui->lineEditAlbum->text();
   year = ui->spinBoxYear->text();
   songs = ui->spinBoxSongs->text();
   genre = ui->lineEditGenre->text();
   fileName = ui->lineEditImage->text();

   if(!conn.connOpen()){
       qDebug()<<"Failed to open database";
       return;
   }

   conn.connOpen();
   QSqlQuery qry;
   qry.prepare("select imageSrc from vinyl where id='"+id+"'");
   QString imageSrc;
   if(qry.exec()){
       while(qry.next())
       imageSrc = qry.value(0).toString();
        qDebug() << (imageSrc);
       conn.connClose();
   }
   else{
       QMessageBox::critical(this,tr("Error"),qry.lastError().text());
   }



   if(!fileName.isEmpty() && imageSrc != fileName){
       QString random = QUuid::createUuid().toString();
       random.remove(QRegularExpression("{|}|-"));
       if(!QDir("Covers").exists()){
           QDir().mkdir("Covers");
       }
       QString newFilePath = "Covers/"+ random+ ".bmp";
       if (QFile::exists(newFilePath))
       {
           QFile::remove(newFilePath);
       }
       QFile::copy(fileName,newFilePath);
       ui->lineEditImage->setText(newFilePath);
       fileName = newFilePath;
   }

   conn.connOpen();
   QSqlQuery qry2;
   qry2.prepare("update vinyl set artist ='"+artist+"', albumName ='"+album+"', year ='"+year+"',songsCount ='"+songs+"', genre ='"+genre+"', imageSrc ='"+fileName+"' where id ='"+id+"'");

   if(qry2.exec()){
       loadTableView();
       drawImage();
       QMessageBox::information(this,tr("Edit"), tr("Updated"));
       conn.connClose();
   }else{
       QMessageBox::critical(this,tr("error:"), qry2.lastError().text());
   }

}


void MainWindow::on_pushButtonRemove_clicked()
{
    Database conn;
    QString id = ui->lineEditId->text();
    if(id.isEmpty()){
        QMessageBox::warning(this,tr("Remove"), tr("Choose item to remove!"));
        return;
    }
    int selectedRow = getSelectedRow();
    if(selectedRow != -1){
        QString imageSrc = ui->tableView->model()->index(selectedRow,6).data().toString();
        if (QFile::exists(imageSrc))
        {
            QFile::remove(imageSrc);
        }
    }

    if(!conn.connOpen()){
        qDebug()<<"Failed to open database";
        return;
    }
    conn.connOpen();
    QSqlQuery qry;
    qry.prepare("delete from vinyl where id='"+id+"'");
    if(qry.exec()){
        QMessageBox::information(this,tr("Remove"), tr("Removed"));
        conn.connClose();
    }else{
        QMessageBox::critical(this,tr("Error"), qry.lastError().text());
    }
    loadTableView();
    clearLineEdits();
}


void MainWindow::on_pushButtonSearch_clicked()
{
    QString searchString = ui->lineEditSearch->text();
    Database conn;
    QSqlQueryModel * model = new QSqlQueryModel();

    conn.connOpen();
    QSqlQuery * qry = new QSqlQuery(conn.db);
    int comboBoxCurrent = ui->comboBoxSearch->currentIndex();
    switch (comboBoxCurrent) {
        case 0:
            qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Image path' from vinyl where artist like '%"+searchString+"%' or albumName like '%"+searchString+"%' or genre like '%"+searchString+"%' or year like '%"+searchString+"%' or songsCount like '%"+searchString+"%'");
            break;
        case 1:
            qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Image path' from vinyl where artist like '%"+searchString+"%'");
            break;
        case 2:
            qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Image path' from vinyl where albumName like '%"+searchString+"%'");
            break;
        case 3:
            qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Image path' from vinyl where genre like '%"+searchString+"%'");
            break;
        case 4:
            qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Image path' from vinyl where year like '%"+searchString+"%'");
            break;
        case 5:
            qry->prepare("select id as ID, artist as Artist, albumName as Album, songsCount as Songs, year as Year, genre as Genre, imageSrc as 'Image path' from vinyl where songsCount like '%"+searchString+"%'");
            break;
    }

    qry->exec();
    model->setQuery(*qry);
    ui->tableView->setModel(model);

    conn.connClose();
    qDebug() << (model->rowCount());
}


void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Images (*.bmp *.png *.xpm *.jpg)"));
    if(fileName != ""){
        ui->lineEditImage->setText(fileName);

    }
    drawImage();

}

bool MainWindow::checkIfLineEditsEmpty(){
    if(ui->lineEditAlbum->text().isEmpty() ||
            ui->lineEditArtist->text().isEmpty() ||
            ui->spinBoxSongs->text().isEmpty() ||
            ui->spinBoxYear->text().isEmpty()){
        QMessageBox::warning(this,tr("Error"), "Some required fields (*) are not filled!");
        return true;
    }
    return false;
}


void MainWindow::on_pushButtonClear_clicked()
{
    clearLineEdits();
}


void MainWindow::on_pushButtonTest_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.count()>0){
    QModelIndex index = selection.at(0);
        qDebug() << index.row();
    }
}

int MainWindow::getSelectedRow(){
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if(selection.count()>0){
    QModelIndex index = selection.at(0);
        qDebug() << index.row();
        return index.row();
    } else return -1;
}

