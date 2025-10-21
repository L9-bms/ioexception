#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// change this depending on the address of the pwm shield
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

void setup()
{
  Serial.begin(9600);
  Serial.println("Welcome to pick and place v0.0.1");

  pwm.begin();

  /*
   * In theory the internal oscillator (clock) is 25MHz but it really isn't
   * that precise. You can 'calibrate' this by tweaking this number until
   * you get the PWM update frequency you're expecting!
   * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
   * is used for calculating things like writeMicroseconds()
   * Analog servos run at ~50 Hz updates, It is importaint to use an
   * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
   * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
   *    the I2C PCA9685 chip you are setting the value for.
   * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
   *    expected value (50Hz for most ESCs)
   * Setting the value here is specific to each individual I2C PCA9685 chip and
   * affects the calculations for the PWM update frequency.
   * Failure to correctly set the int.osc value will cause unexpected PWM results
   */
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);

  delay(10);
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse)
{
  double pulselength;

  pulselength = 1000000;     // 1,000,000 us per second
  pulselength /= SERVO_FREQ; // Analog servos run at ~60 Hz updates
  Serial.print(pulselength);
  Serial.println(" us per period");
  pulselength /= 4096; // 12 bits of resolution
  Serial.print(pulselength);
  Serial.println(" us per bit");
  pulse *= 1000000; // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop()
{
  pwm.setPWM(6, 0, 250); // rotate base back

  pwm.setPWM(3, 0, 150); // lower upper arm
  pwm.setPWM(7, 0, 400); // open grip

  delay(2000);

  pwm.setPWM(7, 0, 150); // engage grip
  
  delay(1000);

  pwm.setPWM(3, 0, 100); // lift up upper arm 
  pwm.setPWM(6, 0, 150); // rotate base

  delay(500);

  pwm.setPWM(3, 0, 150); // lower upper arm
  pwm.setPWM(7, 0, 400); // open grip

  // pins 4 and 5 are the "forearm"

  delay(2000);
}