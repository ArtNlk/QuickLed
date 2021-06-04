#include <FastLED.h>
FASTLED_USING_NAMESPACE

#define FASTLED_ALLOW_INTERRUPTS 0
#define DATA_PIN    51
//#define CLK_PIN   4
#define LED_TYPE    WS2812
#define COLOR_ORDER BRG
#define NUM_LEDS    131
CRGB leds[NUM_LEDS];

byte command[256];
byte code = -1;
bool ready = true;
bool issetup = false;
bool reading = false;
bool executing = false;
bool autoupdate = true;
int length = -1;
unsigned int pointer = 0;

void setup() {
  delay(500); // 0.5 second delay for recovery

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  Serial.begin(115200);
}

void loop() {

  if (ready)
  {
    Serial.write(1);
    length = -1;
    code = -1;
    pointer = 0;
    ready = false;
    issetup = true;
    reading = false;
    executing = false;
  }

  if (issetup and Serial.available() >= 2)
  {
    code = Serial.read(); //0 - set leds, 1 - set autoupdate, 2 - read strip, 3 - fill strip 4 - update
    length = Serial.read();
    if(length != 0)
    {
    issetup = false;
    reading = true;
    }
    else
    {
    issetup = false;
    executing = true;
    }
  }

  if (reading and Serial.available() != 0)
  {
    if (code != 0)
    {
      command[pointer] = Serial.read();
      pointer++;

      if (pointer == length)
      {
        reading = false;
        executing = true;
      }
    }
    else
    {
      if (Serial.available() >= 3)
      {
        int r = Serial.read();
        int g = Serial.read();
        int b = Serial.read();
        CRGB color = CRGB(r, g, b);

        leds[pointer] = color;
        pointer++;
        if (pointer == NUM_LEDS)
        {
          reading = false;
          if(autoupdate){FastLED.show();}
          ready = true;
        }
      }
    }
  }

  if (executing)
  {
      switch (code)
    {
      case 1:
      if (command[0] > 0)
      {
        autoupdate = true;
        ready = true;
      }
      else
      {
        autoupdate = false;
        ready = true;
      }
      break;
      case 2:
      FastLED.setBrightness(command[0]);
      if(autoupdate){FastLED.show();}
      ready = true;
      break;

      case 3:
      fill_solid(leds,NUM_LEDS, CRGB(command[0], command[1], command[2]));
      if(autoupdate){FastLED.show();}
      ready = true;
      break;

      case 4:
      FastLED.show();
      ready = true;
      break;
    }
  }
}
