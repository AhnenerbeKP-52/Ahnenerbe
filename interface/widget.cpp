#include "widget.h"
#include "time.h"
#include "ui_widget.h"
//include <Windows.h>
#include <cstdlib>
#include <iostream>
#include "serialwaiterdialog.h"
#include<windows.h>

#include <QtSerialPort/QSerialPortInfo>

#include <Qt>

void Widget::keyPressEvent(QKeyEvent * event){

    switch(event->key()){
    case Qt::Key_W:
       // ui->pushButton->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red; }");
       // Sleep(10);
       // ui->pushButton->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }");
        carAcceleration();
        break;
    case Qt::Key_A:
        //ui->textEdit->setText("a");
        carTurnLeft();
        break;
    case Qt::Key_S:
        //ui->textEdit->setText("s");
        carBraking();
        break;
    case Qt::Key_D:
       // ui->textEdit->setText("d");
        carTurnRight();
        break;

    case Qt::Key_Escape:
    carStatus();
    break;
    }
}


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    /*Serial port setup*/
    SerialWaiterDialog swd(this);
    swd.exec();
    QString port_name = swd.get_selection();
    if(port_name.isNull()) std::exit(1);

    carPort.setPortName(port_name);
    carPort.setBaudRate(QSerialPort::Baud9600);
    carPort.open(QIODevice::ReadWrite);


    carState=false;
    programCarSpeed_motor1=50;
    programCarSpeed_motor2=50;
    motor1_direction=1;
    motor2_direction=1;

    headlightState = false;
    lineDriveMode = false;
    electromagnetState=false;

    ui->setupUi(this);
    connect(ui->checkBox_electromagnet,&QCheckBox::stateChanged,this,[=](){electromagnetState = !electromagnetState;});
    connect(ui->checkBox_headlight,&QCheckBox::stateChanged,this,[=](){headlightState = !headlightState;});
    connect(ui->checkBox_lineDrive,&QCheckBox::stateChanged,this,[=](){lineDriveMode = !lineDriveMode;});

    QObject::connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(carStatus()));
    QObject::connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(exit()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(carAcceleration()));
    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(carBraking()));
    QObject::connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(carTurnRight()));
    QObject::connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(carTurnLeft()));
    QObject::connect(ui->verticalSlider, SIGNAL(valueChanged(int)), this, SLOT(cangeCarProgramSpeed1()));
    QObject::connect(ui->verticalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(cangeCarProgramSpeed2()));

    QObject::connect(ui->refresh_button, SIGNAL(clicked()), this, SLOT(fillMusicList()));
    QObject::connect(ui->melody_list,SIGNAL(activated(QString)),this,SLOT(SelectMusic()));
    QObject::connect(ui->play_button,SIGNAL(clicked()),this,SLOT(PlayMusic()));

    ui->pushButton->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->pushButton_2->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->pushButton_3->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->pushButton_4->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->pushButton_5->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->pushButton_7->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->play_button->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");
    ui->refresh_button->setStyleSheet("QPushButton{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: gray; }"
"QPushButton:hover{background-color: lightgrey; border-style: outset; border-width: 5px; border-color: red;}");

    ui->verticalSlider->setValue(programCarSpeed_motor1);
    ui->verticalSlider_2->setValue(programCarSpeed_motor2);
    ui->verticalSlider->setRange(0,100);
    ui->verticalSlider_2->setRange(0,100);

    fillMusicList();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::carStatus()
{
    carState = !carState;
    if(carState){
        programCarSpeed_motor1=50;
        programCarSpeed_motor2=50;
        ui->verticalSlider->setValue(programCarSpeed_motor1);
         ui->verticalSlider_2->setValue(programCarSpeed_motor2);
         ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
         ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);
     ui->textEdit->setText("Reset");

    }
    else {
        programCarSpeed_motor1=50;
        programCarSpeed_motor2=50;
        ui->verticalSlider->setValue(programCarSpeed_motor1);
         ui->verticalSlider_2->setValue(programCarSpeed_motor2);
         ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
         ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);
        ui->textEdit->setText("Reset");

    }
ArduinoOut();
}
/*
void Widget::electromagnetStatus()
{
    electromagnetState = !electromagnetState;
    if(electromagnetState){
     ui->textEdit_5->setText("On");

    }
    else {
        ui->textEdit_5->setText("Off");

    }

}
*/
void Widget::exit()
{
    QApplication::exit();

}

void Widget::carAcceleration()
{

        programCarSpeed_motor1+=5;
        programCarSpeed_motor2+=5;

        if(programCarSpeed_motor1>100){
            programCarSpeed_motor1=100;
        }
        if(programCarSpeed_motor2>100){
          programCarSpeed_motor2=100;
       }

        ui->verticalSlider->setValue(programCarSpeed_motor1);
         ui->verticalSlider_2->setValue(programCarSpeed_motor2);
    ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
    ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);


    ArduinoOut();

   // ui->label_10->setText(QString("%1").arg(programCarSpeed));
}

void Widget::carBraking()
{

    programCarSpeed_motor1-=5;
    programCarSpeed_motor2-=5;

        if(programCarSpeed_motor1<0){
            programCarSpeed_motor1=0;
        }
        if(programCarSpeed_motor2<0){
            programCarSpeed_motor2=0;
        }

        ui->verticalSlider->setValue(programCarSpeed_motor1);
         ui->verticalSlider_2->setValue(programCarSpeed_motor2);
     ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
     ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);

ArduinoOut();

}

void Widget::cangeCarProgramSpeed1()
{
    programCarSpeed_motor1 = ui->verticalSlider->value();
     ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
     ArduinoOut();
    // programCarSpeed_motor2 = ui->verticalSlider_2->value();
}

void Widget::carTurnLeft()
{
     programCarSpeed_motor2+=5;
     programCarSpeed_motor1-=5;

     if(programCarSpeed_motor2>100){
     programCarSpeed_motor2=100;
     }
     if(programCarSpeed_motor1<0){
     programCarSpeed_motor1=0;
     }
     ui->verticalSlider->setValue(programCarSpeed_motor1);
      ui->verticalSlider_2->setValue(programCarSpeed_motor2);
  ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
  ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);
  ArduinoOut();
     }


void Widget::carTurnRight()
{
    programCarSpeed_motor2-=5;
    programCarSpeed_motor1+=5;

    if(programCarSpeed_motor1>100){
    programCarSpeed_motor1=100;
    }
    if(programCarSpeed_motor2<0){
    programCarSpeed_motor2=0;
    }
    ui->verticalSlider->setValue(programCarSpeed_motor1);
     ui->verticalSlider_2->setValue(programCarSpeed_motor2);
 ui->lcdNumber->display((programCarSpeed_motor1-50)*2);
 ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);
 ArduinoOut();
}

void Widget::ArduinoOut()
{
    if(programCarSpeed_motor1>=50){
        motor1_direction=1;
    }
    else{
        motor1_direction=0;
    }

    if(programCarSpeed_motor2>=50){
        motor2_direction=1;
    }
    else{
        motor2_direction=0;
    }

    TempSpeed=abs(programCarSpeed_motor1-50)*5;
    OutMessage[0]=0;
    OutMessage[1]=TempSpeed;
    OutMessage[2]=motor1_direction;
    TempSpeed=abs(programCarSpeed_motor2-50)*5;
    OutMessage[3]=TempSpeed;
    OutMessage[4]=motor2_direction;
    //OutMessage[4]=;

    carPort.write(OutMessage,5);
}

void Widget::fillMusicList()
{

    ui->melody_list->clear();
    QDir dir(QDir::currentPath()+"/Melodies");

    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);

    for(int i = 0; i < files.size(); ++i){
        QString curr_file = files.at(i);
        tempStr=curr_file.left(curr_file.indexOf('.'));
         ui->melody_list->addItem(tempStr,curr_file);

    }
    select=false;
    ui->play_button->setEnabled(false);
}

void Widget::SelectMusic()
{
    this->user_music = ui->melody_list->currentData().toString();

    //ui->label_13->setText(user_music);

    select = true;
    ui->play_button->setEnabled(true);
}


void Widget::PlayMusic()
{


    QFile F(QDir::currentPath()+"/Melodies/"+user_music);
    if(!F.open(QIODevice::ReadOnly)){
        ui->label_13->setText(QDir::currentPath()+"/Melodies/"+user_music);
    }
        ;
    QTextStream in(&F);
    OutMessage[0]=1;//LOAD_MUSIC
    int i=2;
    //ui->label_13->setText(user_music);
        while (!in.atEnd())
        {
            //ui->label_13->setText("Kek");
            ui->label_13->setText(QString::number(i));
            //qDebug()<<i;
            OutMessage[i]=in.readLine().toInt();
            // qDebug()<<OutMessage[i];
            ++i;
        }
     OutMessage[1] = (i-2)/2;
     OutMessage[i] = 2; //PLAY_MUSIC
     F.close();

     carPort.write(OutMessage,i);

}


void Widget::cangeCarProgramSpeed2()
{
     programCarSpeed_motor2 = ui->verticalSlider_2->value();
     ui->lcdNumber_2->display((programCarSpeed_motor2-50)*2);
     ArduinoOut();
}
