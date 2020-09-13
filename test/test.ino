#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position



int ldr_1=A1;
int ldr_2=A2;
int ldr_3=A3;
int ldr_4=A4;
int ldr_5=A5;

bool check_1 = false;
bool check_2 = false;
bool check_3 = false;
bool check_4 = false;
bool check_5 = false;

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
  Serial1.begin(9600);
  myservo.attach(13);
}
void loop() {
//  for (pos = 50; pos < 130; pos++) { // goes from 0 degrees to 360 degrees
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(50);                       // waits 20ms for the servo to reach the position
//  }
//
//  for (pos = 130; pos > 50; pos--) { // goes from 0 degrees to 360 degrees
//    myservo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(50);                       // waits 20ms for the servo to reach the position
//  }
//  delay(7000);
  myFunc();

//  myservo.write(45);
//  delay(3000);
//  myservo.write(90);
//  delay(3000);
//  myservo.write(145);
//  delay(3000);
   
}


void myFunc(){
  value_1 = analogRead(ldr_1);
  value_2 = analogRead(ldr_2);
  value_3 = analogRead(ldr_3);
  value_4 = analogRead(ldr_4);


  if(value_1 > value_2 && value_1 > value_3){
    maxx = value_1;
    Serial1.println("LDR 1 is maximum");
    if(check_1 == false){
      current_pos = previous_pos;
      next_pos = 75;
      //rotate(75);
      if(next_pos > previous_pos){
        rotateRight(current_pos,next_pos);
          
      }
      else{
        rotateLeft(current_pos,next_pos);
      }
      
      check_1 = true;
      check_2 = false;
      check_3 = false;
      previous_pos = next_pos;
    }
    
  }
  else if(value_2 > value_1 && value_2 > value_3){
    maxx = value_2;
    Serial1.println("LDR 2 is Maximum");
    if(check_2 == false){
      current_pos = previous_pos;
      next_pos = 90;
      //rotate(90);
      if(next_pos > previous_pos){
        //rotateRight(current_pos,next_pos);  
      }
      else{
        //rotateLeft(current_pos,next_pos);
      }
      check_2 = true;
      check_1 = false;
      check_3 = false;
      previous_pos = next_pos;
    }
  }

  else if(value_3 >= value_1 && value_3 >= value_2){
    maxx = value_3;
    Serial1.println("LDR 3 is Maximum");
    if(check_3 == false){
      current_pos = previous_pos;
      next_pos = 45;
      rotate(130);
      if(next_pos > previous_pos){
        //rotateRight(current_pos,next_pos);  
      }
      else{
        //rotateLeft(current_pos,next_pos);
      }
      check_3 = true;
      check_1 = false;
      check_2 = false;
      previous_pos = next_pos;
    }
  }


  
  delay(2000);
}

void rotateRight(int current_pos, int next_pos){
  Serial1.println("Servo is rotating RIGHT");
  Serial1.print("current_pos: ");
  Serial1.println(current_pos);
  for (pos = current_pos; pos <= next_pos; pos++) { // goes from 0 degrees to 360 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial1.println(pos);
    delay(20);                       // waits 20ms for the servo to reach the position
  }
}
void rotateLeft(int current_pos,int next_pos){
  Serial1.println("servo is rotating LEFT");
  Serial1.print("current_pos: ");
  Serial1.println(current_pos);
  for (pos = current_pos; pos > next_pos; pos--) { // goes from 360 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial1.println(pos);
    delay(20);                       // waits 20ms for the servo to reach the position
  }
}

void rotate(int pos){
  myservo.write(pos);
}
