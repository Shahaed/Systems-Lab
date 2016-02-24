
int current = HIGH; //Call current switch high
int reading; //Used in loop
int previous = LOW; 
int IN = 2; //Pin 2 is input --from switch
int OUT = 13; //Output pin to LED is 13


//debounce code to remove noice from the circuit
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers

void setup()
{

	pinMode(OUT, OUTPUT);
	pinMode(IN, INPUT);


}

void loop()
{

	reading = digitalRead(IN); //read current switch input

	if (reading == HIGH && previous == LOW && millis() - time > debounce) { //if input went from LOW to HIGH i.e. switch was switched and delay was long enough for debounce 
		if (current == HIGH) //changes state of led from on to off
			current = LOW;
		else
			current = HIGH;   // off to on

		time = millis();
	}

	digitalWrite(OUT, current); //actual output to UNO

	previous = reading; //for the loop if statement

}

