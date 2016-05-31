#include "opcodes.h"

/*----+----+----+----PINS---+---+----+*/
#define LIGHT_PIN 2
#define SPEAKER_PIN 3

#define R_DC_PA 6
#define R_DC_P1 5
#define R_DC_P2 4

#define L_DC_PA 9
#define L_DC_P1 7
#define L_DC_P2 8
/*-----------------PINS END----------*/

#define MAX_MELODY_LENGTH 100

int melody_size;
char notes[MAX_MELODY_LENGTH];
char note_length[MAX_MELODY_LENGTH];
unsigned long note_change_time;
bool melody_playing;
char current_note;

bool right_dc_dir_forward;
bool left_dc_dir_forward;
bool car_accelerating;
bool car_braking;
bool car_turning_left;
bool car_turning_right;
int car_left_dc_speed;
int car_right_dc_speed;

/*********DC CONTROL*******/
void L_DC_stop(){
  digitalWrite(L_DC_P1, LOW);
  digitalWrite(L_DC_P2, LOW);
}
void R_DC_stop(){
  digitalWrite(R_DC_P1, LOW);
  digitalWrite(R_DC_P2, LOW);
}

void L_DC_rotateBackward(){
  left_dc_dir_forward = false;
  digitalWrite(L_DC_P1, LOW);
  digitalWrite(L_DC_P2, HIGH);
}
void R_DC_rotateBackward(){
  right_dc_dir_forward = false;
  digitalWrite(R_DC_P1, LOW);
  digitalWrite(R_DC_P2, HIGH);
}

void L_DC_rotateForward(){
  left_dc_dir_forward = true;
  digitalWrite(L_DC_P1, HIGH);
  digitalWrite(L_DC_P2, LOW);
}
void R_DC_rotateForward(){
  right_dc_dir_forward = true;
  digitalWrite(R_DC_P1, HIGH);
  digitalWrite(R_DC_P2, LOW);
}

void L_DC_setSpeed(int speed){
  if(speed < 0 && left_dc_dir_forward){
    speed = -speed;
    L_DC_rotateBackward();
  }
  else if(speed >= 0 && !left_dc_dir_forward)
    L_DC_rotateForward();
  analogWrite(L_DC_PA, speed);
}
void R_DC_setSpeed(int speed){
  if(speed < 0 && right_dc_dir_forward){
    //speed = -speed;
    R_DC_rotateBackward();
  }
  else if(speed >= 0 && !right_dc_dir_forward)
    R_DC_rotateForward();
  Serial.println(speed,DEC);
  analogWrite(R_DC_PA, speed);
}

void setup_DC(){
  pinMode(LIGHT_PIN, OUTPUT);
  
  pinMode(R_DC_PA, OUTPUT);
  pinMode(R_DC_P1, OUTPUT);
  pinMode(R_DC_P2, OUTPUT);
  
  pinMode(L_DC_PA, OUTPUT);
  pinMode(L_DC_P1, OUTPUT);
  pinMode(L_DC_P2, OUTPUT);

  //Stop all DCs
  L_DC_rotateForward();
  R_DC_rotateForward();
  L_DC_setSpeed(0);
  R_DC_setSpeed(0);
  car_left_dc_speed = 0;
  car_right_dc_speed = 0;
}
/******DC CONTROL END*****/

//TODO: use bw^(note/12)
int note_to_freq(char note){
  return 0+note * 30;
}

void update_note(){
  unsigned long time_now = millis();
  if(time_now >= note_change_time) {
    tone(SPEAKER_PIN,
    note_to_freq(notes[current_note]));
    note_change_time = time_now + (int)(note_length[current_note]) * 6;
    current_note++;
    current_note%=melody_size;
  }
}

void setup_melody(){
  notes[0] = 0;
  current_note = 0;
  melody_size = 0;
  note_change_time = 0;
}

void setup() {
setup_DC();
Serial.begin(9600);
current_note = 0;
melody_size = 0;
}

void loadMelody(){
  current_note = 0;
  while(!Serial.available());//Wait for melody length
  melody_size = Serial.read();
 
  for(int i = 0; i < melody_size; i++){
    while(!Serial.available());//Wait for note
    notes[i] = Serial.read();
    while(!Serial.available());//Wait for length
    note_length[i] = Serial.read();
  }
  note_change_time = millis();
}

void loadMovementParams() {
  int speed_l, speed_r, dir_l, dir_r;
  
  while(Serial.available() < 4);
  
  speed_l = Serial.read();
  dir_l = Serial.read();
  speed_r = Serial.read();
  dir_r = Serial.read();
  
  L_DC_setSpeed(speed_l);
  R_DC_setSpeed(speed_r);

  dir_l == 0 ? L_DC_rotateForward() : L_DC_rotateBackward();
  dir_r == 0 ? R_DC_rotateForward() : R_DC_rotateBackward();
}

void loop() {
  if(Serial.available()){
    switch(Serial.read()){
     case OP_MOVE: loadMovementParams(); break;
     case OP_LOAD_MUSIC: loadMelody(); break;
     case OP_PLAY_MUSIC: melody_playing = true;  noTone(SPEAKER_PIN); break;
     case OP_STOP_MUSIC: melody_playing = false; noTone(SPEAKER_PIN); break;
     case OP_LED_ON:  digitalWrite(LIGHT_PIN, HIGH); break;
     case OP_LED_OFF: digitalWrite(LIGHT_PIN, LOW); break;
    }
  }
  if(melody_playing)
    update_note();
}
