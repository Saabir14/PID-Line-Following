// By Saabir Ahmed Parvaiz Ahmed (2024-2-11)
// This code is used to control the motors and read the encoder values of the car

#include <Wire.h>

// Define the MotorController class
class MotorController {
public:
  // This is the address of the slave device
  int I2C_SLAVE_ADDR;
  int offset;
  int maxL;
  int maxR;

  MotorController(int slaveAddr, int offset = 0, int maxL = -75, int maxR = 75)
  {
    I2C_SLAVE_ADDR = slaveAddr;
    this->offset = offset;
    this->maxL = maxL;
    this->maxR = maxR;
  }

  void getEncoderValues(int *encoderL, int *encoderR)
  {
    // two 16-bit integer values are requested from the slave
    int16_t a = 0;
    int16_t b = 0;
    uint8_t bytesReceived = Wire.requestFrom(I2C_SLAVE_ADDR, 4);  // 4 indicates the number of bytes that are expected
    uint8_t a16_9 = Wire.read();  // receive bits 16 to 9 of a (one byte)
    uint8_t a8_1 = Wire.read();   // receive bits 8 to 1 of a (one byte)
    uint8_t b16_9 = Wire.read();   // receive bits 16 to 9 of b (one byte)
    uint8_t b8_1 = Wire.read();   // receive bits 8 to 1 of b (one byte)

    *encoderL = (a16_9 << 8) | a8_1; // combine the two bytes into a 16 bit number
    *encoderR = (b16_9 << 8) | b8_1; // combine the two bytes into a 16 bit number
  }

  void setMotorSteer(int steering, int motorL = 255, int motorR = 255)
  {
    steering += offset;

    Wire.beginTransmission(I2C_SLAVE_ADDR); // transmit to device #4
    /* depending on the microcontroller, the int variable is stored as 32-bits or 16-bits
    if you want to increase the value range, first use a suitable variable type and then modify the code below
    for example; if the variable used to store motorL and motorR is 32-bits and you want to use signed values between -2^31 and (2^31)-1
    uncomment the four lines below relating to bits 32-25 and 24-17 for motorL and motorR
    for my microcontroller, int is 32-bits hence motorL and motorR are AND operated with a 32 bit hexadecimal number - change this if needed

    >> motorL refers to a shift right operator by motorL bits
    */

    //Wire.write((byte)((motorL & 0xFF000000) >> 24)); // bits 32 to 25 of motorL
    //Wire.write((byte)((motorL & 0x00FF0000) >> 16)); // bits 24 to 17 of motorL
    Wire.write((byte)((motorL & 0x0000FF00) >> 8));    // first byte of motorL, containing bits 16 to 9
    Wire.write((byte)(motorL & 0x000000FF));           // second byte of motorL, containing the 8 LSB - bits 8 to 1
    //Wire.write((byte)((motorR & 0xFF000000) >> 24)); // bits 32 to 25 of motorR
    //Wire.write((byte)((motorR & 0x00FF0000) >> 16)); // bits 24 to 17 of motorR
    Wire.write((byte)((motorR & 0x0000FF00) >> 8));    // first byte of motorR, containing bits 16 to 9
    Wire.write((byte)(motorR & 0x000000FF));           // second byte of motorR, containing the 8 LSB - bits 8 to 1
    Wire.write((byte)((steering & 0x0000FF00) >> 8));    // first byte of motorR, containing bits 16 to 9
    Wire.write((byte)(steering & 0x000000FF));           // second byte of motorR, containing the 8 LSB - bits 8 to 1
    Wire.endTransmission();   // stop transmitting  }
  }
};