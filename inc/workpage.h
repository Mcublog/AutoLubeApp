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
    explicit WorkPage(QWidget *parent = nullptr, BleDevice *bled = nullptr);
    ~WorkPage();

    void set_connection(QString *dev_name, QList <QBluetoothDeviceInfo> &devlist);

private:
    Ui::WorkPage *ui;
    BleDevice *bled;

signals:
    void disconnected();

private slots:
    void on_pbDisconnect_clicked();
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_device_connected();
    void on_device_disconnect();
    void on_timer_timeout();
};

#endif // WORKPAGE_H
