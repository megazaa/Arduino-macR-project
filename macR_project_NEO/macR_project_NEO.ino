#include <AccelStepper.h>
#include <MultiStepper.h>

#define pul1 5
#define pul2 4
#define dir1 3
#define dir2 2

bool finished = false;
bool cmd_Mode = false;
String cmd_input = "";
String norm_input = "";
String result = "";
String cmd_result ="";
bool started, ended;
int movX, movY;

class Value {
private:
  int stepper;
  int moveto;
public:
  void set_stepper(int s) {
    stepper = s;
  }
  void get_stepper() {
    return stepper;
  }
  void set_moveto(int s) {
    moveto = s;
  }
  void get_moveto() {
    return moveto;
  }
};


AccelStepper stepper1(1, pul1, dir1);
AccelStepper stepper2(1, pul2, dir2);

void setup() {
  stepper1.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
  Serial.begin(9600);
}

void loop() {
  command();
  if (result == "1") {  //ถ้าตัวแปร key เป็น 1 ให้ทำในปีกกานี้
    stepper1.setSpeed(800);
    stepper1.moveTo(800);
    stepper1.runSpeedToPosition();
  }
  if (result == "2") {  //ถ้าตัวแปร key เป็น 2 ให้ทำในปีกกานี้
    stepper2.setSpeed(800);
    stepper2.moveTo(1600);
    stepper2.runSpeedToPosition();
  }
  if (result == "reset") {  //ถ้าตัวแปร key เป็น 2 ให้ทำในปีกกานี้
    stepper1.setSpeed(800);
    stepper1.moveTo(0);
    stepper1.runSpeedToPosition();

    stepper2.setSpeed(800);
    stepper2.moveTo(0);
    stepper2.runSpeedToPosition();
  }
  if (result == cmd_result) {
    stepper1.setSpeed(800);
    stepper1.moveTo(movX);
    stepper1.runSpeedToPosition();

    stepper2.setSpeed(800);
    stepper2.moveTo(movY);
    stepper2.runSpeedToPosition();
  }
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
      norm_input += incomingByte;
      delay(10);
    }
    result = norm_input;
    norm_input = "";
    Serial.println(norm_input);

    if (ended == true) {
      int x, y;
      sscanf(cmd_input.c_str(), "(%d,%d)", &x, &y);

      Serial.println(cmd_input);
      Serial.print("Received: ");
      Serial.print("Value: ");
      Serial.print(x);
      Serial.print(", Key: ");
      Serial.println(y);
      movX = x;
      movY = y;
      started = false;
      ended = false;
      cmd_result = cmd_input;
      cmd_input = "";
    }
  }
}
