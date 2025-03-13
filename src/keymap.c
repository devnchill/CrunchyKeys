#include "./include/keymap.h"
char *get_key_name(uint32_t key) {
  switch (key) {
  case KEY_A:
    return "A";
  case KEY_B:
    return "B";
  case KEY_C:
    return "C";
  case KEY_D:
    return "D";
  case KEY_E:
    return "E";
  case KEY_F:
    return "F";
  case KEY_G:
    return "G";
  case KEY_H:
    return "H";
  case KEY_I:
    return "I";
  case KEY_J:
    return "J";
  case KEY_K:
    return "K";
  case KEY_L:
    return "L";
  case KEY_M:
    return "M";
  case KEY_N:
    return "N";
  case KEY_O:
    return "O";
  case KEY_P:
    return "P";
  case KEY_Q:
    return "Q";
  case KEY_R:
    return "R";
  case KEY_S:
    return "S";
  case KEY_T:
    return "T";
  case KEY_U:
    return "U";
  case KEY_V:
    return "V";
  case KEY_W:
    return "W";
  case KEY_X:
    return "X";
  case KEY_Y:
    return "Y";
  case KEY_Z:
    return "Z";
  case KEY_0:
    return "0";
  case KEY_1:
    return "1";
  case KEY_2:
    return "2";
  case KEY_3:
    return "3";
  case KEY_4:
    return "4";
  case KEY_5:
    return "5";
  case KEY_6:
    return "6";
  case KEY_7:
    return "7";
  case KEY_8:
    return "8";
  case KEY_9:
    return "9";
  case KEY_SPACE:
    return "SPACE";
  case KEY_ENTER:
    return "ENTER";
  case KEY_BACKSPACE:
    return "BACKSPACE";
  case KEY_TAB:
    return "TAB";
  case KEY_ESC:
    return "ESC";
  case KEY_CTRL:
    return "CTRL";
  case KEY_SHIFT:
    return "SHIFT";
  case KEY_ALT:
    return "ALT";
  case KEY_CAPSLOCK:
    return "CAPSLOCK";
  case KEY_F1:
    return "F1";
  case KEY_F2:
    return "F2";
  case KEY_F3:
    return "F3";
  case KEY_F4:
    return "F4";
  case KEY_F5:
    return "F5";
  case KEY_F6:
    return "F6";
  case KEY_F7:
    return "F7";
  case KEY_F8:
    return "F8";
  case KEY_F9:
    return "F9";
  case KEY_F10:
    return "F10";
  case KEY_F11:
    return "F11";
  case KEY_F12:
    return "F12";
  case KEY_ARROW_UP:
    return "UP";
  case KEY_ARROW_DOWN:
    return "DOWN";
  case KEY_ARROW_LEFT:
    return "LEFT";
  case KEY_ARROW_RIGHT:
    return "RIGHT";
  case KEY_DELETE:
    return "DELETE";
  case KEY_INSERT:
    return "INSERT";
  case KEY_HOME:
    return "HOME";
  case KEY_END:
    return "END";
  case KEY_PAGEUP:
    return "PAGE UP";
  case KEY_PAGEDOWN:
    return "PAGE DOWN";
  default:
    return "UNKNOWN";
  }
}
