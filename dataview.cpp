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
    dataTimer2 = new QTimer;
    dataTimerAlarm = new QTimer;
    m_tmp=0.0;
    m_hum=0.0;
    m_ledEtat=false;
    socket = new QTcpSocket();

    initGraph();

    ui->thermometre->setSuffix("°C");
    ui->thermometre->setNominal(20);
    ui->thermometre->setCritical(29);
    //    ui->thermometre->set

    ui->manometre->setSuffix(" bar");
    ui->manometre->setMaximum(2000.0);
    ui->manometre->setMinimum(500.0);
    ui->manometre->setNominal(800.0);
    ui->manometre->setCritical(1500.0);

    ui->alarm->setColor("gray");
    ui->led->setColor("white");

    connect( ui->customPlot_frq->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_frq->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot_frq->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_frq->yAxis2, SLOT(setRange(QCPRange)));

    connect( ui->customPlot_tmp->xAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_tmp->xAxis2, SLOT(setRange(QCPRange)));
    connect( ui->customPlot_tmp->yAxis, SIGNAL(rangeChanged(QCPRange)),  ui->customPlot_tmp->yAxis2, SLOT(setRange(QCPRange)));


    connect(ui->pushButton_start_frq, SIGNAL(clicked()),this, SLOT(startFrq()));
    connect(ui->pushButton_pause_frq, SIGNAL(clicked()),this, SLOT(pauseFrq()));
    connect(ui->pushButton_start_tmp, SIGNAL(clicked()),this, SLOT(startTmp()));
    connect(ui->pushButton_pause_tmp, SIGNAL(clicked()),this, SLOT(pauseTmp()));


    connect(socket, SIGNAL(connected()), this, SLOT(connecter()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecter()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    connect(parent, SIGNAL(sendConfSocket(QString, int)), this, SLOT(recevSocket(QString, int)));
    connect(dataTimer2, SIGNAL(timeout()), this, SLOT(readSensor()));
    dataTimer2->setInterval(3000);


    connect(dataTimerFrq, SIGNAL(timeout()), this, SLOT(readCpuFrq()));
    dataTimerFrq->setInterval(2000);

    connect(dataTimerTmp, SIGNAL(timeout()), this, SLOT(readCpuTmp()));
    dataTimerTmp->setInterval(2000);

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
    socket->connectToHost(m_host, m_port);

    if(!socket->waitForConnected(1000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());

    }
    socket->write("cpt");
    m_choix=1;
}


void Dataview::readCpuTmp()
{
    socket->connectToHost(m_host, m_port);

    if(!socket->waitForConnected(5000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());

    }
    socket->write("tmp");
    m_choix=3;
}

void Dataview::readCpuFrq()
{
    socket->connectToHost(m_host, m_port);

    if(!socket->waitForConnected(5000)){
        QMessageBox::critical(this, "Erreur",socket->errorString());

    }
    socket->write("frq");
    m_choix=2;
}

void Dataview::startFrq()
{
    dataTimerFrq->start();
    ui->pushButton_start_tmp->setDisabled(true);
}

void Dataview::pauseFrq()
{
    dataTimerFrq->stop();
    ui->pushButton_start_tmp->setDisabled(false);
}

void Dataview::startTmp()
{
    dataTimerTmp->start();
    ui->pushButton_start_frq->setDisabled(true);
}

void Dataview::pauseTmp()
{
    dataTimerTmp->stop();
    ui->pushButton_start_frq->setDisabled(false);
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

    if(m_choix==2){

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
    else if(m_choix==1)
    {

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
        ui->manometre->setValue(pre.toDouble());

        if((int)tmp.toDouble()>=29 ){
            dataTimerAlarm->start();
        }else{
            dataTimerAlarm->stop();
            ui->alarm->setColor("gray");
        }


    } else if(m_choix==3){
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

}

void Dataview::alarm()
{
    m_ledEtat = !m_ledEtat;
    if(m_ledEtat)
        ui->alarm->setColor("red");
    else
        ui->alarm->setColor("gray");
}

