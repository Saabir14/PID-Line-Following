#include <PID_v1.h>
#include <Wire.h>
#include "sensor_array.h"
#include "motor.h"


// This is the address of the slave device
#define I2C_SLAVE_ADDR 0x04 // 4 in hexadecimal

// Create a MotorController object
MotorController motorController(I2C_SLAVE_ADDR, 90, -70, 70);
// 90 is taken as the offset/center point, 60 is the steering difference

// Set the photo diode array pins (left to right)
int photoDiodePins[] = {33, 32, 35, 34, 39, 36};

// Create a SensorArray object
SensorArray sensorArray(photoDiodePins, sizeof(photoDiodePins) / sizeof(int));

// Set the black and white mapping values
int blackMap[] = {137, 194, 227, 218, 172, 149};
int whiteMap[] = {1228, 2327, 2524, 1914, 1911, 1573};

// Set the sensor displacement values
int sensorDisplacement[] = {18, 10, 3, -3, -10, -18}; // -18 is to the right of the car which is left from human perspective (depends on individual car)

const int weightedAverageRight = 1030;  // This is the value when the lane is on the right side of the car
const int weightedAverageLeft = -1030;  // This is the value when the lane is on the left side of the car

double Setpoint = 0, Input, Output;

// Specify the PID tuning parameters
// (motorController.maxL - motorController.maxR) / (weightedAverageLeft - weightedAverageRight) = 0.05825242718446602
const double Kp = static_cast<double>(motorController.maxL - motorController.maxR) / (weightedAverageLeft - weightedAverageRight);
const double Ki = 1.15;
const double Kd = 0.02;

PID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE);
void setup() {
  Serial.begin(9600);
  Wire.begin();   // join i2c bus (address optional for the master) - on the ESP32 the default I2C pins are 21 (SDA) and 22 (SCL)

  sensorArray.setMap(blackMap, whiteMap); // default min and max values are 0 and 4095
  // sensorArray.setMap(blackMap, whiteMap, 0, 100); // set custom min and max values
  sensorArray.setDisplacement(sensorDisplacement);

  // Set the steering to be straight
  motorController.setMotorSteer(0, 0, 0); // set the motor to 0 and steer values to 0

  //turn the PID on
  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(motorController.maxL, motorController.maxR);
}
void loop() {
  sensorArray.readValues();
  sensorArray.mapValues();

  Input = sensorArray.weightedAverage();
  pid.Compute();
  
  Serial.printf("%f,", millis() / 1000.0);
  Serial.printf("%f,", Input);
  Serial.printf("%f\n", Output);

  // motorController.setMotorSteer(map(w, weightedAverageRight, weightedAverageLeft, motorController.maxR, motorController.maxL), 0, 0);
  motorController.setMotorSteer(Output);
}