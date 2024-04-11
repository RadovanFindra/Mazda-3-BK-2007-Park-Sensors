#include <TFT_eSPI.h>
#include "SPI.h"
#include <PNGdec.h>
#include "Mazda3.h"
#include "Wire.h"


TFT_eSPI tft = TFT_eSPI();

PNG png;
#define MAX_IMAGE_WIDTH 240  // Adjust for your images
int16_t xpos = 0;
int16_t ypos = 0;

//Sensor vars
volatile long duration = 0;
uint16_t writeByteBuffered = 0;
uint16_t writeMode = 0;
uint16_t readMode = 0;
long cm;
static uint32_t _pulseBegin;

uint8_t _interruptPin = 35;

typedef struct sensor {
  uint8_t trig;
  volatile int duration;
  volatile bool waiting;
  sensor *next;
} sensor;

sensor sensors[5];

sensor *current;


void setup() {
  Serial.begin(115200);
  Wire.begin();

  tft.begin();
  tft.setRotation(3);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  int16_t rc = png.openFLASH((uint8_t *)Mazda_3_BK_Back__ParkL, sizeof(Mazda_3_BK_Back__ParkL), pngDraw);
  
  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    
    tft.startWrite();
    
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);

    Serial.print(millis() - dt);
    Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  } else {
    Serial.printf("png.open() failed, rc=%d\n", rc);
  }

  xpos = 161;
  rc = png.openFLASH((uint8_t *)Mazda_3_BK_Back__ParkR, sizeof(Mazda_3_BK_Back__ParkR), pngDraw);

  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    
    tft.startWrite();
    
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);

    Serial.print(millis() - dt);
    Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  } else {
    Serial.printf("png.open() failed, rc=%d\n", rc);
  }

  for (int i = 0; i < sizeof(sensors) / sizeof(sensors[0]); ++i) {
    sensors[i].duration = 0;
    sensors[i].trig = i * 2;
    //sensors[i].trig = 2;

    Mode(sensors[i].trig, OUTPUT);
    Mode(sensors[i].trig + 1, OUTPUT);
    Write(sensors[i].trig + 1, HIGH);
    sensors[i].waiting = false;
    if (i < sizeof(sensors) / sizeof(sensors[0]) - 1) {
      sensors[i].next = &sensors[i + 1];  // Nastavíme ukazatel next na následující prvek v poli senzorů
    } else {
      sensors[i].next = &sensors[0];  // Pokud jsme na posledním prvku, next nastavíme na první prvek
    }
  }
  current = &sensors[0];
  pinMode(_interruptPin, INPUT_PULLUP);
}

void loop(void) {
  if (!current->waiting) {
    doSonar();
  }
  tft.setTextSize(1);
  int x = 5;
  int y = 10;
  tft.setTextPadding(20);
  tft.drawNumber(sensors[0].duration, x, y);  // Left Aligned
  x = 50;
  y = 203;
  tft.setTextPadding(20);
  tft.drawNumber(sensors[1].duration, x, y);  // Left Aligned
  x = 250;
  y = 203;
  tft.setTextPadding(20);
  tft.drawNumber(sensors[2].duration, x, y);  // Left Aligned
   x = 150;
   y = 203;
   tft.setTextPadding(20);
   tft.drawNumber(sensors[3].duration, x, y);  // Left Aligned
   x = 300;
   y = 10;
   tft.setTextPadding(20);
   tft.drawNumber(sensors[4].duration, x, y);  // Left Aligned
}

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void doSonar() {
  if (digitalRead(_interruptPin) == LOW) return;  // interrupt pin is active - skip
  send_ping();
}

void send_ping() {
  //Serial.println("ping");
  _pulseBegin = 0;                                                             // cleared until we see it
  attachInterrupt(digitalPinToInterrupt(_interruptPin), startPulse, FALLING);  // NOR gate - pulse is inverted
  current->waiting = true;
  Write(current->trig, HIGH);
  delayMicroseconds(10);
  Write(current->trig, LOW);
}

void startPulse() {
  _pulseBegin = micros();  // pulse is starting now
  attachInterrupt(digitalPinToInterrupt(_interruptPin), endPulse, RISING);
  //Serial.println("pulse start");
}

void endPulse() {
  uint32_t now = micros();
  detachInterrupt(digitalPinToInterrupt(_interruptPin));  // clean up after ourselves
  // ignore wacko values
  if (now < _pulseBegin) return;                           // we started in the future - micros rollover or the like
  current->duration = (((now - _pulseBegin) / 2) / 29.1);  // calculate length of pulse
  current->waiting = false;
  current = current->next;
}

void Mode(uint8_t pin, uint8_t mode) {
  if (mode == OUTPUT) {
    writeMode = writeMode | bit(pin);
    readMode = readMode & ~bit(pin);

  } else if (mode == INPUT) {
    writeMode = writeMode & ~bit(pin);
    readMode = readMode | bit(pin);
  }
};

void Write(uint8_t pin, uint8_t value) {
  Wire.beginTransmission(0x20);  //Begin the transmission to PCF8575
  if (value == HIGH) {
    writeByteBuffered = writeByteBuffered | bit(pin);
  } else {
    writeByteBuffered = writeByteBuffered & ~bit(pin);
  }


  writeByteBuffered = writeByteBuffered & writeMode;
  Wire.write((uint8_t)writeByteBuffered);
  Wire.write((uint8_t)(writeByteBuffered >> 8));
  Wire.endTransmission();
};


