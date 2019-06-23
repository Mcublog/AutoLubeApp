#ifndef BLEDEVICE_H
#define BLEDEVICE_H

#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>


class BleDevice
{

public:
    BleDevice();
    ~BleDevice();

    void start_scanning(void);

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QList <QBluetoothDeviceInfo *> devices;

    QLowEnergyController *m_control = nullptr;
    QLowEnergyService    *m_service = nullptr;
    QLowEnergyDescriptor m_notificationDesc;
};

#endif // BLEDEVICE_H
