#include "bledevice.h"

#include <QLowEnergyService>

BleDevice::BleDevice()
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BleDevice::addDevice);
    //connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,         this, &BleDevice::deviceScanFinished);
//    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,         this,
//            [this]()
//    {
//        emit BleDevice::deviceScanFinished();
//    });


}

BleDevice::~BleDevice()
{
    if (m_control)
        m_control->disconnectFromDevice();
    delete m_control;
    delete m_service;
    delete discoveryAgent;
}

/*-----------------------------------------------------------
/Fill a list of device available to connect.
/param:
/return:
-----------------------------------------------------------*/
void BleDevice::startScanning()
{
    setDeviceDisconnect();
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

/*-----------------------------------------------------------
/Add device to list
/param:
/return:
-----------------------------------------------------------*/
void BleDevice::addDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "addDevice()";
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        devices.append(info);
        qDebug() << devices.last().name();
        qDebug() << "rssi: "<< devices.last().rssi() << " " << devices.last().address().toString();
        qDebug() << devices.last().serviceUuids();

        m_control = QLowEnergyController::createCentral(devices.last());
        m_control->setRemoteAddressType(QLowEnergyController::PublicAddress);

        connect(m_control, &QLowEnergyController::serviceDiscovered, this, &BleDevice::serviceDiscovered);
        connect(m_control, &QLowEnergyController::discoveryFinished, this, &BleDevice::serviceScanDone);

        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Controller connected. Search services...";
            m_control->discoverServices();
        });

        connect(m_control, &QLowEnergyController::disconnected, this, &BleDevice::setDeviceDisconnect);

        // Connect
        m_control->connectToDevice();
    }
}

/*-----------------------------------------------------------
/Add device to list
/param:
/return:
-----------------------------------------------------------*/
QStringList BleDevice::getDeviveList()
{
    QStringList device_names;
    for (QBluetoothDeviceInfo device: devices)
    {
        device_names.append(device.name() + " " + device.address().toString());
    }
    return device_names;
}

/*-----------------------------------------------------------
/Add device to list
/param:
/return:
-----------------------------------------------------------*/
void BleDevice::setDeviceDisconnect()
{
    if (m_control)
    {
        m_control->disconnectFromDevice();
    }
    delete m_control;
    m_control = nullptr;
    devices.clear();
    m_foundUART = false;
    emit BleDevice::deviceDisconnected();
}

/*-----------------------------------------------------------
/New service discovered
/param: link to UUID service
/return:
-----------------------------------------------------------*/
void BleDevice::serviceDiscovered(const QBluetoothUuid &gatt)
{
    qDebug() << "serviceDiscovered()";
    qDebug() << gatt.toString();
    if (gatt.toString() == "{6e400001-b5a3-f393-e0a9-e50e24dcca9e}")
    {
        qDebug() << "UART finded";
        m_foundUART = true;
        UART_uuid = gatt;
    }
}

/*-----------------------------------------------------------
/All service scanned and now we can to disxonnect from the device
/param:
/return:
-----------------------------------------------------------*/
void BleDevice::serviceScanDone()
{
    qDebug() << "serviceScanDone()";

    if (!m_foundUART)
    {
        devices.pop_back();
    }

    if (m_control)
    {
        m_control->disconnectFromDevice();
    }
    delete m_control;
    m_control = nullptr;
    m_foundUART = false;
    emit BleDevice::deviceScanFinished();
}

void BleDevice::connectionDone()
{
    qDebug() << "connectionDone()";
    // Delete old service if available
    if (m_service)
    {
        delete m_service;
        m_service = nullptr;
    }

    if (m_foundUART)
    {
        m_service = m_control->createServiceObject(UART_uuid);
    }

    if (m_service)
    {
        connect(m_service, &QLowEnergyService::stateChanged,          this, &BleDevice::serviceStateChanged);
        connect(m_service, &QLowEnergyService::characteristicChanged, this, &BleDevice::updateHeartRateValue);
        connect(m_service, &QLowEnergyService::descriptorWritten,     this, &BleDevice::confirmedDescriptorWrite);
        m_service->discoverDetails();
        emit BleDevice::deviceConnected();
    }
}

void BleDevice::setConnect(QString device_name)
{
    bool device_find = false;
    int dev_idx = 0;
    for (int i = 0; i < devices.count(); i++)
    {
        if (devices[i].name() + " " + devices[i].address().toString() == device_name)
        {
            qDebug() << "Device find at: " << i;
            device_find = true;
            dev_idx = i;
            break;
        }
    }

    if (device_find)
    {
        m_control = QLowEnergyController::createCentral(devices.at(dev_idx));
        m_control->setRemoteAddressType(QLowEnergyController::PublicAddress);

        connect(m_control, &QLowEnergyController::serviceDiscovered, this, &BleDevice::serviceDiscovered);
        connect(m_control, &QLowEnergyController::discoveryFinished, this, &BleDevice::connectionDone);

        connect(m_control, &QLowEnergyController::connected, this, [this]()
        {
            qDebug() << "Controller connected. Search services...";
            m_control->discoverServices();
        });

        connect(m_control, &QLowEnergyController::disconnected, this, &BleDevice::setDeviceDisconnect);

        // Connect
        m_control->connectToDevice();

    }

}

void BleDevice::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    qDebug() << s;
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
    {
        qDebug() << "Discovering services...";
    }break;

    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug() << "Service discovered";
        QUuid RX_uuid("{6e400002-b5a3-f393-e0a9-e50e24dcca9e}");
        Uart_rx = m_service->characteristic(RX_uuid);
        if (!Uart_rx.isValid())
        {
            qDebug() << "Uart RX not found.";
            break;
        }
        else
        {
            qDebug() << "Uart RX found.";
            QByteArray data = "3";
            m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
            emit deviceScanFinished();
        }
//        m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//        if (m_notificationDesc.isValid())
//            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));


    }break;

    default:
        //nothing for now
        break;
    }

}

void BleDevice::updateHeartRateValue(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid() != QBluetoothUuid(QBluetoothUuid::HeartRateMeasurement))
        return;

    //addMeasurement(hrvalue);
}

void BleDevice::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value)
{
    if (d.isValid() && d == m_notificationDesc && value == QByteArray::fromHex("0000")) {
        //disabled notifications -> assume disconnect intent
        m_control->disconnectFromDevice();
        delete m_service;
        m_service = nullptr;
    }
}
