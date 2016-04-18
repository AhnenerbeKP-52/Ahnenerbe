#ifndef SERIALWAITERDIALOG_H
#define SERIALWAITERDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QPicture>
#include "ui_serialreaderdialog.h"

class SerialWaiterDialog : public QDialog
{
    Q_OBJECT
    Ui::Dialog* form;
    bool selected;
    QString user_selection;
public:
    SerialWaiterDialog(QWidget* parent = nullptr);
    QString get_selection();
private slots:
    void refreshPortList();
    void selectPort(int index);
};

#endif // SERIALWAITERDIALOG_H
