# 1 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
# 2 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino" 2

# 4 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino" 2
# 5 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino" 2

TFT_eSPI tft = TFT_eSPI();

PNG png;

int trigPin = 27; // Trigger
int echoPin = 22; // Echo
long duration, cm, inches;

int16_t xpos = 0;
int16_t ypos = 0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, 0x03);

  pinMode(echoPin, 0x01);
  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(0xFFFF /* 255, 255, 255 */, 0x0000 /*   0,   0,   0 */);
  tft.fillScreen(0x0000 /*   0,   0,   0 */);
  int16_t rc = png.openFLASH((uint8_t *)Mazda_3_BK_Back, sizeof(Mazda_3_BK_Back), pngDraw);
  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(
# 32 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino" 3 4
                   __null
# 32 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
                       , 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }
  delay(3000);
  tft.fillScreen(random(0x10000));

}


void loop(void) {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, 0x0);
  delayMicroseconds(5);
  digitalWrite(trigPin, 0x1);
  delayMicroseconds(10);
  digitalWrite(trigPin, 0x0);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, 0x01);
  duration = pulseIn(echoPin, 0x1);

  // Convert the time into a distance
  cm = (duration / 2) / 29.1; // Divide by 29.1 or multiply by 0.0343

  int x = 5;
  int y = 10;
  int fontNum = 2;
  tft.setTextPadding(240);
  tft.drawNumber(cm, x, y); // Left Aligned
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
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
