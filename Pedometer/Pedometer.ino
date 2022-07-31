#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>

/* Assign a unique ID to the sensors */
Adafruit_10DOF                dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);

/* Update this with the correct SLP for accurate altitude measurements */
float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
float xaccl[15] = {0};
float yaccl[15] = {0};
float zaccl[15] = {0};
float xavg;
float yavg;
float zavg;
int steps, flag = 0;
float threshhold = 80.0;
/**************************************************************************/
/*!
    @brief  Initialises all the sensors used by this example
*/
/**************************************************************************/
void initSensors()
{
  if (!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while (1);
  }
  if (!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (1);
  }
  if (!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    while (1);
  }
}

/**************************************************************************/
/*!

*/
/**************************************************************************/
void setup(void)
{
  Wire.begin(D3, D5);
  Serial.begin(115200);
  Serial.println(F("Adafruit 10 DOF Pitch/Roll/Heading Example")); Serial.println("");

  /* Initialise the sensors */
  initSensors();
  calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(getPedo());
}

int getPedo()
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  int acc = 0;
  int totvect[15] = {0};
  int totave[15] = {0};
  xaccl[15] = {0};
  yaccl[15] = {0};
  zaccl[15] = {0};

  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    for (int i = 0; i < 15; i++)
    {
      xaccl[i] = float(orientation.pitch);
      delay(1);
      yaccl[i] = float(orientation.heading);
      delay(1);
      zaccl[i] = float(orientation.roll);
      delay(1);
      totvect[i] = sqrt(((xaccl[i] - xavg) * (xaccl[i] - xavg)) + ((yaccl[i] - yavg) * (yaccl[i] - yavg)) + ((zaccl[i] - zavg) * (zaccl[i] - zavg)));
      totave[i] = (totvect[i] + totvect[i - 1]) / 2 ;
      delay(150);

      //cal steps
      if (totave[i] > threshhold && flag == 0)
      {
        steps = steps + 1;
        flag = 1;
      }
      else if (totave[i] > threshhold && flag == 1)
      {
        //do nothing
      }
      if (totave[i] < threshhold  && flag == 1)
      {
        flag = 0;
      }
      // Serial.print("steps=");
      // Serial.println(steps);
      return (steps);
    }
  }
  delay(100);
}

void calibrate()
{
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  digitalWrite(13, HIGH);
  Serial.println("Calibrating......");
  float sum = 0;
  float sum1 = 0;
  float sum2 = 0;
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    for (int i = 0; i < 15; i++)
    {
      xaccl[i] = float(orientation.pitch);
      sum = xaccl[i] + sum;
    }
    delay(15);
    xavg = sum / 15.0;
    Serial.println(xavg);
    for (int j = 0; j < 15; j++)
    {
      yaccl[j] = float(orientation.heading);
      sum1 = yaccl[j] + sum1;
    }
    yavg = sum1 / 15.0;
    Serial.println(yavg);
    delay(15);
    for (int i = 0; i < 15; i++)
    {
      zaccl[i] = float(orientation.roll);
      sum2 = zaccl[i] + sum2;
    }
    zavg = sum2 / 15.0;
    delay(15);
    Serial.println(zavg);
    digitalWrite(13, LOW);
  }
  Serial.println("Calibration Successful!");
  //delay(1500);
}
