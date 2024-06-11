#include <AccelStepper.h>
#include <MultiStepper.h>

#define pul1 2
#define dir1 3

#define pul2 4
#define dir2 5

#define pul3 6
#define dir3 7

bool cmd_Mode = false;
String cmd_input = "";
String result = "";
String cmd_result = "";
String prev_str = "";
bool started = false, ended = false;
int movX = 0, movY = 0, movZ = 0;
bool done = false;
int timeout = 10;

unsigned long period = 500;
unsigned long last_time = 0;

AccelStepper stepper1(1, pul1, dir1);
AccelStepper stepper2(1, pul2, dir2);
AccelStepper stepper3(1, pul3, dir3);

void setup() {
  stepper1.setMaxSpeed(2000);      // Increase max speed
  stepper1.setAcceleration(1000);  // Increase acceleration
  stepper2.setMaxSpeed(2000);      // Increase max speed
  stepper2.setAcceleration(1000);  // Increase acceleration
  stepper3.setMaxSpeed(2000);      // Increase max speed
  stepper3.setAcceleration(1000);  // Increase acceleration
  Serial.begin(115200);
}

void loop() {
  command();
  if ((millis() - last_time > period) ) {
    last_time = millis();
    String str = "send: {" + String(stepper1.currentPosition()) + "," + String(stepper2.currentPosition()) + "," + String(stepper3.currentPosition()) + "}";
    if (prev_str == str ) {
      timeout--;
      Serial.println(timeout);
      if (timeout == 0) {
        period = 1000;
      }
    } else {
      timeout = 10;
      period = 500;
    }
    int len = str.length() + 1;
    char charArr[len];
    str.toCharArray(charArr, len);
    for (int i = 0; i < len - 1; i++) {
      Serial.print(charArr[i]);
    }
    Serial.println();
    prev_str = str;
  }


  if (result == cmd_result) {
    stepper1.moveTo(movX);
    stepper2.moveTo(movY);
    stepper3.moveTo(movZ);
  }
  stepper1.run();
  stepper2.run();
  stepper3.run();
}

void command() {
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      char incomingByte = Serial.read();
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
    }

    if (ended == true) {
      result = cmd_input;
      Serial.println(result);
      sscanf(cmd_input.c_str(), "(%d,%d,%d)", &movX, &movY, &movZ);
      //Serial.println(cmd_input);
      Serial.print("Received: ");
      Serial.print("x: ");
      Serial.print(movX);
      Serial.print(", y: ");
      Serial.print(movX);
      Serial.print(", z: ");
      Serial.println(movZ);

      started = false;
      ended = false;
      cmd_result = cmd_input;
      cmd_input = "";
    }
  }
}
