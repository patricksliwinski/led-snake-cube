#ifndef CUBE_H
#define CUBE_H

#include <stdlib.h>
#include <stdint.h>
#include <Adafruit_NeoPixel.h>

#define PIN        6
#define NUMPIXELS 25

Adafruit_NeoPixel cubepixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

class Cube {
public:
	Cube(uint16_t n);
	~Cube();

	void fill(uint32_t color);
	void fill(uint32_t color, uint8_t grid);
	void show() {
    cubepixels.show();
	}

};

#endif
