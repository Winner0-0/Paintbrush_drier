#include <RobotIRremote.h>
#include <RobotIRremoteInt.h>
#include <RobotIRremoteTools.h>

//#include "IRremote.h"

//Below is DC motor connections
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// make integer values that will store the inital humidity, humidity with brushes
// when humidity with brushes = inital humidy, the device will stop 

int i, f;
unsigned long dryingStart = 0;
bool dryingComplete = false;

int receiver = 12;
IRrecv irrecv(receiver);    // create instance of 'irrecv'
uint32_t last_decodedRawData = 0; //uses to store the last decodedRawData
bool motorEnabled = false;
 
void setup() {
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  irrecv.enableIRIn(); //initialize the reciever
  Serial.begin(9600);
  lcd.begin(16,2); 
  lcd.print("Brush dryer system on!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration underway");
  lcd.setCursor(0, 1);
  lcd.print("please wait");
  //put here the fgets of the value of the humidity, include:
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration completed!");
  lcd.setCursor(0, 1);
  lcd.print("insert brushes");
  delay(5000);
  lcd.clear();
}

void loop() {
  if (irrecv.decode()){ //if signal is recieved
    if (irrecv.decodedIRData.flags){ // repeat signal if you hold the button
      irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    }
    switch (irrecv.decodedIRData.decodedRawData){
      case 0xF30CFF00: //power button
       motorEnabled = !motorEnabled;
       dryingComplete = false;
       break; //makes sure it doesn't check for other cases
    }
  }
  
  if (motorEnabled && f != i){
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
    analogWrite(ENABLE, 255); //we want to have it at full speed for optimum drying
    dryingStart = millis();
    dryingComplete = false;
  }else if (motorEnabled && f == i && !dryingComplete){
    digitalWrite(ENABLE, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Drying completed :)");
    dryingStart = millis(); //for 10 minute timer
    dryingComplete = true;
  }else if(dryingComplete && millis() - dryingStart >= 600000){
    motorEnabled = false;
    dryingComplete = false;
  }else if (!motorEnabled){
    digitalWrite(Enable, LOW);
  }
       
  delay(2000);
  /** logic: wew want the speed of the motor to be depending on the value
  of the humidity. We want the device to measure the humidity
  of the area before (for calibration). The brushes will then be inserted after calibration
  If the humidity is higher than the original calibrated value, then the brush would go in at 
  max speed until the final humidity is equal to or lower than original. 
  I think it's best to just keep in highest speed always, and make the stopping of the motor fast.
  Through this process, the LCD screen will read "Calibration underway, please wait...",
  then, "Calibration completed, insert brushes", then "Drying your brushes...", then
  "Brushes dried successfully :)"**/


  //store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  irrecv.resume(); // receive the next value
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}
   
