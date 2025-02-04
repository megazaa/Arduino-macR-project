#include <AccelStepper.h>

const int pinA = 2;
const int pinB = 3;
const int pinC = 13;
const int pinRS = 4;
const int pinX = 5;
const int pinY = 6;
const int pinZ = 12;
const int pinRD = 7;
const int pinDir = 8;
const int pinD = 9;

const int pulseLimitX = 1, pulseLimitY = 1;
int pulseInitX = 0, pulseInitY = 0;

enum class STATE {
  NORMAL,
  MOVING,
  REDO,
  RESET,
  ERROR,
  z_MOV,
};

STATE currentState = STATE::NORMAL;

int direction = 1;

int movingFlag = pulseLimitY;
int Xcout = 0;
int Ycout = 0;
int buttonStateX = digitalRead(pinA);
int buttonStateY = digitalRead(pinB);
int buttonLimitZ = digitalRead(pinC);
int buttonDIR = digitalRead(pinDir);
int buttonRedo = digitalRead(pinRD);
int buttonReset = digitalRead(pinRS);
int i = 0;
int try_LIMIT = 3;
int try_attempt;
int z_STATE = 0;
int z_check = 0;
int redoX, redoY, left_stepsX, left_stepsY;
bool notFinish = true;
bool error = false;
int arrX[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
int arrY[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};

AccelStepper stepperX(1, pinX, pinD);
AccelStepper stepperY(1, pinY, pinD);

void setup() {
  //z_check = 1;
  stepperX.setMaxSpeed(2000);
  stepperX.setSpeed(2000);
  stepperY.setMaxSpeed(2000);
  stepperY.setSpeed(2000);
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  pinMode(pinDir, INPUT_PULLUP);
  pinMode(pinRD, INPUT_PULLUP);
  pinMode(pinRS, INPUT_PULLUP);
  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinZ, OUTPUT);
  pinMode(pinD, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  //currentState = STATE::NORMAL;
  if (Execpthrow() == false) {
    Serial.println(z_check);
    Serial.println(z_STATE);
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    Serial.println("current position(arr): " + String(arrX[i]) + " " + String(arrY[i]));
    //Serial.println("STATE : NORMAL" );
    //stepperX.runSpeed();
    //stepperX.runSpeed();
    //stepperY.runSpeed();
    
    DirButton();
    stateCheck();
    redoButton();
    //stateCheck();
    returnZero();
    //stateCheck();
    //Serial.println("i" + String(i));
    //Serial.println("movingFlag" + String(movingFlag));
    delay(500);
  }
}

void DirButton() {
  buttonDIR = digitalRead(pinDir);
  if (buttonDIR == LOW && (pulseInitY >= pulseLimitY && pulseInitX >= pulseLimitX)) {
    if (z_check == 0 && z_STATE != -1) {
      Serial.println("z_check = 0 ");
      z_STATE = 0;
      MovingMov(pinZ);  //if not down yet go down state =1
      return;
    }
    if (z_check == 0 && z_STATE == -1) {
      while(true){}
      stepperY.move(640000);
      stepperY.run();
      Serial.println("z_check = 1 ");
      MovingMov(pinZ);  // if it down go up
      z_STATE = 0;
      z_check = 1;
      return;
    }
    if (z_check = 1) {
      Serial.println("Finish ");
      notFinish = false;
      returnZero();
      return;
    }
  }
  if (buttonDIR == LOW && z_check == 0 && z_STATE != -1) {
    //currentState = STATE::z_MOV;
    Serial.println("z_check = 0 ");
    z_STATE = 0;
    MovingMov(pinZ);  //if not down yet go down state =1
    return;
  }
  if (buttonDIR == LOW && z_check == 0 && z_STATE == -1) {
    Serial.println("z_check = 1 ");
    MovingMov(pinZ);  // if it down go up
    z_STATE = 0;
    z_check = 1;
    currentState = STATE::NORMAL;
    return;
  }
  if (buttonDIR == LOW && z_check == 1) {
    currentState = STATE::MOVING;
    directionCHECK();
    /*
        if (pulseInitY > 0) {
        Serial.println("pulseInitY > 0");
        directionCHECK(pulseInitY);
        }*/
    if ((pulseInitX == 0 && pulseInitY == 0) || (pulseInitY % 2 == 1 && (pulseInitX >= pulseLimitX))) {  //start End
      MovingMov(pinX);
    } else if (((pulseInitX >= pulseLimitX) || (pulseInitX == 0)) && (movingFlag > 0)) {
      MovingMov(pinY);
      digitalWrite(pinX, LOW);
      stepperX.stop();
      //movingFlag --;
      Serial.println("1 movingFlag =" + String(movingFlag));

    } else {
      MovingMov(pinX);
      Serial.println(movingFlag);
      Serial.println(" yyyyyyyyoooooooooooo movwe  this huh");
    }
    Serial.println("moving");
  } else {
    currentState = STATE::NORMAL;
  }


  //delay(500);
}
void PulseCheck(int buttonStateX, int buttonStateY) {
  xPulse(buttonStateX);
  yPulse(buttonStateY);
  Ycout = 0;
  Xcout = 0;
}

void xPulse(int buttonState) {
  if ((buttonState == HIGH) && (Xcout >= Ycout)) {
    digitalWrite(pinX, LOW);
    stepperX.stop();
    directionCHECK();
    pulseInitX = pulseInitX + direction;
    Serial.println("X Pulseisworking");
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    //currentState = STATE::NORMAL;
    arrX[i] = pulseInitX;
    arrY[i] = pulseInitY;
    if (currentState == STATE::MOVING) {
      z_check = 0;
    }
  }
}
void yPulse(int buttonState) {
  if ((buttonState == HIGH) && (Ycout >= Xcout)) {
    directionCHECK();
    pulseInitY = pulseInitY + direction;
    digitalWrite(pinY, LOW);
    Serial.println("Y Pulseisworking");
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    //currentState = STATE::NORMAL;
    arrX[i] = pulseInitX;
    arrY[i] = pulseInitY;
    if (currentState == STATE::MOVING) {
      z_check = 0;
    }
  }
}
void zPulse() {
  Serial.println("iam checking pin c");
  buttonLimitZ = digitalRead(pinC);
  while (buttonLimitZ == HIGH) {
    buttonLimitZ = digitalRead(pinC);
    Serial.print(".");
    if (buttonLimitZ == LOW && z_STATE == 0) {
      digitalWrite(pinZ, LOW);
      z_STATE = -1;
      Serial.println("z_STATE = -1");
      break;
      //return;
    }
    if (buttonLimitZ == LOW && z_STATE == -1) {
      digitalWrite(pinZ, LOW);
      z_STATE = 0;
      Serial.println("z_STATE = 0");
      break;
      //return;
    }
  }
}

void directionCHECK() {
  if (z_check == 0) {
    if (z_STATE == 0) {
      digitalWrite(pinD, HIGH);
      direction = -1;
      return;
    }
    if (z_STATE == -1) {
      digitalWrite(pinD, LOW);
      direction = 1;
      return;
    }
  } else {
    if (pulseInitX == 0 && pulseInitY == 0) {
      Serial.println("pulseInitX == 0");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (redoX < 0 && currentState == STATE::REDO) {
      Serial.println("redoX<0 && currentState == STATE::RESET");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (currentState == STATE::REDO || currentState == STATE::RESET) {
      Serial.println("other currentState == STATE::RESET");
      direction = -1;
      digitalWrite(pinD, HIGH);
      return;
    }
    if (pulseInitX == 0) {
      Serial.println("pulseInitX == 0");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (pulseInitY % 2 == 0 && currentState == STATE::MOVING) {
      Serial.println("others 1");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (pulseInitY % 2 == 1 && currentState == STATE::MOVING) {
      Serial.println("others 1 else");
      direction = -1;
      digitalWrite(pinD, HIGH);
      return;
    }
  }



  //return direction;
}
void digitalWritePinSelect(int pinSOMETING) {
  if (pinSOMETING == pinX) {
    digitalWrite(pinSOMETING, HIGH);
    stepperX.move(640000);
    stepperX.run();
    return;
  }
  if (pinSOMETING == pinY) {
    digitalWrite(pinSOMETING, HIGH);
    stepperY.move(640000);
    stepperY.run();
    return;
  }
  if (pinSOMETING == pinZ) {
    digitalWrite(pinSOMETING, HIGH);
    return;
  }
}

void MovingMov(int pinSOMETING) {
  if ((z_STATE == 0 || z_STATE == -1) && z_check == 0 && currentState != STATE::MOVING && currentState != STATE::REDO && currentState != STATE::RESET) {
    directionCHECK();
    digitalWritePinSelect(pinSOMETING);
    Serial.println("MovingMov = Z ");
    zPulse();
    return;
  }
  if (currentState == STATE::MOVING) {
    digitalWritePinSelect(pinSOMETING);
    delay(500);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    if (buttonStateX == LOW && buttonStateY == LOW) {
      Serial.println("buttonStateX == LOW || buttonStateY == LOW");
    } else if (buttonStateX == LOW) {
      while (buttonStateX == LOW) {
        Serial.println("XTime = " + String(Xcout));
        digitalWritePinSelect(pinSOMETING);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Xcout++;
        if (buttonStateX == HIGH) {
          Serial.println("BREAKK");
          i++;
          PulseCheck(buttonStateX, buttonStateY);
          currentState = STATE::NORMAL;
          break;
        }
      }
    } else if (buttonStateY == LOW) {
      while (buttonStateY == LOW) {
        Serial.println("YTime = " + String(Ycout));
        digitalWritePinSelect(pinSOMETING);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Ycout++;
        if (buttonStateY == HIGH) {
          Serial.println("BREAKK");
          i++;
          PulseCheck(buttonStateX, buttonStateY);
          movingFlag--;
          currentState = STATE::NORMAL;
          break;
        }
      }
    } else {
      Serial.println("Error: not on rail 0");
      digitalWrite(pinX, LOW);
      stepperX.stop();
      digitalWrite(pinY, LOW);
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      currentState == STATE::NORMAL;
    }
  }
  if (currentState == STATE::REDO || currentState == STATE::RESET) {
    digitalWritePinSelect(pinSOMETING);
    delay(500);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    //DEBUG(pinSOMETING);
    //DEBUG(pinSOMETING);
    if (buttonStateX == LOW && buttonStateY == LOW) {
      Serial.println("HOW you get here");
    } else if (buttonStateX == LOW) {
      while (buttonStateX == LOW) {
        Serial.println("XTime = " + String(Xcout));
        digitalWritePinSelect(pinSOMETING);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Xcout++;
        if (buttonStateX == HIGH) {
          Serial.println("BREAKK");
          i--;
          PulseCheck(buttonStateX, buttonStateY);
          if (currentState == STATE::REDO) {
            currentState = STATE::NORMAL;
            return;
          }
          if (currentState == STATE::RESET) {
            currentState = STATE::RESET;
            left_stepsX--;
            try_attempt = try_LIMIT;
            return;
          }
          //currentState = STATE::NORMAL;
          break;
        }
      }
    } else if (buttonStateY == LOW) {
      while (buttonStateY == LOW) {
        Serial.println("XTime = " + String(Ycout));
        digitalWritePinSelect(pinSOMETING);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Ycout++;
        if (buttonStateY == HIGH) {
          Serial.println("BREAKK");
          i--;
          PulseCheck(buttonStateX, buttonStateY);
          movingFlag++;
          if (currentState == STATE::REDO) {
            currentState = STATE::NORMAL;
            return;
          }
          if (currentState == STATE::RESET) {
            currentState = STATE::RESET;
            left_stepsY--;
            try_attempt = try_LIMIT;
            return;
          }
          break;
        }
      }
    } else {
      Serial.println("Error: not on rail 1");
      digitalWrite(pinX, LOW);
      stepperX.stop();
      digitalWrite(pinY, LOW);
      stepperY.stop();
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      if (currentState == STATE::REDO) {
        currentState = STATE::NORMAL;
        return;
      }
      if (currentState == STATE::RESET) {
        Serial.println("try : " + String(try_attempt));
        delay(1000);
        currentState = STATE::RESET;
        try_attempt--;
        return;
      }
    }
  }
}

void returnZero() {
  buttonReset = digitalRead(pinRS);
  if (buttonReset == LOW || notFinish == false) {
    try_attempt = try_LIMIT;
    Serial.println("returnZero is working");
    currentState = STATE::RESET;

    if (currentState == STATE::RESET) {

      left_stepsX = arrX[i];
      left_stepsY = arrY[i];
      directionCHECK();
      while (left_stepsX != 0 || left_stepsY != 0) {
        if (try_attempt <= 0) {
          Serial.println("Error: over try");
          currentState = STATE::NORMAL;
          break;
        }
        if (left_stepsX != 0) {
          MovingMov(pinX);
        } else if (left_stepsY != 0) {
          MovingMov(pinY);
        }
        Serial.println("left_stepsX = " + String(left_stepsX));
        Serial.println("left_stepsY = " + String(left_stepsY));
        Serial.println(left_stepsX != 0 || left_stepsY != 0);
        stateCheck();
      }
      notFinish = true;
      z_check = 0;
      cut_out();
      Serial.println("break returnZero");
    }
  } else {
    currentState = STATE::NORMAL;
  }
}
void redoButton() {
  buttonRedo = digitalRead(pinRD);
  if (buttonRedo == LOW && currentState == STATE::NORMAL && (pulseInitX != 0 || pulseInitY != 0)) {
    currentState = STATE::REDO;

    redoX = arrX[i] - arrX[i - 1];
    Serial.println("x current: " + String(arrX[i]) + "x before: " + String(arrX[i - 1]));
    redoY = arrY[i] - arrY[i - 1];
    Serial.println("y current: " + String(arrY[i]) + "y before: " + String(arrY[i - 1]));
    directionCHECK();
    if (redoX > 0) {
      Serial.println("doing: redoX > 0");
      MovingMov(pinX);
      z_check = 0;
    } else if (redoX < 0) {
      Serial.println("doing: redoX < 0");
      MovingMov(pinX);
      z_check = 0;
    } else if (redoY > 0) {
      Serial.println("doing: redoY > 0");
      MovingMov(pinY);
      z_check = 0;
    } else {
      Serial.println("Error: You cant");
      Serial.println();
      while (i >= 0) {
        Serial.print("(");
        Serial.print(String(arrX[i]));
        Serial.print(",");
        Serial.print(String(arrY[i]));
        Serial.print(")");
      }
      z_check = 0;
    }
  } else {
    currentState = STATE::NORMAL;
  }
}
bool Execpthrow() {
  if ((pulseInitX == 0 && pulseInitY == 0) && z_check == 1) {
    //Serial.println("pulseInitX == 0");
    direction = 1;
    digitalWrite(pinD, LOW);
  }
  if (pulseInitX > pulseLimitX || pulseInitY > pulseLimitY) {
    cut_out();
    Serial.println("Error: Out of rail");
    delay(1000);
    return true;
  } else {
    return false;
  }
}
void stateCheck() {
  //currentState = STATE::NORMAL;
  // Print the state along with the string "STATE : "
  switch (currentState) {
    case STATE::NORMAL:
      Serial.println("STATE : NORMAL");
      break;
    case STATE::MOVING:
      Serial.println("STATE : MOVING");
      break;
    case STATE::REDO:
      Serial.println("STATE : REDO");
      break;
    case STATE::RESET:
      Serial.println("STATE : RESET");
      break;
    case STATE::ERROR:
      Serial.println("STATE : ERROR");
      break;
  }
}
void DEBUG(int pinSOMETING) {
  if (pinSOMETING == pinX) {
    //delay(random(5, 10));
    digitalWrite(pinA, LOW);
    Serial.println("click X on rail");
    return;
  }
  if (pinSOMETING == pinY) {
    //delay(random(5, 10));
    digitalWrite(pinB, LOW);
    Serial.println("click Y on rail");
    return;
  }
}
void cut_out() {
  digitalWrite(pinX, LOW);
  stepperX.stop();
  digitalWrite(pinY, LOW);
  stepperY.stop();
  digitalWrite(pinZ, LOW);
  digitalWrite(pinD, LOW);
}
