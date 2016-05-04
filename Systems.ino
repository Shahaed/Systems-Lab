/* Name: UV Photomeasurments
 * Programmer: Shahaed Hasan and Michael Hass
 * Date: May 3rd, 2016
 * Licence: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 * Professor Osama Saba can use this only if we get an A in the Systems class
 * 
 * Use for GUVA-S12SD UV-PhototDiode as UV Sensor
 */


int ledPins[6] = {8, 9, 10, 11, 12, 13};
float maxI = 2.00; // max intensity, measured from fluorescent bulb
float minI = 0; // min intensity
float calI = 0; // calibrated intensity
float maxUV = 6.00; // max UV, measured  from sun
float minUV = 0; // min UV
float calUV = 0; // calbrated UV
int MaxCal; //Setup Runs and will set theses numbers -- will be between 1023&0
int MinCal;
const int  buttonPin = 2; //The botton pin
const int numberOfReadings = 64;



void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  //Pins For LEDs
  for (int thisPin = 0; thisPin < 6; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 /* Calibration stage -runs once 
  * Stage 1 runs when there is no UV light. Measures the voltage and determines that as MinCal. 
  * While in this stage Blue LED is solid. 
  * 
  * Change to stage 2 by hitting the button. The blue LED flashes until that stage is complete. 
  * Hitting the button exits the calibration stage. 
  */

  int counter = 0;
  int buttonState = 0; 
  int lastButtonState = 0; 
  
  while (counter < 1){

    buttonState = digitalRead(buttonPin);
    digitalWrite(3, HIGH); //Blue LED

    MinCal = averageAnalogRead(A0);
     
    if (buttonState != lastButtonState) { //Check if button is pressed
    // if the state has changed, increment the counter
     if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      counter++;
     } 
    }
    
    // Delay a little bit to avoid bouncing
    delay(50);
    lastButtonState = buttonState;
    
  } 

  digitalWrite(3,LOW);
  while (counter < 2){ //Second Stage
    
    buttonState = digitalRead(buttonPin);
    
    digitalWrite(3, HIGH); //Blue LED
    digitalWrite(4, HIGH); //Blue LED

    MaxCal = averageAnalogRead(A0);
    
    if (buttonState != lastButtonState) { //Check if button is pressed
    // if the state has changed, increment the counter
     if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      counter++;
     } 
   }
   
   delay(50);// Delay a little bit to avoid bouncing
   lastButtonState = buttonState;
  }

  digitalWrite(3, LOW); 
  digitalWrite(4, LOW);

/////////////////////////////////////////Calibration ended -- setup ended/////////////////////////////////////////////
}







void loop() {
  // put your main code here, to run repeatedly:

  LEDoff(); //Used to turn all lights off
 
  /*************************************************************Used for debugging************************************************
  int sensorValue = averageAnalogRead(A0);//connect UV sensors to Analog 0
  Serial.print("MinCal: ");
  Serial.println(MinCal);
  Serial.print("MaxCal: ");
  Serial.println(MaxCal);
  Serial.print("Current Val: ");
  Serial.println(sensorValue);
  Serial.print("Current AVG Voltage: ");
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.println(voltage);
  
  sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  // print out the value you read:
  Serial.print("Current instant Voltage: ");
  Serial.println(voltage);
 *************************************************************Used for debugging************************************************/
 
  
  float UV = calibrationUVIndex( MaxCal, MinCal, averageAnalogRead(A0));
  int u;
  u = (int) UV;

  
  UVledArray(UV);
  delay (1000);
  
  
}



//lights the UV array of LEDs
void UVledArray(int UVindex)
{
  if (UVindex <= 6){
    LightOn(2*UVindex);
  }
}


//Turns all the LED's off
void LEDoff(){
  for (int i = 0; i < 7 ; i++){ //turn on half as many LEDs
      digitalWrite(ledPins[i], LOW);
    }
}

//Uses 6 leds to return numbers up to 12. A solid means 2, blink means 1
void LightOn(int number){ 

  if (number % 2 == 0 && number < 14){ //if even
    number = number/2;
   
    for (int i = 0; i < number ; i++){ //turn on half as many LEDs
      digitalWrite(ledPins[i], HIGH);
    }
    //delay (5000);
  }

  else if(number < 14)
    { //if odd
    int j = 0;
    for (int i = 0; i < number/2 ; i++){ //turn on half as many LEDs
      digitalWrite(ledPins[i], HIGH);
      j++;
  }
    flash(ledPins[j], 5000); //flash the odd number for 5 seconds
  }
}

//Flashes a pinLED for x seconds
void flash(int PIN, int sec){ 

  unsigned long timer = millis(); //milliseconds
  while (millis() < timer +sec){ //FOr 5 seconds
    digitalWrite(PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);              // wait for a second
    digitalWrite(PIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  } 
}

// Will take 64 readings and give the average
int averageAnalogRead(int pinToRead)
{
  
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++){
    runningValue = analogRead(pinToRead) + runningValue;
  }
  
  runningValue =  runningValue/numberOfReadings;
  return(runningValue);  
}

//Takes in a max & min calibration voltage and returns the intensity (mW/cm^2)
float calibrationIntensity( float maxV, float minV, float currentV) {

  float calI;
  return calI = minI + (maxI - minI)*((currentV-minV)/(maxV-minV));
  //Serial.println ("Intensity (mW/cm^2)", calI);
  
 }

 //Takes in a max & min calibration voltage and returns the UV index (mW/cm^2) 1-6
float calibrationUVIndex(float maxV, float minV, float currentV) {
  float calUV ;
  Serial.println("UV Index");
  
  calUV = 6.00 * ((currentV - minV)/(maxV-minV));
  Serial.println(calUV);
  return calUV;
 }

