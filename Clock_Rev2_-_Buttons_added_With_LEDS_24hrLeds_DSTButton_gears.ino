#include "RTClib.h"
#include <Stepper.h>

RTC_DS3231 rtc;

int skip = 0;
const int stepsPerRevolution = 200; //for the motor itself, total steps w/ reduction ~2038

//Stepper clockStepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper clockStepperMin(stepsPerRevolution, 11, 9, 10, 8);
Stepper clockStepperHr(stepsPerRevolution, 7, 5, 6, 4);

int timer = 100;           // The higher the number, the slower the timing.
int ledPins1[] = {31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 46, 53};       // an array of pin numbers to which LEDs are attached
int ledPins2[] = {33, 35, 37, 39, 41, 43, 45, 47, 49, 46, 53, 31};       // an array of pin numbers to which LEDs are attached
int ledPins3[] = {35, 37, 39, 41, 43, 45, 47, 49, 46, 53, 31, 33};       // an array of pin numbers to which LEDs are attached
int ledPins4[] = {37, 39, 41, 43, 45, 47, 49, 46, 53, 31, 33, 35};       // an array of pin numbers to which LEDs are attached
int ledPins5[] = {39, 41, 43, 45, 47, 49, 46, 53, 31, 33, 35, 37};       // an array of pin numbers to which LEDs are attached
int ledPins6[] = {41, 43, 45, 47, 49, 46, 53, 31, 33, 35, 37, 39};       // an array of pin numbers to which LEDs are attached
int ledPins7[] = {43, 45, 47, 49, 46, 53, 31, 33, 35, 37, 39, 41};       // an array of pin numbers to which LEDs are attached
int ledPins8[] = {45, 47, 49, 46, 53, 31, 33, 35, 37, 39, 41, 43};       // an array of pin numbers to which LEDs are attached
int ledPins9[] = {47, 49, 46, 53, 31, 33, 35, 37, 39, 41, 43, 45};       // an array of pin numbers to which LEDs are attached
int ledPins10[] = {49, 46, 53, 31, 33, 35, 37, 39, 41, 43, 45, 47};       // an array of pin numbers to which LEDs are attached
int ledPins11[] = {46, 53, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49};       // an array of pin numbers to which LEDs are attached
int ledPins12[] = {53, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 46};       // an array of pin numbers to which LEDs are attached

int pinCount = 12;           // the number of pins (i.e. the length of the array)

byte ledState0A = LOW;     // the current state of LED
byte ledState1A = LOW;     // the current state of LED
byte ledState2A = LOW;     // the current state of LED
byte ledState3A = LOW;     // the current state of LED

void setup () {
  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);

//#ifndef ESP8266
//  while (!Serial); // for Leonardo/Micro/Zero
//#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
     //rtc.adjust(DateTime(2022, 1, 1, 16, 59, 0));
         //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  clockStepperMin.setSpeed(100);
  clockStepperHr.setSpeed(100);
}

void loop() {
  DateTime now = rtc.now();

  int dstButton = digitalRead(14);
  if (dstButton == LOW){
    Serial.println ("GMT");
  }else{
    Serial.println ("BST");
  }
  int buttonForwardValueHr = digitalRead(12);
  if (buttonForwardValueHr == LOW) {
    clockStepperHr.step(15); //advance ~1:00
  }
  int buttonBackwardValueHr = digitalRead(13);
  if (buttonBackwardValueHr == LOW) {
    clockStepperHr.step(-15); //advance ~1:00
  }
  int buttonForwardValueMin = digitalRead(2);
  if (buttonForwardValueMin == LOW) {
    clockStepperMin.step(15); //advance ~1:00
  }
  int buttonBackwardValueMin = digitalRead(3);
  if (buttonBackwardValueMin == LOW) {
    clockStepperMin.step(-15); //advance ~1:00
  }
  Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + now.timestamp(DateTime::TIMESTAMP_FULL));
  //  int buttonForwardValue = digitalRead(4);
  //Serial.print(now.minute(), DEC); Serial.print("  "); Serial.print(now.second()); Serial.print("  "); Serial.println(buttonForwardValue);
  //delay(10);

  //  if (buttonForwardValue == LOW) {
  //Serial.println("advance stepper 34 steps");
  //    clockStepperMin.step(-34); //advance ~1:00
  //  }
  if (now.second() == 0) {
    skip += 1;
    if (skip != 29) {
      //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
      clockStepperMin.step(-68); //advance ~1:00
      delay(2000);
    }
    else {
      skip = 0;
      //Serial.print("1 step skipped to keep correct time"); Serial.print(" skip "); Serial.println(skip);
      clockStepperMin.step(-72); //advance ~1:00 minus 1 step to keep clock in time
      delay(2000);
    }
  }
  else {
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
  }

  if (now.second() == 5 && now.minute() == 0 && now.hour() == 1) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      One_oclock();
    } else {
      Two_oclock();
    }
  }

  if (now.second() == 5 && now.minute() == 0 && now.hour() == 2) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Two_oclock();
    } else {
      Three_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 3) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Three_oclock();
    } else {
      Four_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 4) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Four_oclock();
    } else {
      Five_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 5) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Five_oclock();
    } else {
      Six_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 6) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Six_oclock();
    } else {
      Seven_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 7) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Seven_oclock();
    } else {
      Eight_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 8) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Eight_oclock();
    } else {
      Nine_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 9) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
     if (dstButton == LOW) {
      Nine_oclock();
    } else {
      Ten_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 10) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Ten_oclock();
    } else {
      Eleven_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 11) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
     if (dstButton == LOW) {
      Eleven_oclock();
    } else {
      Midnight_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 12) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Midnight_oclock();
    } else {
      One_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 13) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      One_oclock();
    } else {
      Two_oclock();
    }
  }

  if (now.second() == 5 && now.minute() == 0 && now.hour() == 14) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
     if (dstButton == LOW) {
      Two_oclock();
    } else {
      Three_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 15) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Three_oclock();
    } else {
      Four_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 16) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Four_oclock();
    } else {
      Five_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 17) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    if (dstButton == LOW) {
      Five_oclock();
    } else {
      Six_oclock();
    }
    delay(2000);

  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 18) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Six_oclock();
    } else {
      Seven_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 19) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Seven_oclock();
    } else {
      Eight_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 20) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
     if (dstButton == LOW) {
      Eight_oclock();
    } else {
      Nine_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 21) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(340); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Nine_oclock();
    } else {
      Ten_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 22) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
     if (dstButton == LOW) {
      Ten_oclock();
    } else {
      Eleven_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 23) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Eleven_oclock();
    } else {
      Midnight_oclock();
    }
  }
  if (now.second() == 5 && now.minute() == 0 && now.hour() == 0) {
    //Serial.print("advance stepper 34 steps"); Serial.print(" skip "); Serial.println(skip);
    clockStepperHr.step(342); //advance ~1:00
    delay(2000);
    if (dstButton == LOW) {
      Midnight_oclock();
    } else {
      One_oclock();
    }
  }

  else {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
  }

}

void Midnight_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins12[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins12[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}



void One_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins1[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins1[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}



void Two_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins2[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins2[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}




void Three_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins3[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins3[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}


void Four_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins4[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins4[k], LOW);
      delay(timer);
    }
  }
  //digitalWrite(ledPins11[5], HIGH);
  ledState0A = LOW;
}



void Five_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins5[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins5[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}




void Six_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins6[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins6[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}




void Seven_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins7[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins7[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}




void Eight_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins8[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins8[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}




void Nine_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins9[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins9[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}





void Ten_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins10[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins10[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}





void Eleven_oclock() {

  ledState0A = HIGH;
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins11[thisPin], HIGH);
    delay(timer);
  }
  if (ledState0A = HIGH) {
    for (int k = 1; k < pinCount; k++) {
      // turn the pin off:
      digitalWrite(ledPins11[k], LOW);
      delay(timer);
    }
  }
  ledState0A = LOW;
}
