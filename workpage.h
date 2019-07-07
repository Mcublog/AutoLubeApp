#ifndef WORKPAGE_H
#define WORKPAGE_H

#include <QWidget>

namespace Ui {
class WorkPage;
}

class WorkPage : public QWidget
{
    Q_OBJECT

public:
    explicit WorkPage(QWidget *parent = nullptr);
    ~WorkPage();

private:
    Ui::WorkPage *ui;

private slots:
    void on_pbStart_clicked();
    void on_pbStop_clicked();
};

#endif // WORKPAGE_H
