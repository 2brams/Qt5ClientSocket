#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QDialog>
#include <QTimer>
#include <QTcpSocket>

namespace Ui {
class Dataview;
}

class Dataview : public QDialog
{
    Q_OBJECT

public:
    explicit Dataview(QWidget *parent = nullptr);
    ~Dataview();
    void initGraph();

private slots:
    void recevSocket(QString, int);
    void readSensor();
    void readCpuTmp();
    void readCpuFrq();

    void startFrq();
    void pauseFrq();
    void startTmp();
    void pauseTmp();
    void ledOn();
    void ledOff();

    void connecter();
    void deconnecter();

    void readyReadFrq();
    void readyReadBme();
    void readyReadTmp();
    void readyReadLed();
    void alarm();

private:
    Ui::Dataview *ui;
    QTimer  *dataTimerBme;
    QTimer  *dataTimerFrq;
    QTimer  *dataTimerTmp;
    QTimer  *dataTimerAlarm;
    double m_tmp;
    double m_hum;
    QTcpSocket *socketFrq;
    QTcpSocket *socketBme;
    QTcpSocket *socketTmp;
    QTcpSocket *socketLed;

    QString m_host;
    int m_port;
    bool m_ledEtat;

};

#endif // DATAVIEW_H
