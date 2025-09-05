#include <RobotIRremote.h>
#include <RobotIRremoteInt.h>
#include <RobotIRremoteTools.h>

//#include "IRremote.h"

#define ENABLE 5
#define DIRA 3
#define DIRB 4

// make integer values that will store the inital humidity, humidity with brushes
// when humidity with brushes = inital humidy, the device will stop 

int i;
int receiver = 12;
IRrecv irrecv(receiver);    // create instance of 'irrecv'
uint32_t last_decodedRawData = 0;
 
void setup() {
  //---set pin direction
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  irrecv.enableIRIn();
  Serial.begin(9600);
  lcd.begin(16,2); // Initialize the LCD with 16 columns and 2 rows
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
}

void loop() {
  if (irrecv.decode()) //for remote control connections
  {
    if (irrecv.decodedIRData.flags)
    {
      irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    }
    switch (irrecv.decodedIRData.decodedRawData)
  }
  
  digitalWrite(ENABLE,LOW); // disable
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

  // Fast stop code
  digitalWrite(ENABLE,HIGH); //enable on
  digitalWrite(DIRA,LOW); //one way
  digitalWrite(DIRB,HIGH);
  delay(3000);
  digitalWrite(DIRA,HIGH); //fast stop
  delay(2000);

  // Full speed code
  analogWrite(ENABLE,255); //enable on
  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  delay(3000);

  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  irrecv.resume(); // receive the next value
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}
   
