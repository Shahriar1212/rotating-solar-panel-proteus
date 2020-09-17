#include <Servo.h>
#include <LiquidCrystal.h>

#include <Keypad.h>

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'7', '8', '9', 'A'},
  {'4', '5', '6', 'B'},
  {'1', '2', '3', 'C'},
  {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {A3, A2, A1, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
const int lm35_pin = A8;


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position



int led_1 = 22;
int led_2 = 23;
int led_3 = 24;

int ldr_1 = A4;
int ldr_2 = A5;
int ldr_3 = A6;
int charge = 52;
int discharge = 53;

bool check_1 = false;
bool check_2 = false;
bool check_3 = false;

bool cp = false;
bool dp = false;

int current_pos = 0;
int next_pos = 0;

int value_1 = 0;
int value_2 = 0;
int value_3 = 0;
int value_4 = 0;
int value_5 = 5;

int previous_pos = 0;


bool isInAuto = true;
bool isInManual = false;



int maxx = 0;
void setup() {

  lcd.begin(16, 2);
  Serial1.begin(9600);
  myservo.attach(3);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);

  pinMode(charge, INPUT);
  pinMode(discharge, INPUT);
}

void loop() {

  controlPad();
  solarMode();
  

  
}

void solarMode(){
  if(isInAuto == true){
    solarAuto();
  }
  else if(isInManual == true){
    solarManual();
  }
  turnOnLed();
}


void controlPad2(){
  char c = keypad.getKey();
  
  if(c== '1'){
    lcd.clear();
    lcd.setCursor(0,0);
    temperature();
  }
}

void controlPad(){
  char c = keypad.getKey();

  if(c == '1'){
    displayChargingStatus();
    printSolarStatus();
    delay(5000);
    lcd.clear();
  } 
  else if(c == '2'){
    isInAuto = true;
    isInManual = false;
    printSolarStatus();
    lcd.clear();
  }
  else if(c == '3'){
    isInManual = true;
    isInAuto = false;
    printSolarStatus();
    lcd.clear();
  }
  else if(c == '4'){
    lcd.clear();
    lcd.setCursor(0,0);
    temperature();
    printSolarStatus();
    delay(5000);
    lcd.clear();
  }
  else if(isInManual && c == '7'){
    myservo.write(57);
  }
  else if(isInManual && c == '8'){
    myservo.write(92);
  }
  else if(isInManual && c == '9'){
    myservo.write(128);
  }


   printSolarStatus();
   c = '0';
   lcd.setCursor(0,0);
   lcd.print("Select Option");

}

void printSolarStatus(){
  if(isInAuto){
    lcd.setCursor(10,1);
    lcd.print("Auto");
  }else{
    lcd.setCursor(10,1);
    lcd.print("Manual");
  }
}

void displayChargingStatus(){
  if(digitalRead(charge) == HIGH && cp == false){
    cp = true;
    dp = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("charging");
  } 
  else if(digitalRead(discharge) == HIGH && dp == false){
    cp = false;
    dp = true;
    lcd.clear();
    lcd.print("discharging");
  }
}


void temperature() {
  lcd.clear();
  lcd.setCursor(0,0);
  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(lm35_pin);
  temp_val = (temp_adc_val * 4.88);
  temp_val = (temp_val / 10);
  lcd.print("Temp: ");
  lcd.print(temp_val);
}






void solarManual(){
//  char c = keypad.getKey();
//
//  if(isInManual && c == '7'){
//    myservo.write(57);
//  }
//  else if(isInManual && c == '8'){
//    myservo.write(92);
//  }
//  else if(isInManual && c == '9'){
//    myservo.write(128);
//  }
}

void solarAuto() {
  value_1 = analogRead(ldr_1);
  value_2 = analogRead(ldr_2);
  value_3 = analogRead(ldr_3);


  if (value_1 > value_2 && value_1 > value_3) {
    maxx = value_1;
//    Serial1.println("LDR 1 is maximum");
    if (check_1 == false) {
      current_pos = previous_pos;
      next_pos = 57;
      if (next_pos > previous_pos) {
        rotateRight(current_pos, next_pos);

      }
      else {
        rotateLeft(current_pos, next_pos);
      }

      check_1 = true;
      check_2 = false;
      check_3 = false;
      previous_pos = next_pos;
    }

  }
  else if (value_2 > value_1 && value_2 > value_3) {
    maxx = value_2;
//    Serial1.println("LDR 2 is Maximum");
    if (check_2 == false) {
      current_pos = previous_pos;
      next_pos = 92;
      if (next_pos > previous_pos) {
        rotateRight(current_pos, next_pos);
      }
      else {
        rotateLeft(current_pos, next_pos);
      }
      check_2 = true;
      check_1 = false;
      check_3 = false;
      previous_pos = next_pos;
    }
  }

  else if (value_3 >= value_1 && value_3 >= value_2) {
    maxx = value_3;
//    Serial1.println("LDR 3 is Maximum");
    if (check_3 == false) {
      current_pos = previous_pos;
      next_pos = 128;
      if (next_pos > previous_pos) {
        rotateRight(current_pos, next_pos);
      }
      else {
        rotateLeft(current_pos, next_pos);
      }
      check_3 = true;
      check_1 = false;
      check_2 = false;
      previous_pos = next_pos;
    }
  }

}

void rotateRight(int current_pos, int next_pos) {
  Serial1.println("Servo is rotating RIGHT");
  Serial1.print("current_pos: ");
  Serial1.println(current_pos);
  for (pos = current_pos; pos <= next_pos; pos++) { // goes from 0 degrees to 360 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //    delay(20);                       // waits 20ms for the servo to reach the position
  }
}
void rotateLeft(int current_pos, int next_pos) {
  Serial1.println("servo is rotating LEFT");
  Serial1.print("current_pos: ");
  Serial1.println(current_pos);
  for (pos = current_pos; pos > next_pos; pos--) { // goes from 360 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //    delay(20);                       // waits 20ms for the servo to reach the position
  }
}

void rotate(int pos) {
  myservo.write(pos);
}

void turnOnLed() {
  if (maxx == value_1) {
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
  }
  else if (maxx == value_2) {
    digitalWrite(led_2, HIGH);
    digitalWrite(led_1, LOW);
    digitalWrite(led_3, LOW);
  }
  else if (maxx == value_3) {
    digitalWrite(led_3, HIGH);
    digitalWrite(led_2, LOW);
    digitalWrite(led_1, LOW);
  }
}
