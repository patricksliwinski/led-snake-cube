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

void Cube::setPixel(uint8_t color, uint8_t row, uint8_t col, uint8_t side) {
  pixelSides[side].setPixelColor(row * NUMPIXELS + col, color);
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
