#include <i2c_t3.h>
#include "AudioInfra.h"
#include "Encoder.h"
#include "BoardSupport.h"
#include "DisplayDriver.h"

Encoder *encoders[8];
AudioInfra audioInfra;
DisplayDriver *disp;
int POT_DICT[8] = {3, 2, 1, 0, 7, 6, 5, 4};

void setup()
{
  Serial.begin(112500);
  while(!Serial);
  Serial.print("serial has begun\n");
  audioInfra.begin();
  Serial.print("audio infra started\n");
  portSetup();

  disp->init();
  disp->putScreen("0X303", "BEGIN");
  Serial.print("port setup complete\n");

  for (byte i = 0; i < 8; ++i)
  {
    encoders[i] = new Encoder();
    encoders[i]->setList(audioInfra.getParamList(i));
  }
  Serial.print("encoders setup done \n");

  usbMIDI.setHandleNoteOn(handleNoteOn);
  usbMIDI.setHandleNoteOff(handleNoteOff);

  Serial.print("midi setup done\n");
}

void loop()
{
  usbMIDI.read();
  for (byte i = 0; i < 8; i++)
  {
    sendBits(POT_DICT[(int)i]);
    delayMicroseconds(100);
    bool changed = encoders[i]->setReading(digitalRead(E_A), digitalRead(E_B), digitalRead(E_C));
    if (changed)
    {
      float a = encoders[i]->getValue();

      Serial.printf("%s %d %f\n", encoders[i]->getParamName(), i, a);
      disp->putScreen(encoders[i]->getParamName(), a);
      audioInfra.setChanged(i);
    }
  }
}

void handleNoteOn(byte channel, byte note, byte velocity)
{
  // Serial.printf("%d, %d, %d\n", channel, note, velocity);
  audioInfra.noteOn(channel, note, velocity);
}

void handleNoteOff(byte channel, byte note, byte velocity)
{
  audioInfra.noteOff(channel, note, velocity);
}
