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
}

MainWindow::~MainWindow()
{
    delete scan_page;
    delete work_page;
    delete ui;
}
