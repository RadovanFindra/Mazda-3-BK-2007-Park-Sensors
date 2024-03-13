# 1 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino"
# 2 "E:\\OneDrive - Žilinská univerzita v Žiline\\VVS\\Mazda-3-BK-2007-Park-Sensors\\Mazda-3-BK-2007-Park-Sensors.ino" 2
TFT_eSPI tft = TFT_eSPI();


int trigPin = 27; // Trigger
int echoPin = 22; // Echo
long duration, cm, inches;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, 0x03);

  pinMode(echoPin, 0x01);
  tft.init();
  tft.setRotation(1);
  tft.setTextColor(0xFFFF /* 255, 255, 255 */, 0x0000 /*   0,   0,   0 */);
  tft.fillScreen(0x0000 /*   0,   0,   0 */);
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
