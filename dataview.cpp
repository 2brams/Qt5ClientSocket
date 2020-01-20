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

    setFixedSize(1100,565);
    dataTimerFrq = new QTimer;
    dataTimerTmp = new QTimer;
    dataTimerBme = new QTimer;
    dataTimerAlarm = new QTimer;
    m_tmp=0.0;
    m_hum=0.0;
    m_ledEtat=false;
    socketFrq = new QTcpSocket();
    socketBme = new QTcpSocket();
    socketTmp = new QTcpSocket();
    socketLed = new QTcpSocket();


    initGraph();

    ui->thermometre->setSuffix("°C");
    ui->thermometre->setNominal(20);
    ui->thermometre->setCritical(29);

    ui->manometre->setSuffix(" bar");
    ui->manometre->setMaximum(2000.0);
    ui->manometre->setMinimum(500.0);
    ui->manometre->setNominal(800.0);
    ui->manometre->setCritical(1500.0);

    ui->alarm->setColor("gray");
    ui->led->setColor("white");
    ui->pushButton_pause_frq->setEnabled(false);
    ui->pushButton_pause_tmp->setEnabled(false);

    connect( ui->customPlot_frq->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_frq->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot_frq->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_frq->yAxis2, SLOT(setRange(QCPRange)));

    connect( ui->customPlot_tmp->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_tmp->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot_tmp->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_tmp->yAxis2, SLOT(setRange(QCPRange)));


    connect(ui->pushButton_start_frq, SIGNAL(clicked()),this, SLOT(startFrq()));
    connect(ui->pushButton_pause_frq, SIGNAL(clicked()),this, SLOT(pauseFrq()));
    connect(ui->pushButton_start_tmp, SIGNAL(clicked()),this, SLOT(startTmp()));
    connect(ui->pushButton_pause_tmp, SIGNAL(clicked()),this, SLOT(pauseTmp()));

    connect(ui->pushButton_on, SIGNAL(clicked()),this, SLOT(ledOn()));
    connect(ui->pushButton_off, SIGNAL(clicked()),this, SLOT(ledOff()));
     connect(socketLed, SIGNAL(readyRead()),this, SLOT(readyReadLed()));


    connect(socketBme, SIGNAL(connected()), this, SLOT(connecter()));
    connect(socketBme, SIGNAL(disconnected()), this, SLOT(deconnecter()));


    connect(parent, SIGNAL(sendConfSocket(QString, int)), this, SLOT(recevSocket(QString, int)));
    connect(dataTimerBme, SIGNAL(timeout()), this, SLOT(readSensor()));
    dataTimerBme->setInterval(1500);
    connect(socketBme, SIGNAL(readyRead()),this, SLOT(readyReadBme()));


    connect(dataTimerFrq, SIGNAL(timeout()), this, SLOT(readCpuFrq()));
    dataTimerFrq->setInterval(3000);
    connect(socketFrq, SIGNAL(readyRead()),this, SLOT(readyReadFrq()));


    connect(dataTimerTmp, SIGNAL(timeout()), this, SLOT(readCpuTmp()));
    dataTimerTmp->setInterval(2000);
    connect(socketTmp, SIGNAL(readyRead()),this, SLOT(readyReadTmp()));


    connect(dataTimerAlarm, SIGNAL(timeout()), this, SLOT(alarm()));
    dataTimerAlarm->setInterval(500);

}

Dataview::~Dataview()
{
    delete ui;
}

void Dataview::initGraph()
{
    ui->customPlot_frq->setBackground(QBrush(QColor( 224, 224, 224)));
    ui->customPlot_frq->legend->setVisible(true);

    ui->customPlot_frq->addGraph();
    QPen pen0;
    pen0.setWidth(2);
    pen0.setColor(QColor("green"));
    ui->customPlot_frq->graph(0)->setName("Fréquence");
    ui->customPlot_frq->graph(0)->setPen(pen0);

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot_frq->xAxis->setTicker(timeTicker);
    ui->customPlot_frq->axisRect()->setupFullAxesBox();
    ui->customPlot_frq->yAxis->setRange(450, 700);


    ui->customPlot_tmp->setBackground(QBrush(QColor( 224, 224, 224)));
    ui->customPlot_tmp->legend->setVisible(true);

    ui->customPlot_tmp->addGraph();
    QPen pen1;
    pen1.setWidth(2);
    pen1.setColor(QColor("blue"));
    ui->customPlot_tmp->graph(0)->setName("Température");
    ui->customPlot_tmp->graph(0)->setPen(pen1);

    QSharedPointer<QCPAxisTickerTime> timeTicker2(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot_tmp->xAxis->setTicker(timeTicker2);
    ui->customPlot_tmp->axisRect()->setupFullAxesBox();
    ui->customPlot_tmp->yAxis->setRange(40, 70);

}


void Dataview::readSensor()
{
    socketBme->connectToHost(m_host, m_port);

    if(!socketBme->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socketBme->errorString());

    }
    socketBme->write("cpt");
}


void Dataview::readCpuTmp()
{
    socketTmp->connectToHost(m_host, m_port);

    if(!socketTmp->waitForConnected(2000)){
        QMessageBox::critical(this, "Erreur",socketTmp->errorString());

    }
    socketTmp->write("tmp");
}

void Dataview::readCpuFrq()
{
    socketFrq->connectToHost(m_host, m_port);

    if(!socketFrq->waitForConnected(3000)){
        QMessageBox::critical(this, "Erreur",socketFrq->errorString());

    }
    socketFrq->write("frq");
}

void Dataview::startFrq()
{
    dataTimerFrq->start();
    ui->pushButton_start_frq->setEnabled(false);
    ui->pushButton_pause_frq->setEnabled(true);
}

void Dataview::pauseFrq()
{
    dataTimerFrq->stop();
    ui->pushButton_start_frq->setEnabled(true);
    ui->pushButton_pause_frq->setEnabled(false);
}

void Dataview::startTmp()
{
    dataTimerTmp->start();
    ui->pushButton_start_tmp->setEnabled(false);
    ui->pushButton_pause_tmp->setEnabled(true);
}

void Dataview::pauseTmp()
{
    dataTimerTmp->stop();
    ui->pushButton_start_tmp->setEnabled(true);
    ui->pushButton_pause_tmp->setEnabled(false);
}

void Dataview::ledOn()
{
    socketLed->connectToHost(m_host, m_port);

    if(!socketLed->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socketLed->errorString());

    }
    socketLed->write("on");
}

void Dataview::ledOff()
{
    socketLed->connectToHost(m_host, m_port);

    if(!socketLed->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socketLed->errorString());

    }
    socketLed->write("off");
}

void Dataview::recevSocket(QString host, int port)
{
    m_host= host;
    m_port=port;
    dataTimerBme->start();

}

void Dataview::connecter()
{
    qDebug() << "Connected!";
}

void Dataview::deconnecter()
{
    qDebug() << "Not connected!";
}

void Dataview::readyReadFrq()
{
    QByteArray responseBytes =  socketFrq->readAll();
    socketFrq->close();


    QString s(responseBytes);
    bool ok = false;
    double b = s.toDouble(&ok);

    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0;

    static double lastPointKey = 0;
    if (key - lastPointKey > 0.002 && b!=0.0)
    {

        ui->customPlot_frq->graph(0)->addData(key, b);
        lastPointKey = key;
    }

    ui->customPlot_frq->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot_frq->replot();

    qDebug() <<"Frq" <<responseBytes;

}

void Dataview::readyReadBme()
{
    QByteArray responseBytes =  socketBme->readAll();
    socketBme->close();

    QString val= QString(responseBytes);
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject sett2 = d.object();

    QJsonValue tmp = sett2.value(QString("temperature"));
    QJsonValue hum = sett2.value(QString("humidity"));
    QJsonValue pre = sett2.value(QString("pressure"));

    qDebug()<< tmp.toDouble();
    qDebug()<< hum.toDouble();
    qDebug()<< pre.toDouble();

    ui->thermometre->setValue((int)tmp.toDouble());
    ui->humidite->setValue(hum.toDouble());
    ui->manometre->setValue((int)pre.toDouble());

    if((int)tmp.toDouble()>=29 ){
        dataTimerAlarm->start();
    }else{
        dataTimerAlarm->stop();
        ui->alarm->setColor("gray");
    }
}

void Dataview::readyReadTmp()
{
    QByteArray responseBytes =  socketTmp->readAll();
    socketTmp->close();

    QString s(responseBytes);
    bool ok = false;
    double b1 = s.toDouble(&ok);

    static QTime time2(QTime::currentTime());
    double key2 = time2.elapsed()/1000.0;

    static double lastPointKey = 0;
    if (key2 - lastPointKey > 0.002 && b1!=0.0)
    {

        ui->customPlot_tmp->graph(0)->addData(key2, b1);
        lastPointKey = key2;
    }

    ui->customPlot_tmp->xAxis->setRange(key2, 8, Qt::AlignRight);
    ui->customPlot_tmp->replot();

    qDebug() <<"Tmp" <<responseBytes;
}

void Dataview::readyReadLed()
{
    QByteArray responseBytes =  socketLed->readAll();
    socketLed->close();

    qDebug()<< "Led" << responseBytes;


    if(responseBytes=="0"){
       ui->led->setColor("white");
    }else if (responseBytes=="1"){
      ui->led->setColor("red");
    }
}

void Dataview::alarm()
{
    m_ledEtat = !m_ledEtat;
    if(m_ledEtat)
        ui->alarm->setColor("red");
    else
        ui->alarm->setColor("gray");
}

