#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scan_page = new ScanPage(this);
    work_page = new WorkPage(this);

    scan_page->setVisible(true);
    work_page->setVisible(false);

    connect(scan_page, SIGNAL(connection(QString *)),
            this,      SLOT(on_scan_page_connection(QString *)),
            Qt::ConnectionType::UniqueConnection);

    connect(work_page, SIGNAL(disconnected()),
            this,      SLOT(on_work_page_disconnected()),
            Qt::ConnectionType::UniqueConnection);
}

MainWindow::~MainWindow()
{
    delete scan_page;
    delete work_page;
    delete ui;
}

void MainWindow::on_scan_page_connection(QString *dev_name)
{
    qDebug() << "on_scan_page_connection(QString *) " << *dev_name;
    scan_page->setVisible(false);
    work_page->setVisible(true);

}

void MainWindow::on_work_page_disconnected()
{
    qDebug() << "on_work_page_disconnected()";
    scan_page->setVisible(true);
    work_page->setVisible(false);
}
