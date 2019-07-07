#include "scanpage.h"
#include "ui_scanpage.h"

#include <QDebug>

#include "bledevice.h"

ScanPage::ScanPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPage)
{
    ui->setupUi(this);

    bled = new BleDevice();
    connect(ui->lwDevList,  SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_lwDevList_clicked(QListWidgetItem*)), Qt::UniqueConnection);
}

ScanPage::~ScanPage()
{
    delete bled;
    delete ui;
}

void ScanPage::on_pbFind_clicked()
{
    qDebug() << "on_pbFind_clicked()";
    ui->lwDevList->clear();
    ui->pbFind->setEnabled(false);
    connect(bled, SIGNAL(deviceScanFinished()), this, SLOT(on_deviceScanFinished()));
    bled->startScanning();
}

void ScanPage::on_deviceScanFinished()
{
    qDebug() << "deviceScanFinished()";
    QStringList devlist = bled->getDeviveList();
    ui->lwDevList->clear();
    ui->lwDevList->addItems(devlist);
    ui->pbFind->setEnabled(true);
}

void ScanPage::on_lwDevList_clicked(QListWidgetItem* listWidgetItem)
{
    qDebug() << "on_lwDevList_dclicked: " << listWidgetItem->text();
    QString dev_name = listWidgetItem->text();
    on_device_disconnect();
    QList <QBluetoothDeviceInfo> devlist;
    bled->getDeviveList(devlist);
    emit connection(&dev_name, devlist);
}

void ScanPage::on_device_disconnect()
{
    qDebug() << "on_device_disconnect";
    ui->pbFind->setEnabled(true);
    ui->lwDevList->clear();
}
