#include <Servo.h>

Servo clawUpDown;   // Servo 2 - D9
Servo clawOpenClose; // Servo 1 - D10

void setup() {

  // Attach servos
  clawUpDown.attach(9);       // Servo 2 - 180 degree
  clawOpenClose.attach(10);   // Servo 1 - 360 degree

  // Put both servos in starting positions
  clawUpDown.write(170);       // Middle position
  clawOpenClose.write(110);    // Stop 360° servo

  delay(1000);
}

void loop() {

  // =================================
  // 1. MOVE CLAW DOWN
  // =================================

  clawUpDown.write(120);
  delay(1000);

  // Stop at position
  clawUpDown.write(120);
  delay(1000);



  // =================================
  // 2. MOVE CLAW UP
  // =================================

  clawUpDown.write(10);
  delay(1000);

  // Stop at position
  clawUpDown.write(10);
  delay(1000);


  // =================================
  // 3. OPEN CLAW
  // =================================

  // clawOpenClose.write(80);   // Rotate one direction
  // delay(500);

  // clawOpenClose.write(80);    // STOP
  // delay(1000);


  // =================================
  // 4. CLOSE CLAW
  // =================================

  // clawOpenClose.write(60);    // Rotate opposite direction
  // delay(500);

  // clawOpenClose.write(60);    // STOP
  // delay(2000);
}