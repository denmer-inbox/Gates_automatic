#ifndef _DEF_H
#define _DEF_H

#include "def.h"

#endif

#define timerTraf 20000

extern bool gUOpen, gUClose, gDOpen, gDClose, loop1, loop2, loop3, sensorWait;

LCD_1602_RUS lcd(0x27, 16, 2);

DS1302 rtc(A3, A4, A5);

Gates classGates;

void setup()
{
#pragma region setup

  rtc.halt(false);
  rtc.writeProtect(false);
  rtc.setDOW(FRIDAY);
  rtc.setDate(10, 11, 2019);
  rtc.setTime(22, 0, 0);

  settingPins(); // выставляем состояния пинов IO

  classGates.getPosGates(); //получаем значения пинов в переменные boolean

  Serial.begin(9600);
  lcd.begin();

  lcd.setBacklight(1); //  Включаем подсветку LCD дисплея
  lcd.setCursor(0, 0); //  Устанавливаем курсор в позицию (0 столбец, 0 строка)
  lcd.print("KOHTPOЛЛЕP BOPOT");

  delay(4000);

  lcdMain();
#pragma endregion
}

void loop()
{

  if (!getSensor && posGates.sensorWait == 0) //если кнопка пульта верхн ворот нажата
  {
    posGates.sensorWait = 1;           // блокирует повторное использования этого условия
    posGates.gUOpen = 1;               // передача управления условию ожидания открытия верхн ворот
    traffic(1);                        // вкл. светофор проезд вниз
    posGates.timerTraffic = timerTraf; // устанолен таймер работы светофора
    delayTime = millis();              // время начала отсчета таймера
  }

  if (posGates.gUOpen == 1) // ожидание открытия верхн ворот
  {
    if (millis() - delayTime > posGates.timerTraffic) // если истекло время таймера
    {
      if (!getUpClose)
      {
        delayTime = millis();         // и верхние ворота не закрыты, добавляем к таймеру 6 сек.
        posGates.timerTraffic = 6000; //
      }
    }
  }
  if (!getUpOpen) //если сработал датчик - верхн ворота открыты
  {
    relayDownGatesH; // открываем нижние ворота
  }
}


void lcdMain()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("текущее время");
}