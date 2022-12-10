#include "Cube.h"

Cube::Cube() {
  for (int i = 0; i < 6; i++) {
    pixelSides[i].begin();
  }
}

Cube::~Cube() {};

void Cube::fill(uint32_t color) {
  for (int side = 0; side < 6; side++) {
    fill(color, side);
  }
}

void Cube::fill(uint32_t color, uint8_t side) {
  pixelSides[side].fill(color, 0, NUMPIXELS * NUMPIXELS);
}

void Cube::fill(uint32_t color, uint8_t side, uint8_t num) {
  pixelSides[side].fill(color, 0, num);
}

void Cube::setPixel(uint32_t color, uint8_t row, uint8_t col, uint8_t side) {
  if (row % 2 == 0) {
    pixelSides[side].setPixelColor(row*NUMPIXELS+col, color);
  } else {
    pixelSides[side].setPixelColor((row+1)*NUMPIXELS-col-1, color);
  }
}

void Cube::setCube(uint32_t grid[6][NUMPIXELS][NUMPIXELS]) {
  for (int side = 0; side < 6; side++) {
    for (int row = 0; row < NUMPIXELS; row++) {
      for (int col = 0; col < NUMPIXELS; col++) {
        setPixel(grid[side][row][col], row, col, side);
      }
    }
  }
}

void Cube::clear() {
  for (int side = 0; side < 6; side++) {
    clear(side);
  }
}

void Cube::clear(uint8_t side) {
  pixelSides[side].clear();
}

void Cube::show() {
  for (int i = 0; i < 6; i++) {
    pixelSides[i].show();
  }
}

uint32_t Cube::color(uint8_t r, uint8_t g, uint8_t b) {
  return pixelSides[0].Color(r, g, b);
}
