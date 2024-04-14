//ST-W48 Blaster Circuit
//Included in a Proton Pack Fed blaster, overheats the blaster after 30 shots.
//Written and made by Tyler Widener, tylerwidenerlm@gmail.com

//PINS:
//IR Sensor Pin (Button for Sim Purposes): 2
//Red Overheat LED: 3
//RGB Barrel LEDs: 4
//Yellow Overheat LED: 5
//Servo Control: 6
//Charging Handle (Button for Sim Purposes): 7

//Servo library for driving the servo
//blasterServo.write(0-180);
#include <Servo.h>

//Pin Variables:
const int ir = 2;
const int rLED = 3;
const int barrelLED = 4;
const int yLED = 5;
const int servoSignal = 6;
const int handle = 7;

Servo blasterServo;

const int overheatCap = 30; //Number of darts for overheat
const int overheatMax = 500; //Time overheat lasts
const int windowTime = 100; //Time for the minigame window
int overheatCount = 0;
int overheatTime = 0;
const int neutralPos = 180;
const int blockedPos = 100;
bool light = false;

volatile int darts = 0; //Represents the number of darts fired since last reset.

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), dartFired, RISING);
  pinMode(ir, INPUT_PULLUP);
  pinMode(rLED, OUTPUT);
  pinMode(barrelLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(handle, INPUT);
  blasterServo.attach(servoSignal);
  randomSeed(analogRead(0));
  blasterServo.write(neutralPos);
  Serial.begin(9600);
}
void dartFired() { //Interrupt routine to trigger when a dart is fired.
  darts++;
  Serial.println(darts);
  light = true;
}
void loop() {
  if ((digitalRead(ir) == HIGH) && (light == true)) {
    light == false;
    digitalWrite(barrelLED, HIGH);
    delay(50);
    digitalWrite(barrelLED, LOW);
  }
  if (digitalRead(handle) == HIGH) {
    digitalWrite(barrelLED, HIGH);
    blasterServo.write(blockedPos);
    Serial.println("Cooling...");
    for (overheatCount = map(darts, 0, overheatCap, 0, overheatMax); overheatCount >= 0; overheatCount--) {
      if (darts >= overheatCap) {
        break; 
      }
      delay(10);
    }
    if (!(darts >= overheatCap)) {
      blasterServo.write(neutralPos);
      darts = 0;
    }
    digitalWrite(barrelLED, LOW);
  }
  if (darts >= overheatCap) {
    Serial.println("Overheat");
    blasterServo.write(blockedPos);
    digitalWrite(barrelLED, HIGH);
    overheatTime = random((windowTime + 5), (overheatMax - 50));
    Serial.println(overheatTime);
    for (overheatCount = overheatMax; overheatCount >= overheatTime; overheatCount--) {
      //analogWrite(rLED, map(overheatCount, 0, overheatMax, 0, 255));
      digitalWrite(rLED, HIGH);
      if (digitalRead(handle) == HIGH) {
        if (digitalRead(handle) == HIGH) {
          Serial.println("Incorrect");
          delay(3000);
          darts = 0;
          digitalWrite(rLED, LOW);
          digitalWrite(barrelLED, LOW);
          blasterServo.write(neutralPos);
          break;
        }
      }
      delay(10);
    }
    if (darts > 0) {
      digitalWrite(rLED, LOW);
      digitalWrite(yLED, HIGH);
      Serial.println("Second Loop");
      while (overheatCount >= (overheatTime - windowTime)) {
        if (digitalRead(handle) == HIGH) {
        	if (digitalRead(handle) == HIGH) {
              	Serial.println("Correct.");
          		darts = 0;
          		digitalWrite(rLED, LOW);
              	digitalWrite(yLED, LOW);
          		digitalWrite(barrelLED, LOW);
          		blasterServo.write(neutralPos);
          		break;
        	}
      	}
        delay(10);
        overheatCount = overheatCount - 1;
      }
    }
    if (darts > 0) {
      digitalWrite(rLED, HIGH);
      digitalWrite(yLED, LOW);
      Serial.println("Third Loop");
      while (overheatCount > 0) {
        //analogWrite(rLED, map(overheatCount, 0, (overheatTime - windowTime), 0, 255));
        digitalWrite(rLED, HIGH);
        if (digitalRead(handle) == HIGH) {
        	if (digitalRead(handle) == HIGH) {
              	Serial.println("Incorrect.");
          		delay(3000);
          		darts = 0;
          		digitalWrite(rLED, LOW);
          		digitalWrite(barrelLED, LOW);
          		blasterServo.write(neutralPos);
          		break;
        	}
      	}
        delay(10);
        overheatCount = overheatCount - 1;
      }
    }
    darts = 0;
    digitalWrite(rLED, LOW);
    digitalWrite(barrelLED, LOW);
    blasterServo.write(neutralPos);
  }
}