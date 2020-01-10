#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    ui->pushButton_connec->setEnabled(false);
    ui->lineEdit_msg_client->setReadOnly(true);
    ui->lineEdit_msg_server->setReadOnly(true);

    connect(ui->pushButton_test, SIGNAL(clicked()), this, SLOT(test()));

    connect(socket, SIGNAL(connected()), this, SLOT(connecter()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecter()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(ui->pushButton_quitter, SIGNAL(clicked()),this, SLOT(close()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::communication(QString msg)
{

    socket->connectToHost(ui->lineEdit_ip->text(), ui->lineEdit_port->text().toInt());

    if(!socket->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());
        ui->lineEdit_msg_server->setText("Error");
        ui->pushButton_connec->setEnabled(false);
        return 0;
    }
    socket->write(msg.toLocal8Bit());

    return 1;
}

void MainWindow::chat()
{

    socket->write("cpt");
    socket->waitForBytesWritten(1000);
    socket->waitForReadyRead(3000);

    qDebug() << "Reading: " << socket->bytesAvailable();

    QByteArray responseBytes =  socket->readAll();

    /* QString val= QString(responseBytes);
          QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
          QJsonObject sett2 = d.object();
          QJsonValue nom = sett2.value(QString("nom"));
          QJsonValue age = sett2.value(QString("age"));
          QJsonValue value = sett2.value(QString("list"));*/

    qDebug() << responseBytes;
    /*  qDebug() << nom.toString();
          qDebug() << age.toString();
          qDebug() << value.toArray();
          QJsonArray test = value.toArray();
          qDebug() << test[1];
          qDebug() << test[2];
          qDebug() << test[0];*/

}

void MainWindow::test()
{
    if(communication(ui->lineEdit_msg_client->text())){
        ui->pushButton_connec->setEnabled(true);
        ui->lineEdit_msg_server->setText("Success");
    }
}

void MainWindow::connecter()
{
    qDebug() << "Connected!";
}

void MainWindow::deconnecter()
{
    qDebug() << "Not connected!";
}

void MainWindow::readyRead()
{
    QByteArray responseBytes =  socket->readAll();
    socket->close();
    qDebug() << responseBytes;
}


