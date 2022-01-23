#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonLoad_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButtonUpdate_clicked();

    void on_pushButtonAddNew_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
