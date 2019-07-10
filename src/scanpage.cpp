#include "scanpage.h"
#include "ui_scanpage.h"

#include <QDebug>
#include <QTimer>

#include "bledevice.h"

ScanPage::ScanPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPage)
{
    ui->setupUi(this);
    if (parent != nullptr)
    {
        this->resize(parent->size());
    }

    bled = new BleDevice();
    connect(ui->lwDevList,  SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_lwDevList_clicked(QListWidgetItem*)), Qt::UniqueConnection);

#ifdef DESKTOP
    QListWidgetItem *item = new QListWidgetItem();
    item->setText("Nordic_UART D4:60:F0:47:14:2E");
    item->setTextAlignment(Qt::AlignHCenter);
    ui->lwDevList->addItem(item);
#endif
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
    QTimer::singleShot(8000, this, SLOT(on_deviceScanFinished()));
}

void ScanPage::on_deviceScanFinished()
{
    qDebug() << "deviceScanFinished()";
    QStringList devlist = bled->getDeviveList();
    if (devlist.count()) //device found
    {
        ui->lwDevList->clear();
        ui->lwDevList->addItems(devlist);
        for (int i = 0; i < ui->lwDevList->count(); i++)
        {
            auto item = ui->lwDevList->item(i);
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    else //nof found
    {
        bled->setDeviceDisconnect();
    }
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
