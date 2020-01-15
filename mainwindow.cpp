#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket();

    dataview = new Dataview(this);

    setFixedSize(330,200);
    ui->pushButton_connec->setEnabled(false);


    connect(ui->pushButton_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(ui->pushButton_quitter, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->pushButton_connec, SIGNAL(clicked()),this, SLOT(view()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    socket->connectToHost(ui->lineEdit_ip->text(), ui->lineEdit_port->text().toInt());

    if(!socket->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());

        ui->pushButton_connec->setEnabled(false);

    }
    else
    {
        socket->write("Bnjour");
        ui->pushButton_connec->setEnabled(true);

    }

}


void MainWindow::readyRead()
{
    QByteArray responseBytes =  socket->readAll();
    socket->close();
    qDebug() << responseBytes;
    emit sendConfSocket(ui->lineEdit_ip->text(), ui->lineEdit_port->text().toInt());
}

void MainWindow::view()
{
    hide();
    dataview->exec();
    show();

}



