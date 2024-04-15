#include <PWM.h>

class SparkMax
{
  private: int pwmPin;

  public:
  SparkMax(int pwmPin)
  {
    this->pwmPin = pwmPin;
  }

  void setUpSparkMax()
  {
    InitTimersSafe(); 
    bool success = SetPinFrequencySafe(pwmPin,200);

    if(success)
    {
      pinMode(pwmPin, OUTPUT);
      digitalWrite(pwmPin, HIGH);
    }
  }

  void setMotorSpeed(int speedPercentage)
  {
    int inputSpeed = map(speedPercentage,-100,100,51,102);
    pwmWrite(pwmPin, inputSpeed);
  }
};

class FlySky{
  private:
  int channelPins[6];
  int channelValues[6];
  readChannel(int channelNumber){
    int reading = pulseIn(channelPins[channelNumber - 1], HIGH, 3000);
    if(reading < 100)
    {
      return 0;
    }
    return map(reading, 1000, 2000, -100, 100);
  }

  public:
  FlySky(int ch1Pin, int ch2Pin, int ch3Pin, int ch4Pin, int ch5Pin, int ch6Pin)
  {
    channelPins[0] = ch1Pin;
    channelPins[1] = ch2Pin;
    channelPins[2] = ch3Pin;
    channelPins[3] = ch4Pin;
    channelPins[4] = ch5Pin;
    channelPins[5] = ch6Pin;
  }
  void updateRC(){
    for(int i = 0; i <= sizeof(channelValues) - 1; i++){
      channelValues[i] = readChannel(channelPins[i]);
    }
  }
  int getChannelReadingPWM(int channelNumber){
    return channelValues[channelNumber - 1];
  }
  bool getChannelReadingBool(int channelNumber){
    int value = channelValues[channelNumber - 1];
    if(value > 0){
      return true;
    }
    else{
      return false;
    }
  }
};

//   _______________________________________________________________ DO NOT TOUCH ANYTHING ABOVE THIS LINE _____________________________________________________________________

//# SET THE MOTOR INPUT PINS
int rightMotorOutputPin = 9; //# right motor pwm pin number
int leftMotorOutputPin = 10; //# left motor pwm pin number


//# SET THE RADIO INPUT PINS
int radioRightStickInput = 5; //# stick input for right motor
int radioLeftStickInput = 6; //# stick input for left motor

int radioSpareInput1 = 0; //# can be used for other functions; set to non-used pin if not in use
int radioSpareInput2 = 1; //# can be used for other functions; set to non-used pin if not in use
int radioSpareInput3 = 2; //# can be used for other functions; set to non-used pin if not in use
int radioSpareInput4 = 3; //# can be used for other functions; set to non-used pin if not in use


SparkMax rightMotor(rightMotorOutputPin);
SparkMax leftMotor(leftMotorOutputPin);

FlySky remoteRadio(radioRightStickInput,radioLeftStickInput,radioSpareInput1,radioSpareInput2,radioSpareInput3,radioSpareInput4);



int leftRemoteSpeed = 0;
int rightRemoteSpeed = 0;

void setup() {
  // put your setup code here, to run once:

  delay(5000);
  leftMotor.setUpSparkMax();
  rightMotor.setUpSparkMax();

}

void loop() {
  // put your main code here, to run repeatedly:

  remoteRadio.updateRC(); //# pull all of the values from the radio

  leftRemoteSpeed = remoteRadio.getChannelReadingPWM(radioLeftStickInput);
  rightRemoteSpeed = remoteRadio.getChannelReadingPWM(radioRightStickInput);

//# DONT WORRY ABOUT MAPPING INPUT SPEED TO PWM SPEED THEY ARE BOTH ON -100 TO 100 SCALE.

  leftMotor.setMotorSpeed(leftRemoteSpeed);
  rightMotor.setMotorSpeed(rightRemoteSpeed);

}
