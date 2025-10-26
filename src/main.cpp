#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVOMIN  55 // Subject to change, based on testing
#define SERVOMAX  515 // Subject to change, based on testing
#define USMIN  600  // Subject to change, based on testing
#define USMAX  2400 // Subject to change, based on testing
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
// 100 - 15
// A guess - will need to be recalibrated in testing
uint8_t SERVO_A  = 6;
uint8_t SERVO_B1 = 5;
uint8_t SERVO_B2 = 4;
uint8_t SERVO_C = 3;

uint8_t SERVO_D  = 1;
uint8_t SERVO_E  = 0;
uint8_t SERVO_F  = 2;
uint8_t Elbow_Vertical   = 2;

#define MOVEOUT 119 // W
#define MOVEIN 115 // S
#define MOVECLOCKWISE 97 // A
#define MOVECOUNTERCLOCKWISE 100 // D
Adafruit_PWMServoDriver servo = Adafruit_PWMServoDriver();

float Forearm = 2.5; // Remember to update this
float Aftarm  = 2.5; // Remember to update this

void setup() {
  Serial.begin(9600); // Output to connected computer
  Serial.println("Begin controlling now"); // Idk we may need a more appropriate startup message but that *is* the end goal so

  servo.begin(); // Initialise connection to the adafruit servo controller thingy
  servo.setOscillatorFrequency(27*pow(10,6)); // Set the oscillator to 27MHz
  servo.setPWMFreq(SERVO_FREQ);  // Tells the PWM board the speed to output to the servos

  delay(10); // Wait 10ms
};

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int moveto(int radius, int deg) {
  radius = constrain(radius, 0, Forearm + Aftarm); // Stops you from going to a location farther away than the robot can reach
  deg = deg%360; // Converts all angles to a number between 1 & 360

  servo.setPWM(SERVO_A, 0, map(deg, 0, 360, SERVOMIN, SERVOMAX)); // Spins the base of the robot to the desired angle

  float Base_Vert_Angle = acos((pow(radius, 2)+pow(Aftarm, 2) - pow(Forearm,2))/2*Forearm*radius); // Gets the angle of the shoulder joint using the law of cosines.
  servo.setPWM(SERVO_B1, 0, mapf(Base_Vert_Angle, 0, 360, SERVOMIN, SERVOMAX)); // Raises the shoulder to the desired angle


  float Elbow_Vert_Angle = acos((pow(Forearm, 2) + pow(Aftarm, 2) - pow(radius, 2)) / 2 * Forearm * Aftarm); // Gets the angle of the shoulder joint using the law of cosines.
  servo.setPWM(Elbow_Vertical, 0, mapf(Elbow_Vert_Angle, 0, 360, SERVOMIN, SERVOMAX)); // Raises the elbow to the desired angle
  return 1;
}


int pos_y = 0;
int pos_deg = 0;
int serial_val;
int val;
void loop() {
  // int success = moveto(sin(wave), wave%360);
  // wave = (wave+1)%360;
  if (Serial.available()) {
    serial_val = Serial.read();
    Serial.println("Position:");
    Serial.println(pos_deg);
    Serial.println(pos_y);
    switch(serial_val) {
      case MOVEOUT:
        Serial.println("MOVING OUT");
        pos_y += 1;
        break;
      case MOVEIN:
        Serial.println("MOVING IN");
        pos_y -= 1;
        break;
      case MOVECLOCKWISE:
        Serial.println("MOVING CLOCKWISE");
        pos_deg -= 5;
        break;
      case MOVECOUNTERCLOCKWISE:
        Serial.println("MOVING COUNTERCLOCKWISE");
        pos_deg += 5;
        break;
      default:
        break;
    }
    servo.setPWM(SERVO_A,0,pos_deg);
    servo.setPWM(SERVO_B1,0,pos_y);
    // moveto(pos_y,pos_deg);
  }
};
