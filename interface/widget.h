#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <string>
#include <QtSerialPort>

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

    bool carState;
    bool lineDriveMode;
    bool electromagnetState;

    int programCarSpeed_motor1;
    int motor1_direction;

    int programCarSpeed_motor2;
    int motor2_direction;

    int TempSpeed;

    char OutMessage[4];

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

};


#endif // WIDGET_H
