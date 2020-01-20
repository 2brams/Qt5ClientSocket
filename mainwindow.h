#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QTcpSocket>
#include "dataview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendConfSocket(QString, int);

public slots:
    void test();
    void readyRead();
    void view();
    void deconnexion();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    Dataview *dataview;
    QTimer  *dataTimer;
};
#endif // MAINWINDOW_H
