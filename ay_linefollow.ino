/*
Code Name: Arduino Line Follower Robot Car
Code URI: https://circuitbest.com/category/arduino-projects/
Author: Make DIY
Author URI: https://circuitbest.com/author/admin/
Description: This program is used to make Arduino Line Follower Robot Car.
Note: You can use any value between 0 to 255 for M1_Speed, M2_Speed, LeftRotationSpeed, RightRotationSpeed.
Here 0 means Low Speed and 255 is for High Speed.
Version: 1.0
License: Remixing or Changing this Thing is allowed. Commercial use is not allowed.
*/

  int count = 0;
  int branch = 0;

#define in1 2                                                                                                                          
#define in2 4
#define in3 7
#define in4 8
#define enA 6
#define enB 5

#define ADVANCE_MS  150  // TUNE creep forward at the junction so the wheels sit over it
#define MIN_TURN_MS 100  // TUNE ignore the sensors briefly when a pivot starts
#define MAX_TURN_MS 1000 // give up so the bot can never spin forever


 int M1_Speed = 80; // speed of motor 1
 int M2_Speed = 80; // speed of motor 2
 int LeftRotationSpeed = 95;  // Left Rotation Speed
 int RightRotationSpeed = 95; // Right Rotation Speed


 void setup() {

  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);

    pinMode(enA,OUTPUT);
    pinMode(enB,OUTPUT);

      pinMode(A0, INPUT); // initialize Left sensor as an input
      pinMode(A1, INPUT); // initialize Right sensor as an input
      pinMode(A2, INPUT); // initialize left branch sensor as an input
      pinMode(A3, INPUT); // initialize right branch sensor as an input

}

void loop() {

  int LEFT_SENSOR = digitalRead(A0);
  int RIGHT_SENSOR = digitalRead(A1);
  int J_LEFT_SENSOR = digitalRead(A2);
  int J_RIGHT_SENSOR = digitalRead(A3);

  if(RIGHT_SENSOR==1 && LEFT_SENSOR==1) {
    
    Stop();  //junction STOP
    count = count + 1;
   
    if(count == 1 || count == 5){

      turnRightToLine();
      delay(1000);
      M1_Speed = 60; // speed of motor 1
      M2_Speed = 60; // speed of motor 2
      LeftRotationSpeed = 90;  // Left Rotation Speed
      RightRotationSpeed = 90; // Right Rotation Speed

      if(J_LEFT_SENSOR == 1){
        Stop();
      }
      return;
    }

    else if(count == 2){
      turnLeftToLine();
      return;
    }

    else{
      backward(); delay(100); return;
    }

 }

  else if(RIGHT_SENSOR==0 && LEFT_SENSOR==0) {
    backward(); //FORWARD
  }

  else if(RIGHT_SENSOR==0 && LEFT_SENSOR==1) {
    left(); //Move Left
  }

  else if(RIGHT_SENSOR==1 && LEFT_SENSOR==0) {
    right(); //Move Right
  }

}


void turnRightToLine()
{
  right();
  unsigned long t0 = millis();

  while (millis() - t0 < MIN_TURN_MS) {}                              // 1. get turning
  while (digitalRead(A1) == 0 && millis() - t0 < MAX_TURN_MS) {}      // 2. right sensor reaches the new line
  while (!(digitalRead(A0) == 0 && digitalRead(A1) == 0) && millis() - t0 < MAX_TURN_MS) {
    right();
  }     // 3. line centred between the sensors
  Stop();
}


void turnLeftToLine()
{
  left();
  unsigned long t0 = millis();

  while (millis() - t0 < 150) {}
  while (digitalRead(A0) == 0 && millis() - t0 < MAX_TURN_MS) {}      // left sensor reaches the new line
  while (!(digitalRead(A0) == 0 && digitalRead(A1) == 0) && millis() - t0 < MAX_TURN_MS) {  }
  Stop();
}


void forward()
{
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);

                analogWrite(enA, M1_Speed);
                analogWrite(enB, M2_Speed);
}

void backward()
{
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);

                analogWrite(enA, M1_Speed);
                analogWrite(enB, M2_Speed);
}

void right()
{
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);

                analogWrite(enA, LeftRotationSpeed);
                analogWrite(enB, RightRotationSpeed);
}

void left()
{
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);

                analogWrite(enA, LeftRotationSpeed);
                analogWrite(enB, RightRotationSpeed);
}

void Stop()
{
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
            delay(4000);
}


// void left()    // turn left: right wheel drives, left wheel stops
// {
//             // motor A (right wheel) forward
//             digitalWrite(in1, HIGH);
//             digitalWrite(in2, LOW);
//             analogWrite(enA, 90);

//             // motor B (left wheel) stopped
//             digitalWrite(in3, LOW);
//             digitalWrite(in4, LOW);
//             analogWrite(enB, 0);
// }

// void right()   // turn right: left wheel drives, right wheel stops
// {
//             // motor B (left wheel) forward
//             digitalWrite(in3, HIGH);
//             digitalWrite(in4, LOW);
//             analogWrite(enB, 90);

//             // motor A (right wheel) stopped
//             digitalWrite(in1, LOW);
//             digitalWrite(in2, LOW);
//             analogWrite(enA, 0);
// }
