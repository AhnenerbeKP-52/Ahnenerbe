#include "serialwaiterdialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>
#include "ui_serialreaderdialog.h"//TODO remove

QString SerialWaiterDialog::get_selection(){
    if(selected)
        return this->user_selection;
    else return QString::null;
}

void SerialWaiterDialog::selectPort(int index){
    this->user_selection = form->port_list->currentData().toString();
    selected = true;
    form->confirm_button->setEnabled(true);
}

void SerialWaiterDialog::refreshPortList(){
    selected = false;
    form->confirm_button->setEnabled(false);

    form->port_list->clear();
    for(QSerialPortInfo& info : QSerialPortInfo::availablePorts()){
        form->port_list->addItem(info.portName()+" "+info.description(),info.portName());
    }

    if(form->port_list->count() == 1){
        form->port_list->setCurrentIndex(0);
        selectPort(0);
    }
}


SerialWaiterDialog::SerialWaiterDialog(QWidget* parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
      form(new Ui::Dialog)
{

    form->setupUi(this);

    form->refresh_button->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");

    form->confirm_button->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");

    connect(form->refresh_button,&QPushButton::clicked,this, &SerialWaiterDialog::refreshPortList);
    connect(form->port_list,static_cast<void (QComboBox::*)(int n)>(&QComboBox::activated),
            this,&SerialWaiterDialog::selectPort);
    connect(form->confirm_button,&QPushButton::clicked,this,[=](){this->accept();});
    refreshPortList();
    selected = false;
}
