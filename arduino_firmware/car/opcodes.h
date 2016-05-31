#ifndef _OPCODES_H_
#define _OPCODES_H_

#define OPCODES_VERSION 15_04_2016_14_17

enum OPCode {
  OP_STOP_ALL = 0,
  OP_MOVE = 1,
  OP_LOAD_MUSIC = 2,
  OP_PLAY_MUSIC = 3,
  OP_STOP_MUSIC = 4,
  OP_LED_ON = 5,
  OP_LED_OFF = 6,
  OP_CARGO_ON  = 7,
  OP_CARGO_OFF = 8,
};

#endif

