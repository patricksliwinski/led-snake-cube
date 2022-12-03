#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define PIN1        6
#define PIN2        7
#define PIN3        8
#define PIN4        9
#define PIN5        10
#define PIN6        11

#define NUMPIXELS 5

class Cube {
public:
	Cube();
	~Cube();

	void fill(uint32_t color);
	void fill(uint32_t color, uint8_t side);
  void setPixel(uint8_t color, uint8_t row, uint8_t col, uint8_t side);
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

  int gameGrid[6][NUMPIXELS][NUMPIXELS];


};

#endif
