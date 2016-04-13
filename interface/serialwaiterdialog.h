#ifndef SERIALWAITERDIALOG_H
#define SERIALWAITERDIALOG_H

#include <QDialog>
#include <QBoxLayout>
#include <QComboBox>
#include <QPushButton>

class SerialWaiterDialog : public QDialog
{
    Q_OBJECT
    bool selected;
    QBoxLayout* lo;
    QComboBox* port_list;
    QPushButton* refresh_button;
    QPushButton* confirm_button;
    QString user_selection;
public:
    SerialWaiterDialog(QWidget* parent = nullptr);
    QString get_selection();
private slots:
    void refreshPortList();
    void selectPort(int index);
};

#endif // SERIALWAITERDIALOG_H
