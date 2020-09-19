// Including library for servo, keypad etc.
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


// LED pin number that are connected with arduino
// each LED is connected with LDR which indicates which LDR is provinding Maximum value
int led_1 = 22;
int led_2 = 23;
int led_3 = 24;

// LDR pin number (that is connected with arduino)
int ldr_1 = A4;
int ldr_2 = A5;
int ldr_3 = A6;

// charge and discharge pin number 
// [from solar inverter to the arduino]
int charge = 52;
int discharge = 53;

int current_pos = 0; // current position of the servo
int next_pos = 0; // next position of the servo

// value getting from LDR
int value_1 = 0;
int value_2 = 0;
int value_3 = 0;

// 
int previous_pos = 0;

// solar position fixed for manual mode
int solar_pos_1 = 57;
int solar_pos_2 = 92;
int solar_pos_3 = 128;

// distinguishing auto and manual mode by using boolean value
bool isInAuto = true; // as in the beginning the system is in the auto mode, so default true
bool isInManual = false; // when user press button '3', the system jumps to manual mode, variable becomes true

// max value among the LDR
int maxx = 0;


bool check_1 = false;
bool check_2 = false;
bool check_3 = false;


/// prototype
void solarMode(); // Runs both auto and manual mode
void turnOnLed(); // Turns the LED according to the maximum LDR value
void controlPad(); // All keypad related code [for manual mode]
void printSolarStatus(); // Is System in auto or manual mode
void displayChargingStatus(); // Are capacitor and the battery charging or not
void temperature(); // Calculate the temparature provided by the LM35 and show it to LCD
void solarManual(); // Manual Mode of the system
void solarAuto(); // Auto mode of the system
void rotateRight(int current_pos, int next_pos); // Servo rotate clockwise from starting to ending position
void rotateLeft(int current_pos, int next_pos); // Servo rotate anti-clockwise from starting to ending position



void setup() {
  
  lcd.begin(16, 2); // initializing lcd display
  Serial1.begin(9600);
  myservo.attach(3); // servo attached with arduino's pin 3

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
  turnOnLed(); // qs
}


void controlPad(){
  char c = keypad.getKey();

  if(c == '1'){
    displayChargingStatus();
    printSolarStatus(); // Manual / Auto Mode
    delay(2000);
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
    delay(2000);
    lcd.clear();
  }
  else if(isInManual && c == '7'){
    lcd.setCursor(0,1);
    lcd.print("Pos: 1");
    myservo.write(solar_pos_1);
  }
  else if(isInManual && c == '8'){
    lcd.setCursor(0,1);
    lcd.print("Pos: 2");
    myservo.write(solar_pos_2);
  }
  else if(isInManual && c == '9'){
    lcd.setCursor(0,1);
    lcd.print("Pos: 3");
    myservo.write(solar_pos_3);
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
  if(digitalRead(charge) == HIGH){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("CHARGING");
  } 
  else if(digitalRead(discharge) == HIGH){
    lcd.clear();
    lcd.print("DISCHARGING");
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

void solarAuto() {
  // getting value from the LDR
  value_1 = analogRead(ldr_1);
  value_2 = analogRead(ldr_2);
  value_3 = analogRead(ldr_3);

  // check which LDR provides the max value according 
  // to the light intensity of the torch
  if (value_1 > value_2 && value_1 > value_3) {
    maxx = value_1;
    if (check_1 == false) {
      current_pos = previous_pos; // initially prev pos is '0'
      next_pos = solar_pos_1;
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
    if (check_2 == false) {
      current_pos = previous_pos;
      next_pos = solar_pos_2;
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
    if (check_3 == false) {
      current_pos = previous_pos;
      next_pos = solar_pos_3;
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
// rotating clockwise
void rotateRight(int current_pos, int next_pos) {
  for (pos = current_pos; pos <= next_pos; pos++) {
    myservo.write(pos);
  }
}
// rotating anti-clockwise
void rotateLeft(int current_pos, int next_pos) {
  for (pos = current_pos; pos > next_pos; pos--) {
    myservo.write(pos);
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
