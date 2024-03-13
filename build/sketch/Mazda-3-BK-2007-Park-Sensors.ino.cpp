#include <Arduino.h>
#line 1 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();


int trigPin = 27;  // Trigger
int echoPin = 22;  // Echo
long duration, cm, inches;

#line 9 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
void setup();
#line 21 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
void loop(void);
#line 9 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);

  pinMode(echoPin, INPUT);
  tft.init();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
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
  tft.setTextPadding(240);
  tft.drawNumber(cm, x, y);  // Left Aligned
  Serial.println(cm);
  delay(100);
}
