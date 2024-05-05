//input
const int pinA = 12;
const int pinB = 11;
const int pinC = 10;
const int pinDir = 9;
const int pinRD = 8;
const int pinRS = 7;

//output
const int pinX = 6;
const int pinY = 4;
const int pinZ = 2;
const int pinD = 5;
const int pinREDE = 3;

//pivot&limiter
const int pulseLimitX = 6, pulseLimitY = 4;
int pulseInitX = 0, pulseInitY = 0;

//state
enum class STATE {
  NORMAL,
  MOVING,
  REDO,
  RESET,
  ERROR,
  z_MOV,
};

//init state
STATE currentState = STATE::NORMAL;

//init input PIN
int buttonStateX = digitalRead(pinA);
int buttonStateY = digitalRead(pinB);
int buttonLimitZ = digitalRead(pinC);
int buttonDIR = digitalRead(pinDir);
int buttonRedo = digitalRead(pinRD);
int buttonReset = digitalRead(pinRS);

//declare values for function
int direction = 1;
int movingFlag = pulseLimitY;
int Xcout = 0;
int Ycout = 0;
int i = 0;
int try_LIMIT = 3;
int try_attempt;
int z_STATE = 0;
int z_check = 0;
int redoX, redoY, left_stepsX, left_stepsY;
bool notFinish = true;
bool delaystart = false;
bool error = false;
bool snake_Action = false;
int arrX[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
int arrY[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};

//setting up program
void setup() {
  //input
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinC, INPUT_PULLUP);
  pinMode(pinDir, INPUT_PULLUP);
  pinMode(pinRD, INPUT_PULLUP);
  pinMode(pinRS, INPUT_PULLUP);

  //output
  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinZ, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinREDE, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication
  delaytimer();
}

//program run loop
void loop() {
  if (Execpthrow() == false) {
    DirButton();
    redoButton();
    returnZero();
    //delay(500); //delay off
  }
}
bool delaytimer() {
  for (int time = 10; time >= 0; time--) {
    delay(100);
    Serial.println(time);
  }
  delaystart = true;
  Serial.println("ready");
}

//moving function
void DirButton() {
  buttonDIR = digitalRead(pinDir);
  if (buttonDIR == LOW && ((pulseInitY == 0) && (pulseInitX == 0)) && (z_check == 0 && z_STATE != -1)) {
    Serial.println("CON 1");
    //check if Z axis is't already go down
    Serial.println("CON 1");
    Serial.println("z_check = 0 ");
    //directionCHECK();
    buttonLimitZ = digitalRead(pinC);
    MovingMov(pinZ);  //if not down yet go down state =1 //move Z axis down
    return;

  } else if (buttonDIR == LOW && (pulseInitY >= pulseLimitY) && (pulseInitX >= pulseLimitX) && (z_check == 0 && z_STATE == -1)) {
    Serial.println("CON 2");
    if (z_check == 0 && z_STATE == -1) {  //
      Serial.println("z_check = 1 ");
      //directionCHECK();
      buttonLimitZ = digitalRead(pinC);
      MovingMov(pinZ);  // if it down go up
      z_STATE = 0;
      z_check = 1;
      notFinish = false;
      returnZero();
      return;
    }
  } else if (buttonDIR == LOW) {
    Serial.println("CON 3 " + String(snake_Action));
    if (z_check == 0 && z_STATE == -1) {  //
      Serial.println("z_check = 1 ");
      buttonLimitZ = digitalRead(pinC);
      MovingMov(pinZ);  // if it down go up
      z_STATE = 0;
      z_check = 1;
      currentState = STATE::MOVING;
      snake_Action = true;
    }
    if (snake_Action == true) {
      currentState = STATE::MOVING;
      directionCHECK();
      if ((pulseInitX == 0 && pulseInitY == 0) || (pulseInitY % 2 == 1 && (pulseInitX >= pulseLimitX)) || (pulseInitX == 0 && pulseInitY % 2 == 0)) {  //start End
        MovingMov(pinX);
        Serial.println("moving flag = " + String(movingFlag));
      } else if (((pulseInitX >= pulseLimitX) || (pulseInitX == 0)) && (movingFlag > 0)) {
        //directionCHECK();
        MovingMov(pinY);
        digitalWrite(pinX, LOW);
        //movingFlag --;
        Serial.println("1 movingFlag =" + String(movingFlag));
      } else {
        MovingMov(pinX);
        Serial.println(movingFlag);
        Serial.println(" yyyyyyyyoooooooooooo movwe  this huh");
      }
      if (z_check == 0 && z_STATE != -1 && snake_Action == false) {  //check if Z axis is't already go down
        Serial.println("z_check = 0 ");
        directionCHECK();
        buttonLimitZ = digitalRead(pinC);
        MovingMov(pinZ);  //if not down yet go down state =1                                  //move Z axis down
        Serial.println("moving flag = " + String(movingFlag));
        return;
      }
      Serial.println("moving flag = " + String(movingFlag));
    } else {
      currentState = STATE::NORMAL;
    }
  }
}

void PulseCheck(int buttonStateX, int buttonStateY) {
  xPulse(buttonStateX);
  yPulse(buttonStateY);
  Ycout = 0;
  Xcout = 0;
}

void xPulse(int buttonState) {
  if ((buttonState == HIGH) && (Xcout >= Ycout)) {
    if (currentState != STATE::RESET) {
      digitalWrite(pinX, LOW);
    }
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
    if (currentState != STATE::RESET) {
      digitalWrite(pinX, LOW);
    }
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
      digitalWrite(pinREDE, LOW);
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      break;
      //return;
    }
    if (buttonLimitZ == LOW && z_STATE == -1) {
      digitalWrite(pinZ, LOW);
      z_STATE = 0;
      Serial.println("z_STATE = 0");
      digitalWrite(pinREDE, LOW);
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      break;
      //return;
    }
    Serial.println("jumppp c");
  }
}

void directionCHECK() {
  if (z_check == 0) {  // z checking
    if (z_STATE == -1) {
      digitalWrite(pinD, LOW);
      direction = -1;
      return;
    }
    if (z_STATE == 0) {
      digitalWrite(pinD, HIGH);
      direction = 1;
      return;
    }
  } else {
    if (pulseInitX == 0 && pulseInitY == 0) {  // At (0,0) normally state the + direction (X AXIZ)
      Serial.println("pulseInitX == 0 && pulseInitY == 0");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (redoX < 0 && currentState == STATE::REDO) {  // REDO STATE : working when it at (0,0) (X AXIZ)
      Serial.println("redoX < 0 && currentState == STATE::REDO");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (currentState == STATE::REDO || currentState == STATE::RESET) {  // REDO STATE or RESET STATE make
      Serial.println("currentState == STATE::REDO || currentState == STATE::RESET");
      direction = -1;
      digitalWrite(pinD, HIGH);
      return;
    }
    if (pulseInitX == 0) {  //(X AXIZ) when it lefty ex (0,1),(0,2)...  the direction should go +
      Serial.println("pulseInitX == 0");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (pulseInitY % 2 == 0 && currentState == STATE::MOVING) {  //(Y AXIZ)
      Serial.println("pulseInitY % 2 == 0 && currentState == STATE::MOVING");
      direction = 1;
      digitalWrite(pinD, LOW);
      return;
    }
    if (pulseInitY % 2 == 1 && currentState == STATE::MOVING) {  //(Y AXIZ)
      Serial.println("pulseInitY % 2 == 1 && currentState == STATE::MOVING");
      direction = -1;
      digitalWrite(pinD, HIGH);
      return;
    }
  }



  //return direction;
}

void MovingMov(int pinSOMETING) {
  if ((z_STATE == 0 || z_STATE == -1) && z_check == 0 && currentState != STATE::MOVING && currentState != STATE::REDO && currentState != STATE::RESET) {
    directionCHECK();
    //buttonDIR = digitalRead(pinDir);
    buttonLimitZ = digitalRead(pinC);
    if (buttonLimitZ == LOW) {
      while (buttonLimitZ == LOW) {
        digitalWrite(pinSOMETING, HIGH);
        Serial.println("buttonDIR == LOW");
        buttonLimitZ = digitalRead(pinC);
        if (buttonLimitZ == HIGH) {
          delay(500);  // add delay for Z
          digitalWrite(pinREDE, HIGH);
          break;
        }
      }
    } else {
      digitalWrite(pinSOMETING, HIGH);
    }
    Serial.println("MovingMov = Z ");
    zPulse();
    return;
  }
  if (currentState == STATE::MOVING) {
    digitalWrite(pinSOMETING, HIGH);
    delay(500);
    digitalWrite(pinREDE, HIGH);
    delay(200);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    if (buttonStateX == LOW && buttonStateY == LOW) {
      Serial.println("buttonStateX == LOW || buttonStateY == LOW");
    } else if (buttonStateX == LOW) {
      while (buttonStateX == LOW) {
        Serial.println("XTime = " + String(Xcout));
        digitalWrite(pinSOMETING, HIGH);
        buttonStateX = digitalRead(pinA);
        //buttonStateY = digitalRead(pinB);
        Xcout++;
        if (buttonStateX == HIGH) {
          Serial.println("BREAKK");
          digitalWrite(pinREDE, LOW);
          i++;
          PulseCheck(buttonStateX, buttonStateY);
          snake_Action = false;
          currentState = STATE::NORMAL;
          break;
        }
      }
    } else if (buttonStateY == LOW) {
      while (buttonStateY == LOW) {
        Serial.println("YTime = " + String(Ycout));
        digitalWrite(pinSOMETING, HIGH);
        //buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Ycout++;
        if (buttonStateY == HIGH) {
          Serial.println("BREAKK");
          digitalWrite(pinREDE, LOW);
          i++;
          PulseCheck(buttonStateX, buttonStateY);
          movingFlag--;
          currentState = STATE::NORMAL;
          snake_Action = false;
          break;
        }
      }
    } else {
      Serial.println("Error: not on rail 0");
      digitalWrite(pinX, LOW);
      digitalWrite(pinY, LOW);
      digitalWrite(pinREDE, LOW);
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      currentState = STATE::NORMAL;
    }
  }
  if (currentState == STATE::REDO || currentState == STATE::RESET) {
    digitalWrite(pinSOMETING, HIGH);
    delay(500);
    digitalWrite(pinREDE, HIGH);
    delay(200);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    if (buttonStateX == LOW && buttonStateY == LOW) {
      Serial.println("HOW you get here");
    } else if (buttonStateX == LOW) {
      while (buttonStateX == LOW) {
        Serial.println("XTime = " + String(Xcout));
        digitalWrite(pinSOMETING, HIGH);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Xcout++;
        if (buttonStateX == HIGH) {
          Serial.println("BREAKK");
          //digitalWrite(pinREDE, LOW);
          i--;
          PulseCheck(buttonStateX, buttonStateY);
          if (pulseInitX == 0) {
            Serial.println("X end");
            digitalWrite(pinX, LOW);
            digitalWrite(pinREDE, LOW);
          }
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
        digitalWrite(pinSOMETING, HIGH);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Ycout++;
        if (buttonStateY == HIGH) {
          Serial.println("BREAKK");
          //digitalWrite(pinREDE, LOW);
          i--;
          movingFlag++;
          PulseCheck(buttonStateX, buttonStateY);
          if (pulseInitY == 0) {
            Serial.println("X end");
            digitalWrite(pinX, LOW);
            digitalWrite(pinY, LOW);
            digitalWrite(pinREDE, LOW);
          }
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
      digitalWrite(pinY, LOW);
      digitalWrite(pinREDE, LOW);
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
  stateCheck();
}

void returnZero() {

  buttonReset = digitalRead(pinRS);
  if (buttonReset == LOW || notFinish == false) {
    try_attempt = try_LIMIT;
    Serial.println("returnZero is working");
    if (z_check == 0 && z_STATE == -1) {  //
      Serial.println("z_check = 1 ");
      //directionCHECK();
      buttonLimitZ = digitalRead(pinC);
      MovingMov(pinZ);  // if it down go up
      z_STATE = 0;
      z_check = 1;
    }
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
      digitalWrite(pinREDE, LOW);
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
    /*
      if(snake == false){

      }*/

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
bool Execpthrow() { /*
  //init Checking thing up
  if (pulseInitX == 0 && pulseInitY == 0) {
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    Serial.println("not in ready position");
    if (buttonStateX == LOW || buttonStateY == LOW) {
      return true;
    }
  }*/
  if ((pulseInitX == 0 && pulseInitY == 0) && z_check == 1) {
    //Serial.println("pulseInitX == 0");
    direction = 1;
    digitalWrite(pinD, LOW);
  }
  if (pulseInitX > pulseLimitX || pulseInitY > pulseLimitY) {
    cut_out();
    Serial.println("Error: Out of rail");
    delay(1500);
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
  /*
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
    }*/

  Serial.println(z_check);
  Serial.println(z_STATE);
  Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
  Serial.println("current position(arr): " + String(arrX[i]) + " " + String(arrY[i]));
  Serial.println("STATE : NORMAL");
  //stateCheck();
  //Serial.println("i" + String(i));
  //Serial.println("movingFlag" + String(movingFlag));
}
void cut_out() {
  digitalWrite(pinX, LOW);
  digitalWrite(pinY, LOW);
  digitalWrite(pinZ, LOW);
  digitalWrite(pinD, LOW);
}
