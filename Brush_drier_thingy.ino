 /** logic: wew want the speed of the motor to depend on the value
  of the humidity. We want the device to measure the humidity
  of the area before (for calibration). The brushes will then be inserted after calibration
  If the humidity is higher than the original calibrated value, then the fan attached to the motor will spin at 
  max speed until the final humidity is equal to or lower than the original. 
  I think it's best to just keep at the highest speed always, and make the stopping of the motor fast.
  Through this process, the LCD screen will read "Calibration underway, please wait...",
  then "Calibration completed, insert brushes", then "Drying your brushes...", then
  "Brushes dried successfully :)"**/

#include <RobotIRremote.h>
#include <RobotIRremoteInt.h>
#include <RobotIRremoteTools.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <math.h>

//#include "IRremote.h"

//Below is DC motor connections
#define ENABLE 5
#define DIRA 3
#define DIRB 2
//DHT11 settings
#define DHTPIN 8
#define DHTTYPE DHT11
LiquidCrystal lcd(13, 11, 4, 10, 6, 7); //RS, enable, d4, d5, d6, d7
DHT dht(DHTPIN, DHTTYPE);

// make integer values that will store the initial humidity, humidity with brushes
// when humidity with brushes = initial humidity, the device will stop 

float humidity;
const float hysT = 5.0; //tolerance value in place to prevent rapid on/off from the system from fluctuations in humidity readings
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
  humidity = dht.readHumidity();
  i = (int)round(humidity); // this is now the intial humidity value, will act as our origin value
  delay(6000); // DHT11 reaches humidity reading within 6 seconds (data sheet)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibration completed!");
  lcd.setCursor(0, 1);
  lcd.print("insert brushes");
  delay(5000);
  lcd.clear();
}

void loop() {
  humidity = dht.readHumidity();
  f = (int)round(humidity); // will get a new f value each time, i will remain unchanged
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
  
  if (motorEnabled && (f > i + hysT)){
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);
    analogWrite(ENABLE, 255); //we want to have it at full speed for optimum drying
    dryingStart = millis();
    dryingComplete = false;
  }else if (motorEnabled && (f <= i) && !dryingComplete){
    digitalWrite(ENABLE, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Drying completed :)");
    dryingStart = millis(); //for 10 minute timer
    dryingComplete = true;
  }else if (!motorEnabled){
    digitalWrite(ENABLE, LOW);
  }
 if (dryingComplete && millis() - dryingStart >= 500000){
    motorEnabled = false;
    dryingComplete = false;
 }
  delay(2000);
}
   
