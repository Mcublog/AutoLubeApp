#ifndef BLEDEVICE_H
#define BLEDEVICE_H

#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>


class BleDevice: public QObject
{
    Q_OBJECT

public:
    BleDevice();
    ~BleDevice();

    void startScanning();
    QStringList getDeviveList();
    void setDeviceDisconnect();
    void setConnect(QString device_name);

signals:
    void deviceScanFinished();
    void deviceDisconnected();
    void deviceConnected();

public slots:
    void addDevice(const QBluetoothDeviceInfo &info);
    void serviceScanDone();
    void serviceDiscovered(const QBluetoothUuid &gatt);
    void connectionDone();

    void serviceStateChanged(QLowEnergyService::ServiceState s);
    void updateHeartRateValue(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList <QBluetoothDeviceInfo> devices;

    QLowEnergyController *m_control = nullptr;
    QLowEnergyService    *m_service = nullptr;
    QLowEnergyDescriptor m_notificationDesc;

    bool m_foundUART = false;
    QBluetoothUuid UART_uuid;
    QLowEnergyCharacteristic Uart_rx;

    bool connection = false;

};

#endif // BLEDEVICE_H
