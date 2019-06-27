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
    m_control->disconnectFromDevice();
    delete m_control;
    delete m_service;
    delete discoveryAgent;
}

void BleDevice::startScanning()
{
    if (m_control)
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = nullptr;
    devices.clear();
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BleDevice::addDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "addDevice()";
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        auto d = info;
        qDebug() << d.name();
        qDebug() << "rssi: " + QString::number(d.rssi());
        qDebug() << d.serviceUuids();

        devices.append(d);
        m_control = QLowEnergyController::createCentral(d);
        m_control->setRemoteAddressType(QLowEnergyController::PublicAddress);

        connect(m_control, &QLowEnergyController::serviceDiscovered, this, &BleDevice::serviceDiscovered);
        connect(m_control, &QLowEnergyController::discoveryFinished, this, &BleDevice::serviceScanDone);

        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Controller connected. Search services...";
            m_control->discoverServices();
        });

        connect(m_control, &QLowEnergyController::disconnected, this,
                [this]()
        {
            qDebug() << "LowEnergy controller disconnected";
            emit BleDevice::deviceDisconnected();
        });

        // Connect
        m_control->connectToDevice();
    }
}

QStringList BleDevice::getDeviveList()
{
    QStringList device_names;
    foreach (QBluetoothDeviceInfo device, devices)
    {
        device_names.append(device.name());
    }
    return device_names;
}

void BleDevice::setDeviceDisconnect()
{
    if (m_control)
    {
        m_control->disconnectFromDevice();
        delete m_control;
        m_control = nullptr;
        devices.clear();
    }
}

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

void BleDevice::serviceScanDone()
{
    qDebug() << "serviceScanDone()";

    // Delete old service if available
    if (m_service) {
        delete m_service;
        m_service = nullptr;
    }

    if (m_foundUART) {
        m_service = m_control->createServiceObject(UART_uuid);
    }

    if (m_service) {
        connect(m_service, &QLowEnergyService::stateChanged,          this, &BleDevice::serviceStateChanged);
        connect(m_service, &QLowEnergyService::characteristicChanged, this, &BleDevice::updateHeartRateValue);
        connect(m_service, &QLowEnergyService::descriptorWritten,     this, &BleDevice::confirmedDescriptorWrite);
        m_service->discoverDetails();
    }
}

void BleDevice::serviceStateChanged(QLowEnergyService::ServiceState s)
{
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug() << "Discovering services...";
        break;
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
            emit deviceScanFinished();
        }
//        m_notificationDesc = hrChar.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
//        if (m_notificationDesc.isValid())
//            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));

        break;
    }
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
