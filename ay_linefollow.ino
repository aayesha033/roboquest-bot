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

#define in1 2                                                                                                                          
#define in2 4
#define in3 7
#define in4 8
#define enA 6
#define enB 5

#define ADVANCE_MS  150  // TUNE creep forward at the junction so the wheels sit over it
#define MIN_TURN_MS 100  // TUNE ignore the sensors briefly when a pivot starts
#define MAX_TURN_MS 1000 // give up so the bot can never spin forever

int phase = 0;          // 0 = start, 1 = looking for left branch, 2 = right branches (going out), 3 = right branches (coming back), 4 = done
int rightBranches = 0;  // right branches passed on the way out
int leftBranches  = 0;  // branches handled on the way back

#define PUSH_MS      600   // TUNE how long to drive forward to push a ring in
#define MIN_BACK_MS  200   // TUNE ignore sensors briefly when reversing
#define MAX_BACK_MS  1500  // give up so it never reverses forever


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
      pinMode(A2, INPUT);  // left junction sensor
      pinMode(A3, INPUT);  // right junction sensor

}

void loop() {

  int LEFT_SENSOR = digitalRead(A0);
  int RIGHT_SENSOR = digitalRead(A1);
  int LEFT_JUNC  = digitalRead(A2);
  int RIGHT_JUNC = digitalRead(A3);

  if (phase == 1 && LEFT_JUNC == 1) {
  forward(); delay(ADVANCE_MS);   // creep so the wheels sit over the branch point
  turnLeftToLine();
  phase = 2;
  return;
  }

    // ---- Going out: right branches ----
  if (phase == 2 && RIGHT_JUNC == 1) {
    rightBranches++;

    if (rightBranches < 3) {          // branch 1 and 2: just skip past
      skipBranch(A3);
      return;
    }
    else if (rightBranches == 3){
    // branch 3: push it first
    forward(); delay(ADVANCE_MS);
    turnRightToLine();                // face into the branch
    pushRing();
    turnRightToLine();                // turn to face back along the main line
    skipBranch(A2);                   // clear branch 3, which is now on our left
    phase = 3;
    return;
    }
    else{
      return;
    }
  }

  // ---- Coming back: branches are now on the LEFT ----
  if (phase == 3 && LEFT_JUNC == 1) {
    leftBranches++;                   // 1 = branch 2, 2 = branch 1

    forward(); delay(ADVANCE_MS);
    turnLeftToLine();                 // face into the branch
    pushRing();
    turnRightToLine();                // turn back to face along the main line

    if (leftBranches == 2) {
      phase = 4;                      // all rings pushed, head home
    } else {
      skipBranch(A2);
    }
    return;
  }

  if(RIGHT_SENSOR==1 && LEFT_SENSOR==1) {
    Stop();  //junction STOP
    count = count + 1;
    if(count == 1){
      turnRightToLine();
      delay(500);
      if (count == 1) phase = 1;   // now start watching for the left branch
      return;
    }
    else{
      forward(); delay(100); return;
    }

 }

  else if(RIGHT_SENSOR==0 && LEFT_SENSOR==0) {
    forward(); //FORWARD
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


// Drive straight until the given junction sensor stops seeing the branch
void skipBranch(int pin)
{
  unsigned long t0 = millis();
  forward();
  while (digitalRead(pin) == 1 && millis() - t0 < 400) {}
  delay(60);   // TUNE small extra so the sensor is fully clear
}

// Bot is centred on a branch: push forward, then reverse back to the main line
void pushRing()
{
  forward();
  delay(PUSH_MS);
  Stop();

  backward();
  unsigned long t0 = millis();
  while (millis() - t0 < MIN_BACK_MS) {}
  // reverse until both sensors see the main line crossing
  while (!(digitalRead(A0) == 1 && digitalRead(A1) == 1) && millis() - t0 < MAX_BACK_MS) {}
  Stop();
}


void backward()
{
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);

                analogWrite(enA, M1_Speed);
                analogWrite(enB, M2_Speed);
}

void forward()
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
            delay(3200);
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
