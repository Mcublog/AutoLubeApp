#include "workpage.h"
#include "ui_workpage.h"

WorkPage::WorkPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkPage)
{
    ui->setupUi(this);

    bled = new BleDevice();

    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);

    connect(ui->pbDisconnect,   SIGNAL(clicked()), this, SLOT(on_pbDisconnect_clicked()));
    connect(ui->pbStart,        SIGNAL(clicked()), this, SLOT(on_pbStart_clicked));
    connect(ui->pbStop,         SIGNAL(clicked()), this, SLOT(on_pbStop_clicked));
}

WorkPage::~WorkPage()
{
    delete bled;
    delete ui;
}

void WorkPage::on_pbDisconnect_clicked()
{
    bled->setDeviceDisconnect();
    emit WorkPage::disconnected();
}

void WorkPage::on_pbStart_clicked()
{
    QByteArray data = "3";
    //m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
}

void WorkPage::on_pbStop_clicked()
{
    QByteArray data = "1";
    //m_service->writeCharacteristic(Uart_rx, data, QLowEnergyService::WriteWithoutResponse);
}
