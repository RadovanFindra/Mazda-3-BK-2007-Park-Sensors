#pragma once
#include "Wire.h"

class Sensor
{
private:
    typedef struct sensor {
        uint8_t trig;
        volatile int duration;
        volatile bool waiting;
        int num;
        TFT_eSprite text = TFT_eSprite(grid.getlcd());
        sensor *next;
    } sensor;
    sensor sensors[5];

private:
    
    Sensor();
    void Write(uint8_t pin, uint8_t value);
    void Mode(uint8_t pin, uint8_t mode);
    void endPulse();
    void startPulse();
    void send_ping();
    void doSonar();
    
public:
    sensor *current;
};

Sensor::Sensor()
{
    for (int i = 0; i < sizeof(this->sensors) / sizeof(this->sensors[0]); ++i) {
    this->sensors[i].num = i;
    this->sensors[i].duration = 20;
    this->sensors[i].trig = i * 2;
    this->sensors[i].waiting = false;
    //sensors[i].trig = 2;

    this->sensors[i].text.createSprite(20, 10);
    this->sensors[i].text.fillScreen(TFT_BLACK);
    this->sensors[i].text.setTextColor(TFT_WHITE);
    this->sensors[i].text.setTextDatum(MC_DATUM);

    Mode(sensors[i].trig, OUTPUT);
    Mode(sensors[i].trig + 1, OUTPUT);
    Write(sensors[i].trig + 1, HIGH);
    this->sensors[i].waiting = false;
    if (i < sizeof(this->sensors) / sizeof(this->sensors[0]) - 1) {
      this->sensors[i].next = &sensors[i + 1];  // Nastavíme ukazatel next na následující prvek v poli senzorů
    } else {
      this->sensors[i].next = &sensors[0];  // Pokud jsme na posledním prvku, next nastavíme na první prvek
    }
  }
  this->current = &sensors[0];
  pinMode(_interruptPin, INPUT_PULLUP);
}


