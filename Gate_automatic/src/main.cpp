#include <sense.h>

sense hg;

void setup()
{
    hg.pinSets();
}

void loop()
{
    hg.senLoop();
}