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

    bool m_foundUART = false;
    QBluetoothUuid UART_uuid;
    QLowEnergyCharacteristic Uart_rx;

public slots:


private slots:
    void on_pbFind_clicked();
    void on_pbStart_clicked();
    void on_pbStop_clicked();
    void on_pbClear_clicked();
\
    void addDevice(const QBluetoothDeviceInfo &info);

    void deviceScanFinished();
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void serviceScanDone();

    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateHeartRateValue(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);

};

#endif // MAINWINDOW_H
