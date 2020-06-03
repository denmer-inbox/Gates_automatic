#ifndef _sense
#define _sense

#include <Arduino.h>
#include <HardwareSerial.h>
#include <EEPROM.h>
#include "LCD_1602_RUS.h"
#include "GyverButton.h"
#include "DS1302.h"

//  Pins table:
//
//  10 -> 6 BH      11 -> 6 loop1   12 -> 6 SET
//  10 -> 7 BB      11 -> 7 loop2   12 -> 7 UP/LEFT
//  10 -> 8 HH      11 -> 8 loop3   13 -> 8 DOWN/RIGHT
//  10 -> 9 HB      11 -> 9 пульт   13 -> 9

class sense
{
public:
    uint8_t pinGroup = 10;
    unsigned long pinsTimeout;
    boolean Fl[3];
    uint8_t pins[3];

 public:
    void senLoop();
    void pinGr();
    void logic();
};

#endif