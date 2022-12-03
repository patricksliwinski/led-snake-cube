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
 *    [5]
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

enum directions {RIGHT, LEFT, UP, DOWN};
enum directions direction = DOWN;
int headRow = 0;
int headCol = 0;

void setup() {
  Serial.begin(115200);

//  test_fill_sides();
  cube.show();
}

void loop() {
  test_rotations();
  delay(100);
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

void test_rotations() {
  cube.clear();
//  cube.fill(cube.color(50, 0, 0), currentSide);

  switch (currentSide) {
    case 0:
      switch (direction) {
        case RIGHT:
          if (++headCol >= NUMPIXELS) {
            currentSide = 1;
            headCol = 0; 
          }
        break;
        case LEFT:
          if (--headCol < 0) {
            currentSide = 3;
            headCol = NUMPIXELS - 1;
          }
        break;
        case UP:
          if (--headRow < 0) {
            currentSide = 4;
            direction = RIGHT;
            headRow = headCol;
            headCol = 0;
          }
        break;
        case DOWN:
          if (++headRow >= NUMPIXELS) {
            currentSide = 5;
            direction = RIGHT;
            headRow = headCol;
            headCol = 0;
          }
        break;
      }
    break;
    case 1:
      switch (direction) {
        case RIGHT:
          if (++headCol >= NUMPIXELS) {
            currentSide = 2;
            headCol = 0; 
          }
        break;
        case LEFT:
          if (--headCol < 0) {
            currentSide = 0;
            headCol = NUMPIXELS - 1;
          }
        break;
        case UP:
          if (--headRow < 0) {
            currentSide = 4;
            direction = DOWN;
            headRow = 0;
          }
        break;
        case DOWN:
        if (++headRow >= NUMPIXELS) {
          currentSide = 5;
          direction = DOWN;
          headRow = 0;
        }
        break;
      }
    break;
    case 2:
      switch (direction) {
        case RIGHT:
          if (++headCol >= NUMPIXELS) {
            currentSide = 3;
            headCol = 0; 
          }
        break;
        case LEFT:
          if (--headCol < 0) {
            currentSide = 1;
            headCol = NUMPIXELS - 1;
          }
        break;
        case UP:
          if (--headRow < 0) {
            currentSide = 4;
            direction = LEFT;
            headRow = headCol;
            headCol = NUMPIXELS-1;
          }
        break;
        case DOWN:
          if (++headRow >= NUMPIXELS) {
            currentSide = 5;
            direction = LEFT;
            headRow = headCol;
            headCol = NUMPIXELS-1;
          }
        break;
      }
    break;
    case 3:
      switch (direction) {
        case RIGHT:
          if (++headCol >= NUMPIXELS) {
            currentSide = 0;
            headCol = 0; 
          }
        break;
        case LEFT:
          if (--headCol < 0) {
            currentSide = 2;
            headCol = NUMPIXELS - 1;
          }
        break;
        case UP:
          if (--headRow < 0) {
            currentSide = 4;
            direction = UP;
            headRow = NUMPIXELS-1;
          }
        break;
        case DOWN:
          if (++headRow >= NUMPIXELS) {
            currentSide = 5;
            direction = UP;
            headRow = NUMPIXELS-1;
          }
        break;
      }
    break;
    case 4:
      switch (direction) {
        case RIGHT:
          if (++headCol >= NUMPIXELS) {
            currentSide = 2;
            headCol = headRow;
            headRow = 0;
          }
        break;
        case LEFT:
          if (--headCol < 0) {
            currentSide = 0;
            headCol = headRow;
            headRow = 0;
          }
        break;
        case UP:
          if (--headRow < 0) {
            currentSide = 1;
            headRow = 0;
          }
        break;
        case DOWN:
          if (++headRow >= NUMPIXELS) {
            currentSide = 3;
            headRow = 0;
          }
        break;
      }
      direction = DOWN;
    break;
    case 5:
      switch (direction) {
        case RIGHT:
          if (++headCol >= NUMPIXELS) {
            currentSide = 2;
            headCol = headRow;
            headRow = 0;
          }
        break;
        case LEFT:
          if (--headCol < 0) {
            currentSide = 0;
            headCol = headRow;
            headRow = 0;
          }
        break;
        case UP:
          if (--headRow < 0) {
            currentSide = 1;
            headRow = 0;
          }
        break;
        case DOWN:
          if (++headRow >= NUMPIXELS) {
            currentSide = 3;
            headRow = 0;
          }
        break;
      }
      direction = UP;
    break;
  }
  cube.setPixel(cube.color(0,0,50), headRow, headCol, currentSide);
}
