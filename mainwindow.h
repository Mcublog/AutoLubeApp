#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>

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
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList <QBluetoothDeviceInfo *> devices;

    QLowEnergyController *m_control = nullptr;
    QLowEnergyService    *m_service = nullptr;
    QLowEnergyDescriptor m_notificationDesc;

private slots:
    void on_pbFind_clicked();
    void addDevice(const QBluetoothDeviceInfo &info);

    void deviceScanFinished();
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceScanDone();

};

#endif // MAINWINDOW_H
