#include <TFT_eSPI.h>
#include "SPI.h"
#include <PNGdec.h>
#include "Mazda3.h"

TFT_eSPI tft = TFT_eSPI();

PNG png;

int trigPin = 27;  // Trigger
int echoPin = 22;  // Echo
long duration, cm, inches;
#define MAX_IMAGE_WIDTH 240 // Adjust for your images
int16_t xpos = 50;
int16_t ypos = 0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);
  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  int16_t rc = png.openFLASH((uint8_t *)Mazda_3_BK_Back, sizeof(Mazda_3_BK_Back), pngDraw);
  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  } else {
    Serial.printf("png.open() failed, rc=%d\n", rc);
  }
 
}


void loop(void) {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance
  cm = (duration / 2) / 29.1;  // Divide by 29.1 or multiply by 0.0343

  int x = 5;
  int y = 10;
  int fontNum = 2;
  tft.setTextPadding(20);
  tft.drawNumber(cm, x, y);  // Left Aligned
  Serial.println(cm);
  delay(100);
}

//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_LITTLE_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
