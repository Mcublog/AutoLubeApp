#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLowEnergyService>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);

    bled = new BleDevice();

    QObject::connect(bled, SIGNAL(deviceScanFinished()), this,SLOT(on_deviceScanFinished()));

    connect(ui->pbStart,    SIGNAL(clicked()), this, SLOT(on_pbStart_clicked));
    connect(ui->pbStop,     SIGNAL(clicked()), this, SLOT(on_pbStop_clicked));
}

MainWindow::~MainWindow()
{
    delete bled;
    delete ui;
}

void MainWindow::on_pbFind_clicked()
{
    qDebug() << "on_pbFind_clicked()";
    ui->lwDevList->clear();
    bled->startScanning();
}

void MainWindow::on_deviceScanFinished()
{
    qDebug() << "deviceScanFinished()";
    QStringList devlist = bled->getDeviveList();
    ui->lwDevList->addItems(devlist);
}

void MainWindow::on_pbStart_clicked()
{
    QByteArray data = "3";
    //m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
}

void MainWindow::on_pbStop_clicked()
{
    QByteArray data = "1";
    //m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
}

