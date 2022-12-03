#include <Adafruit_NeoPixel.h>
#include "Cube.h"
#include "GY521.h"





/*
 *       [4]
 * [0][1][2][3]
 *       [5]
 * 
 *    [4]
 * [3][0][1] 
 *    [5]
 * 
 *    [4]
 * [0][1][2]
 *    [5]
 * 
 *    [4]
 * [1][2][3]
 *    [4]
 * 
 *    [4]
 * [2][3][0]
 *    [5]
 *    
 *    [1]
 * [0][4][2]   
 *    [3]
 * 
 *    [1]
 * [0][5][2]
 *    [3]
 */   

Cube cube;
uint8_t currentSide = 0;

void setup() {
  Serial.begin(115200);

  test_fill_sides();
  cube.show();
}

void loop() {
  rotate_sides();
  delay(500);
  cube.show();
}

void test_fill() {
  cube.fill(cube.color(50, 0, 0));
}

void test_fill_sides() {
  cube.fill(cube.color(50, 0, 0), 0);
  cube.fill(cube.color(50, 0, 0), 2);
  
  cube.fill(cube.color(0, 50, 0), 1);
  cube.fill(cube.color(0, 50, 0), 3);
  
  cube.fill(cube.color(0, 0, 50), 4);
  cube.fill(cube.color(0, 0, 50), 5);
}

void rotate_sides() {
  cube.clear();
  cube.fill(cube.color(50, 0, 0), currentSide);
  currentSide = (currentSide + 1) % 6;
}
