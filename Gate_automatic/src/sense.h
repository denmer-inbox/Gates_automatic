#include <Arduino.h>
#include <HardwareSerial.h>

//  Pins table:
//
//  10 -> 6 BH      11 -> 6 loop1
//  10 -> 7 BB      11 -> 7 loop2
//  10 -> 8 HH      11 -> 8 loop3
//  10 -> 9 HB      11 -> 9 sense

class sense
{

public:
    struct ports
    {
        boolean bh, bb, hh, hb, sense, loop1, loop2, loop3;
    };

private:
    uint8_t pinGroup = 10;
    uint8_t pin = 0;
    unsigned long pinsTimeout;
    boolean Fl[3];
    boolean pins[3];
    unsigned long timer;
    ports p;

public:
    void senLoop()
    {
        pinGr();

       /*  Serial.print(pins[0]);
        Serial.print(pins[1]);
        Serial.print(pins[2]);
        Serial.println(); */

        pinMode(10, (pins[0] ? OUTPUT : INPUT_PULLUP));
        pinMode(11, (pins[1] ? OUTPUT : INPUT_PULLUP));
        pinMode(12, (pins[2] ? OUTPUT : INPUT_PULLUP));

        digitalWrite(10, (!pins[0]));
        digitalWrite(11, (!pins[1]));
        digitalWrite(12, (!pins[2]));

/*         Serial.print(digitalRead(10));
        Serial.print(digitalRead(11));
        Serial.print(digitalRead(12));
        Serial.println(); */

        p.bh = (pins[0] && !digitalRead(7));
        p.bb = (pins[0] && !digitalRead(6));
        p.hh = (pins[0] && !digitalRead(9));
        p.hb = (pins[0] && !digitalRead(8));

        p.loop1 = (pins[1] && !digitalRead(6));
        p.loop2 = (pins[1] && !digitalRead(7));
        p.loop3 = (pins[1] && !digitalRead(8));
        p.sense = (pins[1] && !digitalRead(9));

        
            Serial.print((p.bh)?"BH ":"   ");
      
            Serial.print((p.bb)?"BB ":"   ");
       
            Serial.print((p.hh)?"HH ":"   ");
       
            Serial.print((p.hb)?"HB ":"   ");


            Serial.print((p.sense)?"SENSE ":"      ");
       
            Serial.print((p.loop1)?"LOOP-1 ":"       ");
      
            Serial.print((p.loop2)?"LOOP-2 ":"       ");
        
            Serial.print((p.loop3)?"LOOP-3 ":"       ");

            Serial.println();
    }

private:
    void pinGr()
    {
        pin = (pin >= 2) ? 0 : pin + 1;

        for (int pin1 = 0; pin1 < 3; pin1++)
        {
            pins[pin1] = (pin1 == pin) ? HIGH : LOW;
        }
    }

public:
    void pinSets()
    {
        Serial.begin(115200);

        pinMode(6, INPUT_PULLUP);
        pinMode(7, INPUT_PULLUP);
        pinMode(8, INPUT_PULLUP);
        pinMode(9, INPUT_PULLUP);

        digitalWrite(10, OUTPUT);
        digitalWrite(11, OUTPUT);
        digitalWrite(12, OUTPUT);
    }
};
