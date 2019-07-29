#ifndef SCANPAGE_H
#define SCANPAGE_H

#include <QWidget>
#include <QListWidget>

#include "bledevice.h"

namespace Ui {
class ScanPage;
}

class ScanPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPage(QWidget *parent = nullptr, BleDevice *bled = nullptr);
    ~ScanPage();

signals:
    void connection(QString *name, QList <QBluetoothDeviceInfo> &devlist);

private:
    Ui::ScanPage *ui;
    BleDevice *bled;

private slots:
    void on_pbFind_clicked();
    void on_deviceScanFinished();
    void on_lwDevList_clicked(QListWidgetItem* listWidgetItem);

public slots:
    void on_device_disconnect();
};

#endif // SCANPAGE_H
