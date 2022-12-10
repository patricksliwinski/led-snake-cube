#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define PIN1        0
#define PIN2        4
#define PIN3        2
#define PIN4        1
#define PIN5        3
#define PIN6        5

#define NUMPIXELS 5

class Cube {
public:
	Cube();
	~Cube();

	void fill(uint32_t color);
	void fill(uint32_t color, uint8_t side);
  void fill(uint32_t color, uint8_t side, uint8_t num);
  void setPixel(uint32_t color, uint8_t row, uint8_t col, uint8_t side);
  void setCube(uint32_t grid[6][NUMPIXELS][NUMPIXELS]);
  void clear();
  void clear(uint8_t side);
	void show();
  uint32_t color(uint8_t r, uint8_t g, uint8_t b);
 
private:
  Adafruit_NeoPixel pixelSides[6] = {
      Adafruit_NeoPixel(NUMPIXELS*NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(NUMPIXELS*NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(NUMPIXELS*NUMPIXELS, PIN3, NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(NUMPIXELS*NUMPIXELS, PIN4, NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(NUMPIXELS*NUMPIXELS, PIN5, NEO_GRB + NEO_KHZ800),
      Adafruit_NeoPixel(NUMPIXELS*NUMPIXELS, PIN6, NEO_GRB + NEO_KHZ800),
      };
};

#endif
