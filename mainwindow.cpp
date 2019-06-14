#include "mainwindow.h"
#include "ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete discoveryAgent;
    delete ui;
}


void MainWindow::on_pbFind_clicked()
{
    qDebug() << "on_pbFind_clicked()";
    devices.clear();
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

        ui->teLog->append("Name: " + d.name());
        ui->teLog->append("rssi: " + QString::number(d.rssi()));

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
//        m_control->connectToDevice();
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
}

void MainWindow::serviceScanDone()
{
    qDebug() << "serviceScanDone()";
}
