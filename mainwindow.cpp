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

        devices.append(&d);

        ui->teLog->append("Name: " + d.name());
        ui->teLog->append("rssi: " + QString::number(d.rssi()));
    }
}

void MainWindow::deviceScanFinished()
{
    qDebug() << "deviceScanFinished()";
}
