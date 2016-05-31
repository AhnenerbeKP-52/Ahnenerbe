#include "opcodes.h"

#include "widget.h"
#include "serialwaiterdialog.h"
#include <QtSerialPort/QSerialPortInfo>
#include <Qt>
#include <algorithm>

using std::min;
using std::max;

void Widget::keyPressEvent(QKeyEvent * event){

    switch(event->key()){
    case Qt::Key_W:
        carAcceleration();
        break;
    case Qt::Key_A:
        carTurnLeft();
        break;
    case Qt::Key_S:
        carBreaking();
        break;
    case Qt::Key_D:
        carTurnRight();
        break;

    case Qt::Key_Escape:
        resetMotors();
        break;
    }
}

void Widget::updateSpeedSliders(){
    ui->left_speed_slider->setValue(left_motor_speed);
    ui->right_speed_slider->setValue(right_motor_speed);
}

void Widget::updateSpeedLCDs(){
    ui->left_speed_lcd->display((left_motor_speed-50)*2);
    ui->right_speed_lcd->display((right_motor_speed-50)*2);
}

void Widget::updateSpeedDisplays(){
    updateSpeedSliders();
    updateSpeedLCDs();
}

void Widget::setupEngines(){
    left_motor_speed=50;
    right_motor_speed=50;
    left_motor_forward=1;
    right_motor_forward=1;

    ui->left_speed_slider->setRange(0,100);
    ui->right_speed_slider->setRange(0,100);

    updateSpeedDisplays();

    connect(ui->acceleration_btn,&QPushButton::clicked, this, &Widget::carAcceleration);
    connect(ui->breaking_btn, &QPushButton::clicked, this, &Widget::carBreaking);
    connect(ui->turn_left_btn,&QPushButton::clicked, this, &Widget::carTurnLeft);
    connect(ui->turn_right_btn, &QPushButton::clicked, this, &Widget::carTurnRight);

    connect(ui->left_speed_slider, &QSlider::valueChanged, this, &Widget::change_left_motor_speed);
    connect(ui->right_speed_slider, &QSlider::valueChanged, this, &Widget::change_right_motor_speed);
}

void Widget::setupSerial(){
    SerialWaiterDialog swd(this);

    swd.exec();
    QString port_name = swd.get_selection();

    if(port_name.isNull()) std::exit(1);

    carPort.setPortName(port_name);
    carPort.setBaudRate(QSerialPort::Baud9600);
    carPort.open(QIODevice::ReadWrite);
}

void Widget::setupElectromagnet(){
    electromagnet_enabled=false;
    connect(ui->electromagnet_chbox, &QCheckBox::stateChanged, this, &Widget::electromagnetToggle);
}

void Widget::setupLight(){
    light_enabled=false;
    connect(ui->headlight_chbox, &QCheckBox::stateChanged, this, &Widget::lightToggle);
}

void Widget::setupMusic(){
    connect(ui->melody_list,SIGNAL(currentIndexChanged(int)),this,SLOT(selectMusic(int)));
    connect(ui->refresh_button, &QPushButton::clicked,this, &Widget::fillMusicList);
    connect(ui->upload_button, &QPushButton::clicked,this,&Widget::uploadMusic);
    connect(ui->pause_button, &QPushButton::clicked, this, &Widget::musicToggle);

    music_playing = false;
    fillMusicList();
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    setupSerial();
    ui->setupUi(this);
    setupLight();
    setupEngines();
    setupElectromagnet();
    setupMusic();

    connect(ui->reset_btn,&QPushButton::clicked,this,&Widget::resetMotors);
    QObject::connect(ui->exit_button, SIGNAL(clicked()), this, SLOT(exit()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::resetMotors()
{
    left_motor_speed=50;
    right_motor_speed=50;
    updateSpeedDisplays();
    writeMovementParams();
}

void Widget::exit()
{
    QApplication::exit();

}

void Widget::carAcceleration()
{
    left_motor_speed = min(100,left_motor_speed+5);
    right_motor_speed = min(100,right_motor_speed+5);

    updateSpeedDisplays();

    writeMovementParams();
}

void Widget::carBreaking()
{
    left_motor_speed = max(0,left_motor_speed-5);
    right_motor_speed = max(0,right_motor_speed-5);

    updateSpeedDisplays();

    writeMovementParams();

}

void Widget::carTurnLeft()
{
    left_motor_speed = max(0,left_motor_speed-5);
    right_motor_speed = min(100,right_motor_speed+5);

    updateSpeedDisplays();

    writeMovementParams();
}

void Widget::carTurnRight()
{
    left_motor_speed = min(100,left_motor_speed+5);
    right_motor_speed = max(0,right_motor_speed-5);

    updateSpeedDisplays();

    writeMovementParams();
}

void Widget::writeMovementParams()
{
    left_motor_forward = left_motor_speed>=50;
    right_motor_forward = right_motor_speed>=50;

    int TempSpeed=abs(left_motor_speed-50)*5;//TODO: Don't need abs
    OutMessage[0]=OP_MOVE;
    OutMessage[1]=TempSpeed;
    OutMessage[2]=left_motor_forward;
    TempSpeed=abs(right_motor_speed-50)*5;//TODO: Don't need abs
    OutMessage[3]=TempSpeed;
    OutMessage[4]=right_motor_forward;

    carPort.write(OutMessage,5);
}

void Widget::fillMusicList()
{
    ui->melody_list->clear();
    QDir dir(QDir::currentPath()+"/Melodies");

    QStringList files = dir.entryList(QStringList() << "*.txt", QDir::Files);

    for(int i = 0; i < files.size(); ++i){
        QString curr_file = files.at(i);
        QString tempStr = curr_file.left(curr_file.indexOf('.'));
        ui->melody_list->addItem(tempStr,curr_file);
    }
    music_selected=false;
    ui->upload_button->setEnabled(false);

    if(files.size()==1)
        selectMusic(0);

    ui->melody_list->setCurrentIndex(0);
}

void Widget::selectMusic(int selection)
{
    this->user_music = ui->melody_list->currentData().toString();
    music_selected = true;
    ui->upload_button->setEnabled(true);
}


void Widget::uploadMusic()
{
    QFile F(QDir::currentPath()+"/Melodies/"+user_music);
    if(!F.open(QIODevice::ReadOnly)){
        //TODO:WTF add some error handling
    }
    QTextStream in(&F);
    OutMessage[0]=OP_LOAD_MUSIC;
    int i=2;
    while (!in.atEnd())
    {
        OutMessage[i]=in.readLine().toInt();
        ++i;
    }
    OutMessage[1] = (i-2)/2;
    F.close();

    carPort.write(OutMessage,i);
}

void Widget::musicToggle(){
    OutMessage[0] = music_playing ? OP_STOP_MUSIC : OP_PLAY_MUSIC;
    carPort.write(OutMessage, 1);
    music_playing = !music_playing;
}

void Widget::lightToggle()
{
    OutMessage[0] = light_enabled ? OP_LED_OFF : OP_LED_ON;
    carPort.write(OutMessage, 1);
    light_enabled = !light_enabled;
}

void Widget::electromagnetToggle()
{
    OutMessage[0] = electromagnet_enabled ? OP_CARGO_OFF : OP_CARGO_ON;
    carPort.write(OutMessage, 1);
    electromagnet_enabled = !electromagnet_enabled;
}

void Widget::change_left_motor_speed(int val)
{
    left_motor_speed = val;
    updateSpeedLCDs();
    writeMovementParams();
}

void Widget::change_right_motor_speed(int val)
{
    right_motor_speed = val;
    updateSpeedLCDs();
    writeMovementParams();
}
