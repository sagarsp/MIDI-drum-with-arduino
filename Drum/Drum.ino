#include<ardumidi.h>

unsigned char padNote[6] = {60,16,66,63,40,65};

int padThresholdValue[6] = {50,50,50,50,50,50};
int maximumPlayingNoteTime[6] = {90,90,90,90,90,90};

boolean velocityFlag = false;

boolean activePad[6] = {0,0,0,0,0,0};
int pinPlayTime[6] = {0,0,0,0,0,0};

unsigned char status;

int pin = 0;
int hitAvg = 0;
int note_on = 0;

/***Setup***/

void setup()
{
  Serial.begin(115200);
}

/***Main Function***/

void loop()
{
  for(int pin=0; pin < 6; pin++)
  {
    hitAvg = analogRead(pin);
    hitAvg = 1023 - hitAvg;
    
    if(hitAvg > padThresholdValue[pin])
    {
      if(activePad[pin] == false)
      {
        if(velocityFlag == true)
        {
          hitAvg = (hitAvg / 8) - 1;
        }
        else
        {
          hitAvg = 127;
        }
        midi_note_on(0, padNote[pin], hitAvg);
        Serial.write(padNote[pin]);
        pinPlayTime[pin] = 0;
        activePad[pin] = true;
      }
      else
      {
        pinPlayTime[pin] = pinPlayTime[pin] + 1;
      }
    }
    else if(activePad[pin] == true)
    {
      pinPlayTime[pin] = pinPlayTime[pin] + 1;
      
      if(pinPlayTime[pin] > maximumPlayingNoteTime[pin])
      {
        activePad[pin] = false;
        midi_note_off(0, padNote[pin], 127);
        Serial.write(0x80);
      }
    }
  }
}
