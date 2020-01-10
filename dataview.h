#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QDialog>

namespace Ui {
class Dataview;
}

class Dataview : public QDialog
{
    Q_OBJECT

public:
    explicit Dataview(QWidget *parent = nullptr);
    ~Dataview();

private:
    Ui::Dataview *ui;
};

#endif // DATAVIEW_H
