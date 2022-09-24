#include <Wire.h>                     // I2C library
#include <LiquidCrystal_I2C.h>        // LCD-1602-I2C library
#include <SoftwareSerial.h>

//Sensor Define
LiquidCrystal_I2C lcd(0x27,20,4);     // address : 0x3F or 0x27
SoftwareSerial bluetooth(2,3);

//Control Value
const int AXIS_X = A0;
const int AXIS_Y = A1;
int i;
int scene_number=1;      // lcd Scene Number
int data;                // Num = i Data
int data_past = 1;       // Num = i - 1 Data
int mass = 0;            // Desired Valuew-Mass[kg]



void setup()
{
  Serial.begin(9600);
  bluetooth.begin(9600);
  
  //sensor setting
  Serial.print("Lemote control Start");
  pinMode(8, INPUT_PULLUP);
}


void loop()
{
  lcd.init();              // LCD Setting
  lcd.backlight();         // LCD Setting
  data = digitalRead(8);   // define input pin number


  if( data == 0 and  data_past == 1 ) // When the past and current z-axis values are different when turning the loop, 
  {
    scene_number = scene_number == 1 ? 0 : 1;
  }
 
  if(scene_number == 1)
  {
   lcd.setCursor(0,0);           // setting cursor at (0,0)           
   lcd.print("MASS: kg");
   lcd.setCursor(0,3);           // setting cursor at (0,3)            
   lcd.print("setting");
   lcd.setCursor(5,0);           // setting cursor at (5,0)
   lcd.print(mass);              // show that mass
  }

  if(scene_number == 0 )
  {
   lcd.setCursor(0,0);           // setting cursor at (0,0)           
   lcd.print("MASS: kg");
   lcd.setCursor(0,3);           // setting cursor at (0,3)           
   lcd.print("BACK");
   lcd.setCursor(5,0);           // setting cursor at (5,0)           
   lcd.print(mass);
   lcd.setCursor(5,0);           // setting cursor at (5,0)           
   lcd.cursor();
   lcd.blink();

   mass = analogRead(AXIS_Y)<=300 ? mass+1 : mass;  // Joystic <=
   mass = analogRead(AXIS_Y)>=700 ? mass-1 : mass;  // Joystic =>
   delay(10);
  }

  if (bluetooth.available())
  {
    Serial.write(bluetooth.read()); 
  }
  
  bluetooth.write(mass);
  delay(1000);
  data_past = data;
}
