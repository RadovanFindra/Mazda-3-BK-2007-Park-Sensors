#include "grid.h"


void Grid::createVehicleSide() {

  this->vehicleSide.createSprite(MAZDA_3_BK_BACK_WIDTH, MAZDA_3_BK_BACK_HEIGHT);
  this->vehicleSide.fillSprite(TFT_GREEN);
  this->vehicleSide.setSwapBytes(true);
  this->vehicleSide.pushImage(0, 0, MAZDA_3_BK_BACK_WIDTH, MAZDA_3_BK_BACK_HEIGHT, Mazda_3_BK_Back);
  this->vehicleSide.pushSprite(64, 0, 0x07E0);
  this->vehicleSide.deleteSprite();
}

void Grid::update(int Left, int DownLeft, int DownRight, int Right) {
  if (Right > 0 && this->Right_Last != Right) {
    this->reck.createSprite(70, 120);
    this->reck.fillSprite(TFT_GREEN);

    if (Right >= 4) {
      this->reck.drawSmoothArc(-130, 0, 176, 156, 240, 295, TFT_RED, TFT_RED, true);  //L1
    } else {
      this->reck.drawSmoothArc(-130, 0, 176, 156, 240, 295, TFT_BLACK, TFT_BLACK, true);  //L1
    }
    if (Right >= 3) {
      this->reck.drawSmoothArc(-130, 0, 201, 181, 240, 297, TFT_ORANGE, TFT_ORANGE, true);  // L2
    } else {
      this->reck.drawSmoothArc(-130, 0, 201, 181, 240, 297, TFT_BLACK, TFT_BLACK, true);  // L2
    }
    if (Right >= 2) {
      this->reck.drawSmoothArc(-130, 0, 226, 206, 240, 299, TFT_ORANGE, TFT_ORANGE, true);  // L3
    } else {
      this->reck.drawSmoothArc(-130, 0, 226, 206, 240, 299, TFT_BLACK, TFT_BLACK, true);  // L3
    }
    this->reck.pushSprite(250, 0, 0x07E0);

    this->Right_Last = Right;
  }

  if (Left > 0 && this->Left_Last != Left) {
    this->reck.createSprite(70, 120);
    this->reck.fillSprite(TFT_GREEN);
    if (Left >= 4) {
      this->reck.drawSmoothArc(200, 0, 176, 156, 65, 120, TFT_RED, TFT_RED, true);  //L1
    } else {
      this->reck.drawSmoothArc(200, 0, 176, 156, 65, 120, TFT_BLACK, TFT_BLACK, true);  //L1
    }
    if (Left >= 3) {
      this->reck.drawSmoothArc(200, 0, 201, 181, 63, 120, TFT_ORANGE, TFT_ORANGE, true);  // L2
    } else {
      this->reck.drawSmoothArc(200, 0, 201, 181, 63, 120, TFT_BLACK, TFT_BLACK, true);  // L2
    }
    if (Left >= 2) {
      this->reck.drawSmoothArc(200, 0, 226, 206, 61, 120, TFT_ORANGE, TFT_ORANGE, true);  // L3
    } else {
      this->reck.drawSmoothArc(200, 0, 226, 206, 61, 120, TFT_BLACK, TFT_BLACK, true);  // L3
    }
    this->reck.pushSprite(0, 0, 0x07E0);
    this->Left_Last = Left;
  }

  if (DownLeft > 0 && this->DownLeft_Last != DownLeft) {

    this->reck.createSprite(154, 164);
    this->reck.fillSprite(TFT_GREEN);
    if (DownLeft >= 7) {
      this->reck.drawSmoothArc(160, -120, 175, 155, 0, 35, TFT_RED, TFT_RED, true);  // L1
    } else {
      this->reck.drawSmoothArc(160, -120, 175, 155, 0, 35, TFT_BLACK, TFT_BLACK, true);  // L1
    }
    if (DownLeft >= 6) {
      this->reck.drawSmoothArc(160, -120, 200, 180, 0, 35, TFT_ORANGE, TFT_ORANGE, true);  // L2
    } else {
      this->reck.drawSmoothArc(160, -120, 200, 180, 0, 35, TFT_BLACK, TFT_BLACK, true);  // L2
    }
    if (DownLeft >= 5) {
      this->reck.drawSmoothArc(160, -120, 225, 205, 0, 35, TFT_ORANGE, TFT_ORANGE, true);  // L3
    } else {
      this->reck.drawSmoothArc(160, -120, 225, 205, 0, 35, TFT_BLACK, TFT_BLACK, true);  // L3
    }
    if (DownLeft >= 4) {
      this->reck.drawSmoothArc(160, -120, 250, 230, 0, 35, TFT_BLUE, TFT_BLUE, true);  // L4
    } else {
      this->reck.drawSmoothArc(160, -120, 250, 230, 0, 35, TFT_BLACK, TFT_BLACK, true);  // L4
    }
    if (DownLeft >= 3) {
      this->reck.drawSmoothArc(160, -120, 275, 255, 0, 35, TFT_BLUE, TFT_BLUE, true);  // L5
    } else {
      this->reck.drawSmoothArc(160, -120, 275, 255, 0, 35, TFT_BLACK, TFT_BLACK, true);  // L5
    }
    if (DownLeft >= 2) {
      this->reck.drawSmoothArc(160, -120, 300, 280, 0, 35, TFT_BLUE, TFT_BLUE, true);  // L6
    } else {
      this->reck.drawSmoothArc(160, -120, 300, 280, 0, 35, TFT_BLACK, TFT_BLACK, true);  // L6
    }
    //this->reck.fillTriangle(73, 0, 0, 73, 0, 0, TFT_BLACK);
    this->reck.pushSprite(0, 80, 0x07E0);
    //createGrid_Back();
    this->DownLeft_Last = DownLeft;
  }

  if (DownRight > 0 && this->DownRight_Last != DownRight) {
    this->reck.createSprite(154, 164);
    //this->reck.setRotation(250);
    this->reck.fillScreen(TFT_GREEN);
    if (DownRight >= 7) {
      this->reck.drawSmoothArc(-5, -120, 175, 155, 325, 0, TFT_RED, TFT_RED, true);  // L1
    } else {
      this->reck.drawSmoothArc(-5, -120, 175, 155, 325, 0, TFT_BLACK, TFT_BLACK, true);  // L1
    }
    if (DownRight >= 6) {
      this->reck.drawSmoothArc(-5, -120, 200, 180, 325, 0, TFT_ORANGE, TFT_ORANGE, true);  // L2
    } else {
      this->reck.drawSmoothArc(-5, -120, 200, 180, 325, 0, TFT_BLACK, TFT_BLACK, true);  // L2
    }
    if (DownRight >= 5) {
      this->reck.drawSmoothArc(-5, -120, 225, 205, 325, 0, TFT_ORANGE, TFT_ORANGE, true);  // L3
    } else {
      this->reck.drawSmoothArc(-5, -120, 225, 205, 325, 0, TFT_BLACK, TFT_BLACK, true);  // L3
    }
    if (DownRight >= 4) {
      this->reck.drawSmoothArc(-5, -120, 250, 230, 325, 0, TFT_BLUE, TFT_BLUE, true);
    } else {
      this->reck.drawSmoothArc(-5, -120, 250, 230, 325, 0, TFT_BLACK, TFT_BLACK, true);  // L4
    }
    if (DownRight >= 3) {
      this->reck.drawSmoothArc(-5, -120, 275, 255, 325, 0, TFT_BLUE, TFT_BLUE, true);  // L5
    } else {
      this->reck.drawSmoothArc(-5, -120, 275, 255, 325, 0, TFT_BLACK, TFT_BLACK, true);  //
    }
    if (DownRight >= 2) {
      this->reck.drawSmoothArc(-5, -120, 300, 280, 325, 0, TFT_BLUE, TFT_BLUE, true);  // L6
    } else {
      this->reck.drawSmoothArc(-5, -120, 300, 280, 325, 0, TFT_BLACK, TFT_BLACK, true);  // L6
    }
    //this->reck.fillTriangle(155, 0, 156, 77, 77, 0, TFT_BLACK);
    this->reck.pushSprite(165, 80, 0x07E0);
    //createGrid_Back();
    this->DownRight_Last = DownRight;
  }
  this->reck.deleteSprite();
}

TFT_eSPI* Grid::getlcd() {
  return &this->lcd;
}
void  Grid::set() {
  this->lcd.init();
  this->lcd.fillScreen(TFT_BLACK);
  this->lcd.setRotation(3);
}
