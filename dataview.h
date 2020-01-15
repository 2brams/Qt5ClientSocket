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
    void realtimeDataSlot();
    void readSensor();
    void readFile();
    void start();
    void pause();
    void connecter();
    void deconnecter();
    void readyRead();

private:
    Ui::Dataview *ui;
    QTimer  *dataTimer;
    QTimer  *dataTimer2;
    double m_tmp;
    double m_hum;
    QTcpSocket *socket;
    QString m_host;
    int m_port;
    bool m_isgraph;

};

#endif // DATAVIEW_H
