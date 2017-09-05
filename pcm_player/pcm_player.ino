#include "CurieTimerOne.h"
#include "CurieSerialFlash.h"

int time;
int nextsample = 0;
const int oneSecInUsec = 1000000;   // A second in micro second unit.

SerialFlashFile file;

void setup() {
  // put your setup code here, to run once:
  SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  digitalWrite(4, LOW);
  digitalWrite(5, HIGH);
  analogWriteResolution(12);
  analogWriteFrequency(6, 100000);
  time = oneSecInUsec / 16000;
  CurieTimerOne.start(time, &playNextSample);  // set timer and callback
  file = SerialFlash.open("tune.wav");
}

int idx = 0;

void playNextSample()   // callback function when interrupt is asserted
{
  int16_t sample;
  file.read(&sample, 2);
  analogWrite(6, (sample >> 4) + (127 << 4));
  idx++;
  if (idx >= file.size()) {
    file.seek(0);
  }
  CurieTimerOne.restart(time);
}

void loop() {
  // put your main code here, to run repeatedly:
}


