#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

//--- Bluetooth ---
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>
//---------------



#include "bledevice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    BleDevice *bled;

    bool DoubleClicked;

public slots:
    void on_deviceScanFinished();
    void on_device_disconnect();

private slots:
    void on_pbFind_clicked();
    void on_pbStart_clicked();
    void on_pbStop_clicked();

    void on_lwDevList_clicked(QListWidgetItem* listWidgetItem);

};

#endif // MAINWINDOW_H
