#include "sense.h"

void sense::senLoop()
{
    if (millis() % 100)
    {
        pinGr();
    }
}

void sense::pinGr()
{
    pinGroup++;
    if (pinGroup > 12)
        pinGroup = 10;

    uint8_t step = 0;

    while (step < 3)
    {
        pins[step] = !(pinGroup == step + 10);
        digitalWrite(pinGroup, !(pinGroup == step + 10));
        step++;
    }

    pinMode(10, OUTPUT);    // п и н ы  на   в ы х о д
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);

    digitalWrite(10, pins[0]);
    digitalWrite(11, pins[1]);
    digitalWrite(12, pins[2]);
    delay(20);
}

void sense::logic()
{
    pinMode(6,INPUT_PULLUP);
    pinMode(7,INPUT_PULLUP);
    pinMode(8,INPUT_PULLUP);
    pinMode(9,INPUT_PULLUP);
    
    if (!digitalRead(11))
    {
        if (!digitalRead(6))
        {
            Serial.println("loop1");
        }

        if (!digitalRead(7))
        {
            Serial.println("loop2");
        }

        if (!digitalRead(8))
        {
            Serial.println("loop3");
        }

        if (!digitalRead(9))
        {
            Serial.println("кнопка пульт");
        }
    }

    if (!digitalRead(10) == 100)
    {
        if (!digitalRead(6))
        {
            Serial.println("BH");
        }

        if (!digitalRead(7))
        {
            Serial.println("BB");
        }

        if (!digitalRead(8))
        {
            Serial.println("HH");
        }

        if (!digitalRead(9))
        {
            Serial.println("HB");
        }
    }
}