#include "workpage.h"
#include "ui_workpage.h"

#include <QTimer>

WorkPage::WorkPage(QWidget *parent, BleDevice *bled) :
    QWidget(parent),
    ui(new Ui::WorkPage)
{
    ui->setupUi(this);
    if (parent != nullptr)
    {
        this->resize(parent->size());
    }

    WorkPage::bled = bled;
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
    delete ui;
}

void WorkPage::set_connection(QString *dev_name, QList<QBluetoothDeviceInfo> &devlist)
{
    bled->setConnect(*dev_name, devlist);
    ui->pbDisconnect->setEnabled(false);
    QTimer::singleShot(5000, this, SLOT(on_timer_timeout()));
}

void WorkPage::on_device_connected()
{
    qDebug() << "on_device_connected()()";
    ui->pbDisconnect->setEnabled(true);
    ui->pbStart->setEnabled(true);
    ui->pbStop->setEnabled(true);
}

void WorkPage::on_device_disconnect()
{
    qDebug() << "WorkPage:: on_device_disconnect()";
    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);

    emit WorkPage::disconnected();
}

void WorkPage::on_pbDisconnect_clicked()
{
    ui->pbStart->setEnabled(false);
    ui->pbStop->setEnabled(false);

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

void WorkPage::on_timer_timeout()
{
    ui->pbDisconnect->setEnabled(true);
}

