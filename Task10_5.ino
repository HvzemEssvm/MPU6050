#include <Wire.h>
#define IMU 0x68

float gyroRead=0,yaw=0;
unsigned long prevTime=0, currentTime=0, elapsedTime=0;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Wire.beginTransmission(IMU);
  Wire.write(0x6B);
  Wire.write(0x00); // reseting the power in the IMU
  Wire.endTransmission();
  Wire.beginTransmission(IMU);
  Wire.write(0x1B);
  Wire.write(0x10); // choosing +- 1000 deg/sec as scale range then LSB to be used must be 32.8
  Wire.endTransmission();
  delay(10); // for stabillity
}

void loop()
{
  Wire.beginTransmission(IMU);
  Wire.write(0x47); // 1st register for Gyroscope Z reading 
  Wire.endTransmission(false); // ensuring transmission still on
  Wire.requestFrom(IMU,2,true); // this true makes I2C endTransmission after requesting
  prevTime = currentTime;
  currentTime = millis();
  elapsedTime = currentTime-prevTime;
  gyroRead = (Wire.read()<<8 | Wire.read())/32.8; // this gives us the read in deg/sec
  yaw += (gyroRead*elapsedTime/1000); // this stores the accumilating read in degrees in Yaw variable
  Serial.print("Yaw Angle: ");
  Serial.println(yaw);
}

