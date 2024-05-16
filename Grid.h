#pragma once
#include <TFT_eSPI.h>
#include "Mazda3.h"

class Grid {
private:
  TFT_eSprite vehicleSide = TFT_eSprite(&lcd);
  TFT_eSprite reck = TFT_eSprite(&lcd);
  TFT_eSPI lcd = TFT_eSPI();
  int DownLeft_Last;
  int DownRight_Last;
  int Left_Last;
  int Right_Last;
public:

public:
  void set();
  void createVehicleSide();
  void update(int Left, int DownLeft, int DownRight, int Right);
  TFT_eSPI* getlcd();
};