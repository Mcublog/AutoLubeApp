#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "scanpage.h"
#include "workpage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    ScanPage *scan_page;
    WorkPage *work_page;

private slots:
    void on_scan_page_connection(QString *dev_name, QList <QBluetoothDeviceInfo> &);
    void on_work_page_disconnected();
};

#endif // MAINWINDOW_H
