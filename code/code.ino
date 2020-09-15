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

bool check_1 = false;
bool check_2 = false;
bool check_3 = false;

int current_pos = 0;
int next_pos = 0;

int value_1 = 0;
int value_2 = 0;
int value_3 = 0;
int value_4 = 0;
int value_5 = 5;

int previous_pos = 0;



int maxx = 0;
void setup() {

  lcd.begin(16, 2);
  //  lcd.setCursor(0,1);
  //  lcd.clear();
  //  lcd.print("hey bro");




  Serial1.begin(9600);
  myservo.attach(3);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
}
void loop() {
  //lcd_demo();
//  modeSetting();
  myFunc();
  turnOnLed();

  temperature();



}

void lcd_demo() {

  lcd.print("Temp :");

  lcd.setCursor(0, 1);
  lcd.print("AUTO");

  lcd.setCursor(7, 1);
  lcd.print("MOIN");
  lcd.clear();
}



void temperature() {
  //  lcd.setCursor(2,0);
  int temp_adc_val;
  float temp_val;
  temp_adc_val = analogRead(lm35_pin);
  temp_val = (temp_adc_val * 4.88);
  temp_val = (temp_val / 10);
  //lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp_val);

}

void modeAuto(){
  
}
void modeManual(){
  
}

void modeSetting() {
  char key = keypad.getKey();
  lcd.setCursor(0, 0);
  lcd.print("Enter Mode: ");
  
  if (key == '1') {
    modeAuto();
    Serial.print(key);
    lcd.print(key);
  }
  else if(key == '2'){
    modeManual();
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Wrong Mode");
    delay(500);
    lcd.setCursor(0,0);
    lcd.print("1: AUTO");
    lcd.setCursor(0,1);
    lcd.print("2: MALUAL");
  }
}


void myFunc() {
  value_1 = analogRead(ldr_1);
  value_2 = analogRead(ldr_2);
  value_3 = analogRead(ldr_3);


  if (value_1 > value_2 && value_1 > value_3) {
    maxx = value_1;
    Serial1.println("LDR 1 is maximum");
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
    Serial1.println("LDR 2 is Maximum");
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
    Serial1.println("LDR 3 is Maximum");
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