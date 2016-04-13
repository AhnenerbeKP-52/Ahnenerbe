#include "serialwaiterdialog.h"
#include <QtSerialPort/QSerialPortInfo>

QString SerialWaiterDialog::get_selection(){
    if(selected)
        return this->user_selection;
    else return QString::null;
}

void SerialWaiterDialog::selectPort(int index){
    this->user_selection = port_list->currentData().toString();
    selected = true;
    confirm_button->setEnabled(true);
}

void SerialWaiterDialog::refreshPortList(){
    selected = false;
    confirm_button->setEnabled(false);

    port_list->clear();
    for(QSerialPortInfo& info : QSerialPortInfo::availablePorts()){
        port_list->addItem(info.portName()+" "+info.description(),info.portName());
    }
}


SerialWaiterDialog::SerialWaiterDialog(QWidget* parent)
    : QDialog(parent), selected(false)
{
    setWindowTitle("Select Arduino port");

    lo = new QBoxLayout(QBoxLayout::LeftToRight, this);
    port_list = new QComboBox();
    refresh_button = new QPushButton("Refresh port list");
    confirm_button = new QPushButton("Confirm");
    confirm_button->setEnabled(false);

    lo->addWidget(port_list);
    lo->addWidget(refresh_button);
    lo->addWidget(confirm_button);

    refreshPortList();

    connect(refresh_button,&QPushButton::clicked,this, &SerialWaiterDialog::refreshPortList);
    connect(port_list,static_cast<void (QComboBox::*)(int n)>(&QComboBox::activated),
            this,&SerialWaiterDialog::selectPort);
    connect(confirm_button,&QPushButton::clicked,this,[=](){this->accept();});
}
