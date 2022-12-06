#include <Adafruit_NeoPixel.h>
#include "Cube.h"
#include "GY521.h"

GY521 gyro(0x68);

int diceSides[6][NUMPIXELS][NUMPIXELS] = {{{0,0,0,0,0},
                                           {0,0,0,0,0},
                                           {0,0,1,0,0},
                                           {0,0,0,0,0},
                                           {0,0,0,0,0}},
                                       
                                          {{0,0,0,0,0},
                                           {0,0,0,1,0},
                                           {0,0,0,0,0},
                                           {0,1,0,0,0},
                                           {0,0,0,0,0}},

                                          {{0,0,0,0,0},
                                           {0,0,0,1,0},
                                           {0,0,1,0,0},
                                           {0,1,0,0,0},
                                           {0,0,0,0,0}},

                                          {{0,0,0,0,0},
                                           {0,1,0,1,0},
                                           {0,0,0,0,0},
                                           {0,1,0,1,0},
                                           {0,0,0,0,0}},
                                     
                                          {{0,0,0,0,0},
                                           {0,1,0,1,0},
                                           {0,0,1,0,0},
                                           {0,1,0,1,0},
                                           {0,0,0,0,0}},
                                     
                                          {{0,1,0,1,0},
                                           {0,0,0,0,0},
                                           {0,1,0,1,0},
                                           {0,0,0,0,0},
                                           {0,1,0,1,0}}};                                 
Cube cube;

void setup() {
  Serial.begin(115200);

  Wire.begin();
  delay(100);
  while (gyro.wakeup() == false) {delay(1000);}
  gyro.setAccelSensitivity(2);  // 8g
  gyro.setGyroSensitivity(1);   // 500 degrees/s
  gyro.setThrottle();
}

void loop() {
  if (cube_shaken()) {
    roll();
  }
  cube.clear();
  cube.show();
}

void roll() {

  int permutation[] = {0,1,2,3,4,5};
  for (int flip = 0; flip < 40; flip++) {
    
    for (size_t i = 0; i < 6; i++)
    {
        size_t j = random(0, 5 - i);
        int t = permutation[i];
        permutation[i] = permutation[j];
        permutation[j] = t;
    }

    for (uint8_t i = 0; i < 6; i++)
    {
       cube.setGridSide(diceSides[permutation[i]], i);
    }

    cube.update();
    cube.show();

    delay(50 + flip * 7);
    
  }
  delay(4000);
}

bool cube_shaken() {
  gyro.read();
  float angleX = gyro.getAngleX();
  float angleY = gyro.getAngleY();
  float angleZ = gyro.getAngleZ();
  float accelX = gyro.getAccelX();
  float accelY = gyro.getAccelY();
  float accelZ = gyro.getAccelZ();
//  Serial.print("AngleX: ");
//  Serial.println(angleX);
//  Serial.print("AngleY: ");
//  Serial.println(angleY);
//  Serial.print("AngleZ: ");
//  Serial.println(angleZ);
//  Serial.print("AccelX: ");
//  Serial.println(accelX);
//  Serial.print("AccelY: ");
//  Serial.println(accelY);
//  Serial.print("AccelZ: ");
//  Serial.println(accelZ);

  Serial.println(sqrt(sq(accelX) + sq(accelY) + sq(accelZ)));

  delay(10);
  
  return sqrt(sq(accelX) + sq(accelY) + sq(accelZ)) > 1.6;
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
