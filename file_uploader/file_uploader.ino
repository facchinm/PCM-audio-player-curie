#include <CurieSerialFlash.h>
#include <SPI.h>

const char *filename = "tune.wav";
SerialFlashFile file;

#define filesize 1135794

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // wait for Arduino Serial Monitor
  	while (!Serial);
  delay(100);

  // Init. SPI Flash chip
  if (!SerialFlash.begin(ONBOARD_FLASH_SPI_PORT, ONBOARD_FLASH_CS_PIN)) {
        Serial.println("Unable to access SPI Flash chip");
  }

  SerialFlash.eraseAll();
  while (!SerialFlash.ready());

  // Create the file if it doesn't exist
  if (!create_if_not_exists(filename)) {
    Serial.println("Not enough space to create file " + String(filename));
    return;
  }

  // Open the file and write test data
  file = SerialFlash.open(filename);
}


bool create_if_not_exists (const char *filename) {
  if (!SerialFlash.exists(filename)) {
    Serial.println("Creating file " + String(filename));
    return SerialFlash.create(filename, filesize);
  }

  Serial.println("File " + String(filename) + " already exists");
  return true;
}

int idx = 0;

void loop() {
  if (Serial.available()) {
    char c = Serial.read();
    file.write(&c, 1);
    idx++;
  }
  if (idx % 10000 == 0) {
    Serial.println("Sent " + String(idx) + " bytes");
  }
  if (idx == filesize) {
    Serial.println("done");
    while (1);
  }
}
