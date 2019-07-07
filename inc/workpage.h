#ifndef WORKPAGE_H
#define WORKPAGE_H

#include <QWidget>

#include "bledevice.h"

namespace Ui {
class WorkPage;
}

class WorkPage : public QWidget
{
    Q_OBJECT

public:
    explicit WorkPage(QWidget *parent = nullptr);
    ~WorkPage();

    void set_connection(QString *string);

private:
    Ui::WorkPage *ui;
    BleDevice *bled;

signals:
    void disconnected();

private slots:
    void on_pbDisconnect_clicked();
    void on_pbStart_clicked();
    void on_pbStop_clicked();
};

#endif // WORKPAGE_H
