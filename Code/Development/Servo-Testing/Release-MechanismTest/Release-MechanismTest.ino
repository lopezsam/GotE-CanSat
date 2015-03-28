#include <Servo.h> 

Servo myservo;//pin 9 or 10
			//red -> 5V pin
			//white, yellow -> 9 or 10
			//black -> ground
int x = 0;
void setup() 
{ 
  Serial.begin(9600);
  myservo.attach(9);
  myservo.write(90);  // set servo to mid-point
} 

void loop() {
	myservo.write(180);
	Serial.println(x);
        x++;
	
} 
