#ifndef _INCL_ON
#define _INCL_ON
#include "sense.h"
#endif

#define DISPLAYLIGHT 0
#define DISPLAYPOWER 1
#define TRAFFICTIME 2

boolean mainLCD = true, main1L = false, flcd = true, flagPressBut, flagClrScr;
byte line = 0;
byte svetTime, timeSwitch = 40, stepMenu = 0;
boolean setWorking = true, stepScr = false;
byte screen = 0;
unsigned long timer, displayTimeout = millis();

String LineReg[4];
String lineL[30];
String datEnum[3] = {"День", "Месяц", "Год"};
String timEnum[3] = {"Часы", "Минуты", "Секунды"};
String disEnum[2] = {"Откл. подсветку", "Откл. дисплей"};
byte datEn = 0, timEn = 0, disEn = 0;

LCD_1602_RUS lcd(0x27, 16, 2);
DS1302 rtc(A0, A2, A1);
sense sens;

struct disSetting
{
    byte disLight;
    byte disOn;
};

struct tim
{
    byte thour;
    byte tmin;
    byte tsec;
};

struct dats
{
    byte day;
    byte mounth;
    uint16_t years;
};

byte screenMode = 0;

GButton butt1(6); // SET
GButton butt2(7); // +
GButton butt3(8); // -

tim vremya;
dats data;
disSetting diss;

void Loop(void);
void prt(void);
void FmainLCD(void);
void exitOnScreen(byte, byte);

void replaceArr()
{
    LineReg[0] = (String)svetTime;
    LineReg[1] = setWorking ? "автомат." : "ручное";
    LineReg[2] = (String)timeSwitch;
    LineReg[3] = (String)rtc.getTimeStr();

    lineL[0] = "Время работы";
    lineL[1] = "светофоров " + LineReg[0];
    lineL[2] = "Режим управл.";
    lineL[3] = "воротами " + LineReg[1];

    lineL[4] = "Настройка";
    lineL[5] = "дисплея";

    lineL[6] = "Настройка часов";
    lineL[7] = LineReg[3];

    lineL[8] = "Установка даты";
    lineL[9] = rtc.getDateStr();

    lineL[10] = "Время таймаута";
    lineL[11] = (String)svetTime;

    if (screen == 2)
        lineL[9] = (String)svetTime + " секунд ";

    lineL[12] = timEnum[timEn];
    rtc.getTime();
    if (timEn == 0)
        lineL[13] = vremya.thour;
    if (timEn == 1)
        lineL[13] = vremya.tmin;
    if (timEn == 2)
        lineL[13] = vremya.tsec;
    lineL[13] += " ";

    lineL[14] = datEnum[datEn];
    if (datEn == 0)
        lineL[15] = data.day;
    if (datEn == 1)
        lineL[15] = data.mounth;
    if (datEn == 2)
        lineL[15] = data.years;
    lineL[15] += " ";

    lineL[16] = disEnum[disEn];
    if (disEn == 0)
        lineL[17] = diss.disLight;
    if (disEn == 1)
        lineL[17] = diss.disOn;
    lineL[17] += " ";

    lineL[18] = "Контроль";
    lineL[19] = "датчиков";
    lineL[20] = "Дополн. режим";
    lineL[21] = "работы автомат.";
}

void controlSens()
{
    if (!digitalRead(10))
    {

        if (!digitalRead(7))
        {
            displayTimeout = millis();
            lcd.setCursor(0, 1);
            lcd.printstr("\xFF\xFF");
        }
        else
        {
            lcd.setCursor(0, 1);
            lcd.print("  ");
        }

        if (!digitalRead(8))
        {
            displayTimeout = millis();
            lcd.setCursor(3, 1);
            lcd.printstr("\xFF\xFF");
        }
        else
        {
            lcd.setCursor(3, 1);
            lcd.print("  ");
        }

        if (!digitalRead(9))
        {
            displayTimeout = millis();
            lcd.setCursor(6, 1);
            lcd.printstr("\xFF\xFF");
        }
        else
        {
            lcd.setCursor(6, 1);
            lcd.print("  ");
        }
    }
    delay(10);
}

void setPins()
{

    Serial.begin(9600);
    
    // pinMode(A3, INPUT_PULLUP);
    //  pinMode(12, INPUT_PULLUP);
    //  pinMode(11, INPUT_PULLUP);

    butt1.setDebounce(80);
    butt2.setDebounce(80);
    butt3.setDebounce(80);

    butt1.setTimeout(2000);
    butt1.setClickTimeout(500);
    butt2.setClickTimeout(500);
    butt3.setClickTimeout(500);

    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);

    pinMode(6, INPUT_PULLUP);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);

    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);

    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
}

void Loop()
{
    flagPressBut = false;
    flagClrScr = true;

    sens.senLoop();
    sens.logic();

    butt1.tick();
    butt2.tick();
    butt3.tick();

    if (screen == 0 && !digitalRead(12))
    {
        FmainLCD();

        if (butt2.isClick() || butt3.isClick())
        {
            lcd.display();
            lcd.backlight();
            displayTimeout = millis();
        }

        if (butt1.isClick() && millis() - timer > 2000)
        {
            screen = 1;
            prt();
        }
    }

    if (!digitalRead(12) && (screen == 1 && (millis() - timer >= 2000)))
    {
        if (butt2.isClick() && line + 2 < 19) // +
        {
            line += 2;

            if (line >= 18)
                line = 18;

            if (line > 8 && line < 18)
                line = 18;

            flagPressBut = true;
        }

        if (butt3.isClick() && line - 2 > -2) // -
        {
            line -= 2;

            if (line <= 0)
                line = 0;

            if (line > 8 && line < 18)
                line = 8;

            flagPressBut = true;
        }

        if (butt1.isClick())
        {
            if (line == 0) // таймаут светофоров
            {
                screen = 2;
                line = 10;
                flagPressBut = true;
            }

            if (line == 2) // режим работы (ручн/автомат)
            {
                setWorking = !setWorking;
                flagPressBut = true;
            }

            if (line == 4) // установка дисплея
            {
                screen = 5;
                line = 16;
                flagPressBut = true;
            }

            if (line == 6) // настройка времени
            {
                screen = 3;
                line = 12;
                flagPressBut = true;

                vremya.thour = rtc.getTime().hour;
                vremya.tmin = rtc.getTime().min;
                vremya.tsec = rtc.getTime().sec;
            }

            if (line == 8) // настройка даты
            {
                screen = 4;
                line = 14;
                flagPressBut = true;

                data.day = rtc.getTime().date;
                data.mounth = rtc.getTime().mon;
                data.years = (uint16_t)rtc.getTime().year;
            }

            if (line == 18) // контроль датчиков
            {
                screen = 6;
                line = 20;
                lcd.clear();
                delay(22);
                lcd.setCursor(0, 0);
                lcd.print("BB BH HH HB 123");
            }
        }

        if (butt1.isHolded()) // возврат в основное меню
        {
            screen = 0;
            lcd.clear();
            timer = millis();
            stepScr = true; //добавить на главный экран дату!!!
        }
    }

    if (!digitalRead(12) && screen == 2) // Установка таймаута светофоров
    {
        if (butt2.isClick() && svetTime < 240)
        {
            ++svetTime;
            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt3.isClick() && svetTime > 0)
        {
            --svetTime;
            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt1.isHolded())
        {
            EEPROM.write(TRAFFICTIME, svetTime);
            exitOnScreen(1, 0);
        }
    }

    if (!digitalRead(12) && screen == 3) // установка времени
    {

        if (butt2.isClick()) //++
        {
            if (timEn == 0 && vremya.thour < 24)
                vremya.thour++;

            if (timEn == 1 && vremya.tmin < 60)
                vremya.tmin++;

            if (timEn == 2 && vremya.tsec < 60)
                vremya.tsec++;

            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt3.isClick()) // --
        {
            if (timEn == 0 && vremya.thour > 0)
                vremya.thour--;

            if (timEn == 1 && vremya.tmin > 0)
                vremya.tmin--;

            if (timEn == 2 && vremya.tsec > 0)
                vremya.tsec--;

            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt1.isDouble())
        {
            ++timEn;
            if (timEn > 2)
            {
                rtc.setTime(vremya.thour, vremya.tmin, vremya.tsec);
                timEn = 0;
                exitOnScreen(1, 6);
            }
            flagPressBut = true;
        }
    }

    if (!digitalRead(12) && screen == 4) // установка даты
    {

        if (butt2.isClick()) //++
        {
            if (datEn == 0 && data.day < 31)
                data.day++;

            if (datEn == 1 && data.mounth < 12)
                data.mounth++;

            if (datEn == 2 && data.years < 20000)
                data.years++;

            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt3.isClick()) // --
        {
            if (datEn == 0 && data.day > 0)
                data.day--;

            if (datEn == 1 && data.mounth > 0)
                data.mounth--;

            if (datEn == 2 && data.years > 2000)
                data.years--;

            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt1.isDouble())
        {
            ++datEn;
            if (datEn > 2)
            {
                rtc.setDate(data.day, data.mounth, data.years);
                datEn = 0;
                exitOnScreen(1, 8);
            }
            flagPressBut = true;
        }
    }

    if (!digitalRead(12) && screen == 5) // Установка дисплея
    {
        if (butt2.isClick()) //++
        {
            if (disEn == 0 && diss.disLight < 240)
                diss.disLight++;

            if (disEn == 1 && diss.disOn < 240)
                diss.disOn++;

            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt3.isClick()) // --
        {
            if (disEn == 0 && diss.disLight > 0)
                diss.disLight--;

            if (disEn == 1 && diss.disOn > 0)
                diss.disOn--;

            flagPressBut = true;
            flagClrScr = false;
        }

        if (butt1.isDouble())
        {
            ++disEn;
            if (disEn > 1)
            {
                EEPROM.write(DISPLAYLIGHT, diss.disLight);
                EEPROM.write(DISPLAYPOWER, diss.disOn);
                disEn = 0;
                exitOnScreen(1, 4);
            }
            flagPressBut = true;
        }
    }

    if (screen == 6)
    {
        controlSens();
    }

    if (flagPressBut)
        prt();
}

void exitOnScreen(byte scr, byte lin)
{
    screen = scr;
    line = lin;
    flagPressBut = true;
    timer = millis();
}

void prt()
{
    lcd.display();
    lcd.backlight();
    displayTimeout = millis();
    replaceArr();
    if (flagClrScr)
    {
        lcd.clear();
        delay(22);
    }

    lcd.setCursor(0, 0);
    lcd.print(lineL[line]);
    lcd.setCursor(0, 1);
    lcd.print(lineL[line + 1]);
    flagPressBut = false;
    flagClrScr = true;
}

void FmainLCD()
{
    if (millis() % 1000 == 0)
    { // если прошла 1 секунда
        if (stepScr || millis() - displayTimeout > 15000)
        {
            lcd.setCursor(0, 0);
            lcd.print((String) "Дата: " + rtc.getDateStr());
        }
        lcd.setCursor(0, 1);
        lcd.print((String) "Время: " + rtc.getTimeStr());
        delay(1); // приостанавливаем на 1 мс, чтоб не выводить время несколько раз за 1мс
    }
}
