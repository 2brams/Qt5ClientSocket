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

    setFixedSize(380,200);
    ui->pushButton_connec->setEnabled(false);
    ui->pushButton_deconnexion->setEnabled(false);


    connect(ui->pushButton_test, SIGNAL(clicked()), this, SLOT(test()));
    connect(ui->pushButton_quitter, SIGNAL(clicked()),this, SLOT(close()));
    connect(ui->pushButton_connec, SIGNAL(clicked()),this, SLOT(view()));
    connect(ui->pushButton_deconnexion, SIGNAL(clicked()),this, SLOT(deconnexion()));
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
        ui->pushButton_deconnexion->setEnabled(true);

        ui->led->setColor("green");
        ui->lineEdit_ip->setEnabled(false);
        ui->lineEdit_port->setEnabled(false);

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

void MainWindow::deconnexion()
{
    ui->led->setColor("red");
    ui->lineEdit_ip->setEnabled(true);
    ui->lineEdit_port->setEnabled(true);
    ui->pushButton_connec->setEnabled(false);
    ui->pushButton_deconnexion->setEnabled(false);

}



