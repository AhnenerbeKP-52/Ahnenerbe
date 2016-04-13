#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <string>
#include <QtSerialPort>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    virtual void keyPressEvent(QKeyEvent * event);
    //virtual void sliderMoved();
    Ui::Widget *ui;
    QSerialPort carPort;
    QString user_music;
    QString tempStr;
    //fstream F;

    bool carState;
    bool headlightState;
    bool lineDriveMode;
    bool electromagnetState;

    int programCarSpeed_motor1;
    int motor1_direction;

    int programCarSpeed_motor2;
    int motor2_direction;

    int TempSpeed;
    bool select;

    char OutMessage[1024];

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void carStatus();
    void exit();
    void carAcceleration();
    void carBraking();
    void cangeCarProgramSpeed1();
    void cangeCarProgramSpeed2();
    void carTurnLeft();
    void carTurnRight();
    void ArduinoOut();
    void fillMusicList();
    void SelectMusic();
    void PlayMusic();

};

#endif // WIDGET_H
