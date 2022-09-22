// Loadcell reference| https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=eduino&logNo=221078931222
// "pca9685" board reference | https://m.blog.naver.com/aul-_-/221793940620

#include <Wire.h>                       // I2C library.
#include <Adafruit_PWMServoDriver.h>    // "pca9685" library.
#include <HX711.h>                      // Load cell library.
#include <SoftwareSerial.h>
#define DOUT  3                         // Pin Setting
#define CLK  2                          // Pin Setting


//Sensor define
Adafruit_PWMServoDriver pwm=Adafruit_PWMServoDriver();
SoftwareSerial bluetooth(5, 7);
HX711 scale(DOUT, CLK); 

//calibration Value
#define I_max 300                       // Anti-windup max
#define I_min 0                         // Anti-windup min
#define calibration_factor -7050.0      // Load cell calibration factor ( delta Voltage -> lb)
#define bangbang_control_range 00      // Load cell calibration factor ( delta Voltage -> lb)

//Communication Value - Bluetooth from remote control
int mass; // [kg]

//PID gain
float Kp = 100.0;           // P gain
float Ki = 0.0;             // I gain
float Kd = 0.8;             // D gain

//Control & PID & LPF(Low Pass Filter) Value
float r = 0;                                  // Desired Value-Mass[kg]
float y = 0;                                  // Input-Mass[kg]
float u = 0;                                  // Control Value-PWM[Hz]
float millisTime_i;                           // Initial Time Measurement[ms]
float millisTime_f;                           // Later time measurement[ms]
float dt = 0;                                 // Tiny time[ms]
float y_past = 0;                             // Past input data
int UPorDOWN;





// A "Input_data()" function that stores data received via Bluetooth from the remote control in a variable
int Input_data()
{
  if (bluetooth.available())
  {
      mass = (bluetooth.read());
  }
 
  return mass;
}


// A "lowpassfilter()" function that Low Pass Filtering
float lowpassfilter(float filter, float data, float lowpass_constant)
{
  filter = data * (1 - lowpass_constant) + filter * lowpass_constant;
  return filter;   
}

// A "computePID" function that calculates the "Control Value-PWM[Hz]" through the Error(y-r) between the "Desired value-Mass[kg]" and the "Input-Mass[kg]"
float computePID(float r, float y, float dt)
{
  float error = y - r;      //if error_sign is plus => Up | if error_sign is minus => Down
  
  if (error > 0)
  {
    UPorDOWN = 1;          //UP
  }
  else
  {
    UPorDOWN = 0;          //DOWN
  }
  
  float P = Kp * error;  
  float I = I + Ki * error * dt;
  float D = Kd * (-y + y_past) / dt;
  
  I = constrain(I , I_min , I_max);            // Create a I gain limit
  y_past = y;                                  //Update Value
  
  u = P + I + D;
  if ( abs(error)*1000 <= bangbang_control_range)
  {
    u = 0;
  }
  return u;   
}

void Show_plot()
{
  Serial.print("Desired[kg] : ");
  Serial.print(r);
  Serial.print(" | Load cell[kg] : ");
  Serial.print(y);
  Serial.print(" | PWM[Hz] : ");
  Serial.print(abs(u));
  Serial.print(" | Up or Down : ");
  if (UPorDOWN == 1)
  {
    Serial.println("Up");
  }
  else
  {
    Serial.println("Down");
  }
}

void Show_plot_graph()
{
  Serial.print(r+4);
  Serial.print(",");
  Serial.print(y+4);
  Serial.print(",");
  if (UPorDOWN == 0)
  {
    Serial.println(r - 1.5);
  }
  else{
    Serial.println(r + 1.5);
  }

}

//// A "control_motor" function that produces PWM by identifying the "PWM-frequency[Hz]" and "UPorDOWN" variables obtained from the Desired value
void control_motor(float u, int UPorDOWN)
{ 
  if (UPorDOWN == 1)
  {
    digitalWrite(10, HIGH);    //Rotate the motor to CW, which means UP
  }
  else
  {
    digitalWrite(10, LOW);   //Rotate the motor to CCW, which means DOWN
  }
  pwm.setPWMFreq(abs(u));     
  if ( u == 0 )
  {
    pwm.setPWM(0,0,0);
  }
  else
  {
    pwm.setPWM(0,0,100);
  }
  
}


void setup() 
{
  Serial.begin(9600);
  pwm.begin();
  bluetooth.begin(9600);
  
  //sensor setting
  scale.set_scale(calibration_factor);   //Load cell Scale calibration
  scale.tare();                          //Load cell calibration
  pinMode(10, OUTPUT);                   //Motor Rotation Direction Signal Output Pin

  Input_data();
  delay(5000);
}


void loop() 
{ 
  millisTime_i = millis();
  
  y = lowpassfilter(y , -0.453592 * scale.get_units() / 5.9386 , 0.1); //Note* 1lb = 0.453592kg 
  y = y <= 0 ? 0 : y;
  //r = lowpassfilter(r, Input_data(), 0.7);
  r = 1;
  u = computePID(r, y, dt);
    
  control_motor(u, UPorDOWN);
  //Show_plot();
  Show_plot_graph();
  
  millisTime_f = millis();
  dt = millisTime_f - millisTime_i;
}
