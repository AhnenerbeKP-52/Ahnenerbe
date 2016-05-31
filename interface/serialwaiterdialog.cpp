#include "serialwaiterdialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QLabel>

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
      form(new Ui::Dialog),
      selected(false)
{
    form->setupUi(this);

    connect(form->refresh_button,&QPushButton::clicked,this, &SerialWaiterDialog::refreshPortList);
    connect(form->port_list,SIGNAL(activated(int)), this, SLOT(selectPort(int)));
    connect(form->confirm_button,&QPushButton::clicked,this,[=](){this->accept();});

    refreshPortList();
}
