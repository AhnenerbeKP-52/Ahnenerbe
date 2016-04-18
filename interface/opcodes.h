#ifndef _OPCODES_H_
#define _OPCODES_H_

enum OPCode {
  OP_STOP_ALL = -1,
  OP_MOVE = 0,
  OP_LOAD_MUSIC = 1,
  OP_PLAY_MUSIC = 2,
  OP_STOP_MUSIC = 3,
  OP_LED_ON = 4,
  OP_LED_OFF = 5,
  OP_CARGO_ON  = 6,
  OP_CARGO_OFF = 7,
};

#endif
