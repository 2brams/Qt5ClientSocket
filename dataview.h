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
    void connecter();
    void deconnecter();
    void readyRead();
    void alarm();

private:
    Ui::Dataview *ui;
    QTimer  *dataTimer2;
    QTimer  *dataTimerFrq;
    QTimer  *dataTimerTmp;
    QTimer  *dataTimerAlarm;
    double m_tmp;
    double m_hum;
    QTcpSocket *socket;
    QString m_host;
    int m_port;
    int m_choix;
    bool m_ledEtat;

};

#endif // DATAVIEW_H
