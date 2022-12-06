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

void Cube::setPixel(uint32_t color, uint8_t row, uint8_t col, uint8_t side) {
  pixelSides[side].setPixelColor(row * NUMPIXELS + col, color);
}

void Cube::setGridSide(int copy[NUMPIXELS][NUMPIXELS], uint8_t side) {
  for (int row = 0; row < NUMPIXELS; row++) {
    for (int col = 0; col < NUMPIXELS; col++) {
      gameGrid[side][row][col] = copy[row][col];
    }
  }
}

void Cube::update() {
  for (int side = 0; side < 6; side++) {
    for (int row = 0; row < NUMPIXELS; row++) {
      for (int col = 0; col < NUMPIXELS; col++) {
        if (gameGrid[side][row][col] > 0) {
          setPixel(pixelSides[0].Color(40, 0, 0), row, col, side);
        } else {
          setPixel(color(0,0,0), row, col, side);
        }
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
