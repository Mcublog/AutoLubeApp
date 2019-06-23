#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLowEnergyService>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &MainWindow::addDevice);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &MainWindow::deviceScanFinished);


    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);

    connect(ui->pbClear,    SIGNAL(clicked()), this, SLOT(on_pbClear_clicked));
    connect(ui->pbStart,    SIGNAL(clicked()), this, SLOT(on_pbStart_clicked));
    connect(ui->pbStop,     SIGNAL(clicked()), this, SLOT(on_pbStop_clicked));
}

MainWindow::~MainWindow()
{
    delete discoveryAgent;
    delete ui;
}


void MainWindow::on_pbFind_clicked()
{
    qDebug() << "on_pbFind_clicked()";
    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void MainWindow::addDevice(const QBluetoothDeviceInfo &info)
{
    qDebug() << "addDevice()";
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        auto d = info;
        qDebug() << d.name();
        qDebug() << "rssi: " + QString::number(d.rssi());
        qDebug() << d.serviceUuids();

        devices.append(&d);

        ui->lwDevList->addItem("Name: " + d.name());
        //ui->teLog->append("rssi: " + QString::number(d.rssi()));

        m_control = QLowEnergyController::createCentral(d, this);
        m_control->setRemoteAddressType(QLowEnergyController::PublicAddress);

        connect(m_control, &QLowEnergyController::serviceDiscovered, this, &MainWindow::serviceDiscovered);
        connect(m_control, &QLowEnergyController::discoveryFinished, this, &MainWindow::serviceScanDone);

        connect(m_control, &QLowEnergyController::connected, this, [this]() {
            qDebug() << "Controller connected. Search services...";
            m_control->discoverServices();
        });
        connect(m_control, &QLowEnergyController::disconnected, this, [this]() {
            qDebug() << "LowEnergy controller disconnected";
        });

        // Connect
        m_control->connectToDevice();
        //! [Connect-Signals-2]
    }
}

void MainWindow::deviceScanFinished()
{
    qDebug() << "deviceScanFinished()";
}

void MainWindow::serviceDiscovered(const QBluetoothUuid &gatt)
{
    qDebug() << "serviceDiscovered()";
    qDebug() << gatt.toString();
    if (gatt.toString() == "{6e400001-b5a3-f393-e0a9-e50e24dcca9e}") {
        qDebug() << "UART finded";
        m_foundUART = true;
        UART_uuid = gatt;
    }
}

void MainWindow::serviceScanDone() {
    qDebug() << "serviceScanDone()";

    // Delete old service if available
    if (m_service) {
        delete m_service;
        m_service = nullptr;
    }

    if (m_foundUART) {
        m_service = m_control->createServiceObject(UART_uuid, this);
    }

    if (m_service) {
        connect(m_service, &QLowEnergyService::stateChanged, this, &MainWindow::serviceStateChanged);
        connect(m_service, &QLowEnergyService::characteristicChanged, this, &MainWindow::updateHeartRateValue);
        connect(m_service, &QLowEnergyService::descriptorWritten, this, &MainWindow::confirmedDescriptorWrite);
        m_service->discoverDetails();
    }
}

// Service functions
//! [Find HRM characteristic]
void MainWindow::serviceStateChanged(QLowEnergyService::ServiceState s) {
    switch (s) {
    case QLowEnergyService::DiscoveringServices:
        qDebug() << "Discovering services...";
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        qDebug() << "Service discovered";
        QUuid RX_uuid("{6e400002-b5a3-f393-e0a9-e50e24dcca9e}");
        Uart_rx = m_service->characteristic(RX_uuid);
        if (!Uart_rx.isValid()) {
            qDebug() << "Uart RX not found.";
            break;
        }
        else {
            ui->pbStart->setEnabled(true);
            ui->pbStop->setEnabled(true);
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

    //emit aliveChanged();
}
//! [Find HRM characteristic]

//! [Reading value]
void MainWindow::updateHeartRateValue(const QLowEnergyCharacteristic &c, const QByteArray &value) {
    // ignore any other characteristic change -> shouldn't really happen though
    if (c.uuid() != QBluetoothUuid(QBluetoothUuid::HeartRateMeasurement))
        return;

    //addMeasurement(hrvalue);
}

void MainWindow::confirmedDescriptorWrite(const QLowEnergyDescriptor &d, const QByteArray &value) {
    if (d.isValid() && d == m_notificationDesc && value == QByteArray::fromHex("0000")) {
        //disabled notifications -> assume disconnect intent
        m_control->disconnectFromDevice();
        delete m_service;
        m_service = nullptr;
    }
}

void MainWindow::on_pbStart_clicked() {
    QByteArray data = "3";
    m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
}

void MainWindow::on_pbStop_clicked() {
    QByteArray data = "1";
    m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
}

void MainWindow::on_pbClear_clicked() {
    ui->lwDevList->clear();

    if (m_control)
        m_control->disconnectFromDevice();

    delete m_service;
    m_service = nullptr;

    devices.clear();
    m_foundUART = false;
    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);
}

