#include "workpage.h"
#include "ui_workpage.h"

WorkPage::WorkPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkPage)
{
    ui->setupUi(this);

    bled = new BleDevice();
    connect(bled, SIGNAL(deviceDisconnected()), this, SLOT(on_device_disconnect()));
    connect(bled, SIGNAL(deviceConnected()),    this, SLOT(on_device_connected()));

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

void WorkPage::set_connection(QString *dev_name, QList<QBluetoothDeviceInfo> &devlist)
{
    bled->setConnect(*dev_name, devlist);
}

void WorkPage::on_device_connected()
{
    qDebug() << "on_device_connected()()";
    ui->pbStart->setEnabled(true);
    ui->pbStop->setEnabled(true);
}

void WorkPage::on_device_disconnect()
{
    qDebug() << "on_device_disconnect()";
    ui->pbDisconnect->setEnabled(true);
    emit WorkPage::disconnected();

}

void WorkPage::on_pbDisconnect_clicked()
{
    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);
    ui->pbDisconnect->setEnabled(false);

    bled->setDeviceDisconnect();
}

void WorkPage::on_pbStart_clicked()
{
    QByteArray data = "3";
    bled->write_service(data);
}

void WorkPage::on_pbStop_clicked()
{
    QByteArray data = "1";
    bled->write_service(data);
}
