#include <AccelStepper.h>
#include <MultiStepper.h>

#define pul1 5
#define pul2 4
#define dir1 3
#define dir2 2

bool cmd_Mode = false;
String cmd_input = "";
String result = "";
String cmd_result = "";
bool started = false, ended = false;
int movX = 0, movY = 0;

AccelStepper stepper1(1, pul1, dir1);
AccelStepper stepper2(1, pul2, dir2);

void setup() {
  stepper1.setMaxSpeed(2000);  // Increase max speed
  stepper1.setAcceleration(1000);  // Increase acceleration
  stepper2.setMaxSpeed(2000);  // Increase max speed
  stepper2.setAcceleration(1000);  // Increase acceleration
  Serial.begin(115200);
}

void loop() {
  command();

  if (result == "1") {  
    stepper1.moveTo(800);
    stepper2.moveTo(0);
  }
  if (result == "2") {  
    stepper2.moveTo(1600);
    stepper1.moveTo(0);
  }
  if (result == "reset") {  
    stepper1.moveTo(0);
    stepper2.moveTo(0);
  }
  if (result == cmd_result) {
    stepper1.moveTo(movX);
    stepper2.moveTo(movY);
  }

  // Ensure steppers keep running
  stepper1.run();
  stepper2.run();
}

void command() {
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      char incomingByte = Serial.read();  // Read the incoming byte as a character
      if (incomingByte == '(') {
        cmd_input = "";
        cmd_input += '(';
        cmd_Mode = true;
        started = true;
      } else if (incomingByte == ')') {
        cmd_input += ')';
        ended = true;
      } else if (started) {
        cmd_input += incomingByte;
      }
      delay(10);  // Small delay to ensure we read the full message
    }
    result = cmd_input;  // Store the complete input in result
    Serial.println(result);

    if (ended == true) {
      sscanf(cmd_input.c_str(), "(%d,%d)", &movX, &movY);

      Serial.println(cmd_input);
      Serial.print("Received: ");
      Serial.print("Value: ");
      Serial.print(movX);
      Serial.print(", Key: ");
      Serial.println(movY);

      started = false;
      ended = false;
      cmd_result = cmd_input;
      cmd_input = "";
    }
  }
}
