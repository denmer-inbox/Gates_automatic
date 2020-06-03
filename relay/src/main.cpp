
#include "my_set.h"

void setup()
{
  setPins();
  replaceArr();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Контроллер ворот");

  delay(300);
  Serial.begin(9600);
  rtc.halt(false);
  rtc.writeProtect(false);
  displayTimeout = millis();
  // rtc.setDOW(FRIDAY);        // Set Day-of-Week to FRIDAY
  //  rtc.setTime(12, 0, 0);     // Set the time to 12:00:00 (24hr format)
  //  rtc.setDate(23, 11, 2019); // Set the date to August 6th, 2010
  // rtc.settime(0, 51, 21, 27, 10, 15, 2); // 0  сек, 51 мин, 21 час, 27, октября, 2015 года, вторник
  EEPROM.get(DISPLAYLIGHT, diss.disLight);
  EEPROM.get(DISPLAYPOWER, diss.disOn);
  EEPROM.get(TRAFFICTIME, svetTime);
}

void loop()
{
  if (millis() - displayTimeout > 30000)
    lcd.noBacklight();
  if (millis() - displayTimeout > 60000)
    lcd.noDisplay();

  Loop();
}