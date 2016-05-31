#ifndef WIDGET_H
#define WIDGET_H
#include "ui_widget.h"

#include <QWidget>
#include <QtGui>
#include <QtSerialPort>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    void setupSerial();
    void setupEngines();
    void setupLight();
    void setupElectromagnet();
    void setupMusic();

    void updateSpeedSliders();
    void updateSpeedLCDs();
    void updateSpeedDisplays();

    virtual void keyPressEvent(QKeyEvent * event);
    Ui::Widget *ui;
    QSerialPort carPort;
    QString user_music;
    bool electromagnet_enabled;
    bool light_enabled;

    int left_motor_speed;
    int left_motor_forward;
    int right_motor_speed;
    int right_motor_forward;

    bool music_selected;
    bool music_playing;

    char OutMessage[1024];

private slots:
    void resetMotors();
    void exit();
    void carAcceleration();
    void carBreaking();
    void change_left_motor_speed(int val);
    void change_right_motor_speed(int val);
    void carTurnLeft();
    void carTurnRight();
    void writeMovementParams();

    void fillMusicList();
    void selectMusic(int selection_index);
    void uploadMusic();
    void musicToggle();

    void lightToggle();
    void electromagnetToggle();
};


#endif // WIDGET_H
