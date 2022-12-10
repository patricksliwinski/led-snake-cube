#include <Adafruit_NeoPixel.h>
#include "Cube.h"
#include "GY521.h"

#define ANGLE_THRESHOLD 70

const float MAX_SCORE = 5000.0;

Cube cube;
GY521 gyro(0x68);

uint32_t appleColor = cube.color(0, 50, 0);
uint32_t snakeColor = cube.color(50, 0, 0);

int8_t rowIncrements[4] = {0, 0, -1, 1};
int8_t columnIncrements[4] = {1, -1, 0, 0};
enum directions
{
  RIGHT,
  LEFT,
  UP,
  DOWN
};

enum directions currentDirection = UP;
int headRow = 2;
int headCol = 2;
int8_t currentSide = 0;
int8_t topSide = 0;
int length = 5;
int score = 0;

uint32_t lastSideChange = millis();

int timerGrid[6][5][5];
uint32_t colorGrid[6][5][5];

// stored which sides are adjacent to a given side
// in order {right, left, up, down}
uint8_t adjacencyMatrix[6][4] = {{3, 1, 4, 2},
                                 {2, 4, 0, 5},
                                 {3, 1, 0, 5},
                                 {4, 2, 0, 5},
                                 {1, 3, 0, 5},
                                 {3, 1, 2, 4}};

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));

  Wire.begin();
  delay(100);
  while (gyro.wakeup() == false)
  {
    delay(1000);
  }
  gyro.setAccelSensitivity(2);
  gyro.setGyroSensitivity(1);
  gyro.setThrottle();

  cube.clear();
  cube.setCube(colorGrid);
  cube.show();

  sleep();
}

void loop()
{
//  readGyro();
//  delay(100);
  changeDirection();
  moveSnake();
  updateGrid();

  cube.clear();
  cube.setCube(colorGrid);
  cube.show();
  // delayWithGyroPoll();
  delay(300);

  score += length;

  if (millis() - lastSideChange > 30000) {
     gameOver();
  }
  // Serial.println(getTopSide());
  // cube.clear();
  // cube.fill(cube.color(50,0,0), getTopSide());
  // cube.show();
  // delay(500);
}

void delayWithGyroPoll() {
  for (int i = 0; i < 10; i++) {
    gyro.read();
    delay(30);
    changeDirection();
  }
}

void sleep()
{
  int shakeCounter = 0;
  int stationaryCounter = 0;

  while (shakeCounter < 5) {
    if (cubeShaken()) {
      shakeCounter++;
    } else if (stationaryCounter++ > 50) {
      shakeCounter = 0;
      stationaryCounter = 0;
    }
    delay(100);
  }
  beginGame();
}

bool cubeShaken() {
  gyro.read();
  float angleX = gyro.getAngleX();
  float angleY = gyro.getAngleY();
  float angleZ = gyro.getAngleZ();
  float accelX = gyro.getAccelX();
  float accelY = gyro.getAccelY();
  float accelZ = gyro.getAccelZ();
  
  return sqrt(sq(accelX) + sq(accelY) + sq(accelZ)) > 1.3;
}

void powerOff()
{
  while (1)
    ;
}

void beginGame()
{
  for (int border = 0; border < 3; border++) {
    cube.clear();
    for (int row = border; row < NUMPIXELS - border; row++) {
      for (int col = border; col < NUMPIXELS - border; col++) {
        cube.setPixel(cube.color(50,0,0), row, col, 0);
      }
    }
    cube.show();
    delay(2000);
  }
  for (int apples = 0; apples < 8; apples++) {
    generateApple();
  }
  lastSideChange = millis();
}

void gameOver()
{

  for (int side = 0; side < 6; side++)
  {
    for (int row = 0; row < NUMPIXELS; row++)
    {
      for (int col = 0; col < NUMPIXELS; col++)
      {
        colorGrid[side][row][col] = 0;
        timerGrid[side][row][col] = 0;
      }
    }
  }

  cube.clear();
  cube.fill(cube.color(50, 0, 0));
  cube.show();
  delay(5000);

  int numPixelsFilled = int((score / MAX_SCORE) * 25);
  numPixelsFilled = min(25, numPixelsFilled);

  for (int i = 1; i <= numPixelsFilled; i++) {
    cube.clear();
    cube.fill(cube.color(0,0,50), 0, i);
    cube.show();
    delay(400);
  }

  delay(1000);

  if (numPixelsFilled > 0) {
    for (int i = 0; i < 5; i++) {
      cube.clear();
      cube.fill(cube.color(0,0,50), 0, numPixelsFilled);
      cube.show();
      delay(400);
      cube.clear();
      cube.show();
      delay(400);
    }
  }

  cube.clear();
  cube.show();

  currentDirection = UP;
  headRow = 2;
  headCol = 2;
  currentSide = 0;
  length = 5;
  score = 0;
  sleep();
}

void updateGrid()
{
  for (int side = 0; side < 6; side++)
  {
    for (int row = 0; row < NUMPIXELS; row++)
    {
      for (int col = 0; col < NUMPIXELS; col++)
      {
        if (timerGrid[side][row][col] == 0)
        {
          colorGrid[side][row][col] = 0;
        }
        else
        {
          timerGrid[side][row][col] -= 1;
        }
      }
    }
  }
}

void generateApple()
{
  bool foundEmptySpot = false;
  while (!foundEmptySpot)
  {
    uint8_t side = random(0, 6);
    uint8_t row = random(0, NUMPIXELS);
    uint8_t col = random(0, NUMPIXELS);
    if (timerGrid[side][row][col] == 0)
    {
      timerGrid[side][row][col] = 1000000;
      colorGrid[side][row][col] = appleColor;
      foundEmptySpot = true;
    }
  }
}

void generateApple(uint8_t side, uint8_t row, uint8_t col)
{
  timerGrid[side][row][col] = 1000000;
  colorGrid[side][row][col] = appleColor;
}

bool outOfBounds()
{
  return headRow < 0 || headCol < 0 || headRow >= NUMPIXELS || headCol >= NUMPIXELS;
}

void updateHeadPosition()
{
  headRow += rowIncrements[currentDirection];
  headCol += columnIncrements[currentDirection];
}

void moveSnake()
{

  if (timerGrid[currentSide][headRow][headCol] > 0)
  {
    if (colorGrid[currentSide][headRow][headCol] == appleColor)
    {
      for (int side = 0; side < 6; side++)
      {
        for (int row = 0; row < NUMPIXELS; row++)
        {
          for (int col = 0; col < NUMPIXELS; col++)
          {
            if (timerGrid[side][row][col] > 0) {
              timerGrid[side][row][col] += 1;
            }
          }
        }
      }
      length += 1;
      generateApple();
    }
    else
    {
      gameOver();
    }
  }
  colorGrid[currentSide][headRow][headCol] = snakeColor;
  timerGrid[currentSide][headRow][headCol] = length;

  updateHeadPosition();
  if (outOfBounds())
  {
    rotateSide();
  }
}

uint8_t getTopSide()
{
  gyro.read();
  float angleX = gyro.getAngleX();
  float angleY = gyro.getAngleY();
  float angleZ = gyro.getAngleZ();
  float accelX = gyro.getAccelX();
  float accelY = gyro.getAccelY();
  float accelZ = gyro.getAccelZ();

  float magAngleX = abs(angleX);
  float magAngleY = abs(angleY);
  float magAngleZ = abs(angleZ);

  int offset = 40;

  if (topSide == 0 || topSide == 5) {
    magAngleZ -= offset;
  }

  if (topSide == 4 || topSide == 2) {
    magAngleY -= offset;
  }

  if (topSide == 1 || topSide == 3) {
    magAngleX -= offset;
  }

  if (magAngleZ >= magAngleX && magAngleZ >= magAngleY)
  {
    return angleZ > 0 ? 5 : 0;
  }
  else if (magAngleY >= magAngleX && magAngleY >= magAngleZ)
  {
    return angleY > 0 ? 4 : 2;
  }
  else if (magAngleX >= magAngleY && magAngleX >= magAngleZ)
  {
    return angleX > 0 ? 1 : 3;
  }

  return 0;
}

void changeDirection()
{
  topSide = getTopSide();
  enum directions targetDirection;
  if (adjacencyMatrix[currentSide][RIGHT] == topSide)
  {
    targetDirection = RIGHT;
  }
  else if (adjacencyMatrix[currentSide][LEFT] == topSide)
  {
    targetDirection = LEFT;
  }
  else if (adjacencyMatrix[currentSide][UP] == topSide)
  {
    targetDirection = UP;
  }
  else if (adjacencyMatrix[currentSide][DOWN] == topSide)
  {
    targetDirection = DOWN;
  }
  else
  {
    return;
  }

  if (((targetDirection == RIGHT || targetDirection == LEFT) && (currentDirection == UP || currentDirection == DOWN)) || ((targetDirection == UP || targetDirection == DOWN) && (currentDirection == LEFT || currentDirection == RIGHT)))
  {
    currentDirection = targetDirection;
    lastSideChange = millis();
  }

  // Serial.println(targetDirection);

}

void rotateSide()
{
  uint8_t newSide = adjacencyMatrix[currentSide][currentDirection];
  int8_t previousRow = headRow;
  int8_t previousCol = headCol;
  uint8_t previousDirection = currentDirection;

  if (adjacencyMatrix[newSide][RIGHT] == currentSide)
  {
    currentDirection = LEFT;
  }
  else if (adjacencyMatrix[newSide][LEFT] == currentSide)
  {
    currentDirection = RIGHT;
  }
  else if (adjacencyMatrix[newSide][UP] == currentSide)
  {
    currentDirection = DOWN;
  }
  else
  {
    currentDirection = UP;
  }

  switch (previousDirection)
  {
  case RIGHT:
    switch (currentDirection)
    {
    case RIGHT:
      headCol = 0;
      break;
    case LEFT:
      headCol = NUMPIXELS - 1;
      headRow = NUMPIXELS - 1 - previousRow;
      break;
    case UP:
      headRow = NUMPIXELS - 1;
      headCol = previousRow;
      break;
    case DOWN:
      headRow = 0;
      headCol = NUMPIXELS - 1 - previousRow;
      break;
    }
    break;
  case LEFT:
    switch (currentDirection)
    {
    case RIGHT:
      headCol = 0;
      headRow = NUMPIXELS - 1 - previousRow;
      break;
    case LEFT:
      headCol = NUMPIXELS - 1;
      break;
    case UP:
      headRow = NUMPIXELS - 1;
      headCol = NUMPIXELS - 1 - previousRow;
      break;
    case DOWN:
      headCol = previousRow;
      headRow = 0;
      break;
    }
    break;
  case UP:
    switch (currentDirection)
    {
    case RIGHT:
      headRow = previousCol;
      headCol = 0;
      break;
    case LEFT:
      headRow = NUMPIXELS - 1 - previousCol;
      headCol = NUMPIXELS - 1;
      break;
    case UP:
      headRow = NUMPIXELS - 1;
      break;
    case DOWN:
      headRow = 0;
      headCol = NUMPIXELS - 1 - previousCol;
      break;
    }
    break;
  case DOWN:
    switch (currentDirection)
    {
    case RIGHT:
      headRow = NUMPIXELS - 1 - previousCol;
      headCol = 0;
      break;
    case LEFT:
      headRow = previousCol;
      headCol = NUMPIXELS - 1;
      break;
    case UP:
      headRow = NUMPIXELS - 1;
      headCol = NUMPIXELS - 1 - previousCol;
      break;
    case DOWN:
      headRow = 0;
      break;
    }
    break;
  }

  currentSide = newSide;
}

void readGyro() {
  gyro.read();
  float angleX = gyro.getAngleX();
  float angleY = gyro.getAngleY();
  float angleZ = gyro.getAngleZ();
  float accelX = gyro.getAccelX();
  float accelY = gyro.getAccelY();
  float accelZ = gyro.getAccelZ();

  float magAngleX = abs(angleX);
  float magAngleY = abs(angleY);
  float magAngleZ = abs(angleZ);

  magAngleZ -= 55;

  Serial.print(magAngleX);
  Serial.print(",");
  Serial.print(magAngleY);
  Serial.print(",");
  Serial.print(magAngleZ);
  Serial.println(",");

//  Serial.print(accelX);
//  Serial.print(",");
//  Serial.print(accelY);
//  Serial.print(",");
//  Serial.print(accelZ);
//  Serial.println();


}
