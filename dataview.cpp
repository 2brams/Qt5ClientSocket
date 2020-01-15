#include "dataview.h"
#include "ui_dataview.h"
#include <QString>
#include <QTimer>
#include <QRandomGenerator>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include <QVariant>
#include <QDebug>


Dataview::Dataview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dataview)
{
    ui->setupUi(this);
    dataTimer = new QTimer;
    dataTimer2 = new QTimer;
    m_tmp=0.0;
    m_hum=0.0;
    socket = new QTcpSocket();
    m_isgraph = false;

    initGraph();
    connect( ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));


    connect(ui->pushButton_start, SIGNAL(clicked()),this, SLOT(start()));
    connect(ui->pushButton_pause, SIGNAL(clicked()),this, SLOT(pause()));

    connect(socket, SIGNAL(connected()), this, SLOT(connecter()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecter()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    connect(parent, SIGNAL(sendConfSocket(QString, int)), this, SLOT(recevSocket(QString, int)));
    connect(dataTimer2, SIGNAL(timeout()), this, SLOT(readSensor()));
    dataTimer2->setInterval(2000);


    connect(dataTimer, SIGNAL(timeout()), this, SLOT(readFile()));
    dataTimer->setInterval(1000);


}

Dataview::~Dataview()
{
    delete ui;
}

void Dataview::initGraph()
{
    ui->customPlot->setBackground(QBrush(QColor( 224, 224, 224)));
    ui->customPlot->legend->setVisible(true);

    ui->customPlot->addGraph();
    QPen pen0;
    pen0.setWidth(2);
    pen0.setColor(QColor("green"));
    ui->customPlot->graph(0)->setName("température");
    ui->customPlot->graph(0)->setPen(pen0);

    ui->customPlot->addGraph();
    QPen pen1;
    pen1.setStyle(Qt::DotLine);
    pen1.setWidth(2);
    pen1.setColor(QColor("blue"));
    ui->customPlot->graph(1)->setName("humidité");
    ui->customPlot->graph(1)->setPen(pen1);


    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(5, 50);

}

void Dataview::realtimeDataSlot()
{

    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.002)
    {

        ui->customPlot->graph(0)->addData(key, 15);
        ui->customPlot->graph(1)->addData(key, 10);

        lastPointKey = key;
    }

    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();
}

void Dataview::readSensor()
{
    socket->connectToHost(m_host, m_port);

    if(!socket->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());

    }
    socket->write("cpt");
    m_isgraph=false;
}

void Dataview::readFile()
{
    socket->connectToHost(m_host, m_port);

    if(!socket->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());

    }
    socket->write("tata");
    m_isgraph=true;
}

void Dataview::start()
{
    dataTimer->start();
}

void Dataview::pause()
{
    socket->connectToHost(m_host, m_port);

    if(!socket->waitForConnected(1000)){
        //        QMessageBox::critical(this, "Erreur",socket->errorString());

    }
    socket->write("cpt");
    dataTimer->stop();
}
void Dataview::recevSocket(QString host, int port)
{
    m_host= host;
    m_port=port;
    dataTimer2->start();

}

void Dataview::connecter()
{
    qDebug() << "Connected!";
}

void Dataview::deconnecter()
{
    qDebug() << "Not connected!";
}

void Dataview::readyRead()
{
    QByteArray responseBytes =  socket->readAll();
    socket->close();

    qDebug() << responseBytes;

    if(m_isgraph){
        realtimeDataSlot();
        qDebug() <<"graph" << responseBytes;
    }
    else
    {
//        qDebug() <<"pas graph" << responseBytes;

        QString val= QString(responseBytes);
        QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject sett2 = d.object();

        QJsonValue tmp = sett2.value(QString("temperature"));
        QJsonValue hum = sett2.value(QString("humidity"));

        qDebug()<< tmp;
        qDebug()<< hum;
        //            m_tmp= tmp.toDouble();
        //            ui->label_tmp->setText(QString::number(tmp.toDouble(),'g',3));
        //            m_hum = hum.toDouble();
        //            ui->label_hum->setText(QString::number(hum.toDouble(),'g',3));

    }

}

