#include <MadgwickAHRS.h>

#include <BMI160.h>
#include <CurieImu.h>

#include <Servo.h>

#include <Adafruit_MotorShield.h>


/*****************************************************************************/
// Global variables and defines
#define SERVO_PIN          9
#define ANGLE_CENTRE        85
#define ANGLE_MIN           10
#define ANGLE_MAX           165

// State variables used in the loop
int speed;        // Motor speed
int angle;        // Angle of the sensor



// Create an instance of the motor shield and two motors
Adafruit_MotorShield AFMS1 = Adafruit_MotorShield(0x61);
Adafruit_DCMotor *lMotor = AFMS1.getMotor(3);
Adafruit_DCMotor *rMotor = AFMS1.getMotor(4);

Servo myservo;

Madgwick filter; // initialise Madgwick object
int ax, ay, az;
int gx, gy, gz;
float yaw;
float pitch;
float roll;
int factor = 800; // variable by which to divide gyroscope values, used to control sensitivity
// note that an increased baud rate requires an increase in value of factor

//int calibrateOffsets = 1; // int to determine whether calibration takes place or not



void setup() 
{
  // Wait for initialization
  while(!Serial);
  
  Serial.begin(9600);

    // initialize device
  CurieImu.initialize();

  // verify connection
  if (!CurieImu.testConnection()) 
  {
    Serial.println("CurieImu connection failed");
  }

  Serial.print("Starting Gyroscope calibration...");
  CurieImu.autoCalibrateGyroOffset();
  Serial.println(" Done");
  Serial.print("Starting Acceleration calibration...");
  CurieImu.autoCalibrateXAccelOffset(0);
  CurieImu.autoCalibrateYAccelOffset(0);
  CurieImu.autoCalibrateZAccelOffset(1);
  Serial.println(" Done");
  Serial.println("Enabling Gyroscope/Acceleration offset compensation");
  CurieImu.setGyroOffsetEnabled(true);
  CurieImu.setAccelOffsetEnabled(true);  
  
  myservo.attach(SERVO_PIN);
  angle = ANGLE_CENTRE;
  myservo.write(angle);       // centre the sensor

  AFMS1.begin();              // Start the motor control

}

void loop() 
{
  // read raw accel/gyro measurements from device
  ax = CurieImu.getAccelerationX();
  ay = CurieImu.getAccelerationY();
  az = CurieImu.getAccelerationZ();
  gx = CurieImu.getRotationX();
  gy = CurieImu.getRotationY();
  gz = CurieImu.getRotationZ();

  // use function from MagdwickAHRS.h to return quaternions
  filter.updateIMU(gx / factor, gy / factor, gz / factor, ax, ay, az);  

  // functions to find yaw roll and pitch from quaternions
  yaw = filter.getYaw();
  roll = filter.getRoll();
  pitch = filter.getPitch();

  Serial.print(yaw);
  Serial.print(","); // print comma so values can be parsed
  Serial.print(pitch);
  Serial.print(","); // print comma so values can be parsed
  Serial.println(roll);

  //nMake the sensor always point in one direction
  angle = ANGLE_CENTRE + (yaw / 3.1415 * 180.0);
  angle = constrain(angle, ANGLE_MIN, ANGLE_MAX);
  myservo.write(angle);
}
