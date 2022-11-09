#include <Adafruit_NeoPixel.h>
#include "Cube.h"
#include "GY521.h"

#define GAMESPEED 600

#define PIN        6
#define NUMPIXELS 25
#define DELAYVAL 500

GY521 gyro(0x68);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

enum input_states {FLAT, LEFT, RIGHT, HOLDING};
enum input_states input_state = FLAT;

enum headings {DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT};
int heading; 

int grid[5][5];

int headRow = 2;
int headCol = 2;
int appleRow;
int appleCol;
int snakeLength = 1;


void setup() {
  Serial.begin(115200);
  pixels.begin();
  Wire.begin();
  delay(100);
  while (gyro.wakeup() == false) {delay(1000);}
  gyro.setAccelSensitivity(2);  // 8g
  gyro.setGyroSensitivity(1);   // 500 degrees/s
  gyro.setThrottle();
  randomSeed(analogRead(0));

//  grid[headRow][headCol] = 1;
  generateApple();
}

void loop() {
  updateInputState();

  if (input_state == LEFT) {
    heading--;
  } else if (input_state == RIGHT) {
    heading++;
  }

  if (heading == 4) {
    heading = 0;
  } else if (heading == -1) {
    heading = 3;
  }

  if (heading == DIR_UP) {
    headRow--;
  } else if (heading == DIR_RIGHT) {
    headCol++;
  } else if (heading == DIR_DOWN) {
    headRow++;
  } else if (heading == DIR_LEFT) {
    headCol--;
  }

  if (headRow == 5) {
    headRow = 0;
  } else if (headRow == -1) {
    headRow = 4;
  }

  if (headCol == 5) {
    headCol = 0;
  } else if (headCol == -1) {
    headCol = 4;
  }

  if (grid[headRow][headCol] > 0) {
    gameOver();
  }
  
  if (headRow == appleRow && headCol == appleCol) {
    snakeLength++; 
  }
  
  grid[headRow][headCol] = 1;
  updateGrid();

  if (headRow == appleRow && headCol == appleCol) {
    generateApple();
  }

//  printGrid();
  delay(GAMESPEED);
}

void updateGrid() {
  pixels.clear();
  for (int row = 0; row < 5; row++) {
    for (int col = 0; col < 5; col++) {
      if (grid[row][col] > 0 && grid[row][col] < snakeLength + 1 || (row==appleRow && col==appleCol)) {
        int color = pixels.Color(0, 15, 0);
        if (headRow == appleRow && headCol == appleCol) {
          color = pixels.Color(0, 0, 60);
        } else if (row==appleRow && col==appleCol) {
          color = pixels.Color(20, 0, 0);
        } else if (row==headRow && col==headCol) {
          color = pixels.Color(0, 60, 0);
        }
        if (row % 2 == 0) {
          pixels.setPixelColor(row*5+col, color);
        } else {
          pixels.setPixelColor((row+1)*5-col-1, color);
        }
      }

      if (grid[row][col] > 0 && grid[row][col] < snakeLength + 1) {
        grid[row][col]++;
      } else {
        grid[row][col]= 0;
      }
    }
  }
  pixels.show();
}

void generateApple() {
  while (true) {
    appleRow = random(0, 5);
    appleCol = random(0, 5);
    if (grid[appleRow][appleCol] == 0) {
      return;
    }
  }
}

void updateInputState() {
  gyro.read();
  float y = gyro.getAngleY();
//  if (abs(y) < 10) {
//    input_state = FLAT;
//  } else if (input_state == FLAT && y > 15) {
//    input_state = LEFT;
//  } else if (input_state == FLAT && y < -15) {
//    input_state = RIGHT;
//  } else if (input_state == LEFT || input_state == RIGHT) {
//    input_state = HOLDING;
//  }

    if (y > 15) {
      input_state = LEFT;
    } else if (y < -15) {
      input_state = RIGHT;
    } else {
      input_state = FLAT;
    }
}

void gameOver() {
  pixels.clear();
  pixels.fill(pixels.Color(30, 0, 0), 0, 25);
  pixels.show();
  for(;;);
}

void printGrid() {
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      Serial.print(grid[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
