int pinA = 2;
int pinB = 3;
int pinRS = 4;
int pinX = 5;
int pinY = 6;
int pinRD = 7;
int pinZ = 8;
int pinD = 9;

enum class STATE {
  NORMAL,
  MOVING,
  REDO,
  RESET,
  ERROR
};

STATE currentState = STATE::NORMAL;
int direction = 1;
int pulseInitX = 0, pulseInitY = 0;
const int pulseLimitX = 4, pulseLimitY = 2;
int movingFlag = pulseLimitY;
int Xcout = 0;
int Ycout = 0;
int buttonStateX = digitalRead(pinA);
int buttonStateY = digitalRead(pinB);
int buttonDIR = digitalRead(pinZ);
int buttonRedo = digitalRead(pinRD);
int buttonReset = digitalRead(pinRS);
int i = 0;
int redoX, redoY, left_stepsX, left_stepsY;
bool notFinish = true;
bool error = false;
int arrX[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
int arrY[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinZ, INPUT_PULLUP);
  pinMode(pinRD, INPUT_PULLUP);
  pinMode(pinRS, INPUT_PULLUP);
  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinD, OUTPUT);
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  //currentState = STATE::NORMAL;
  if (Execpthrow() == false) {
    if (notFinish == true) {
      //Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      //Serial.println("current position(arr): " + String(arrX[i]) + " " + String(arrY[i]));
      //Serial.println("STATE : NORMAL" );
      
      DirButton();
      redoButton();
      returnZero();
      //Serial.println("i" + String(i));
      //Serial.println("movingFlag" + String(movingFlag));
      delay(500);
    }
  }
}

void DirButton() {
  buttonDIR = digitalRead(pinZ);
  if (pulseInitY >= pulseLimitY && pulseInitX >= pulseLimitX) {
    digitalWrite(pinY, LOW);
    digitalWrite(pinX, LOW);
    Serial.println("Finish ");
    //redo();
    notFinish = false;
  }


  if (buttonDIR == LOW) {
    currentState = STATE::NORMAL;
    directionCHECK(pulseInitY);
    if (pulseInitY > 0) {
      Serial.println("buttonDIR == LOW && currentState == STATE::NORMAL ");
      directionCHECK(pulseInitY);
    }
    if ((pulseInitX == 0 && pulseInitY == 0) || (pulseInitY % 2 == 1 && (pulseInitX >= pulseLimitX))) {  //start End
      MovingMov(pinX);
    } else if (((pulseInitX >= pulseLimitX) || (pulseInitX == 0)) && (movingFlag > 0)) {
      MovingMov(pinY);
      digitalWrite(pinX, LOW);
      //movingFlag --;
      Serial.println("1 movingFlag =" + String(movingFlag));

    } else {
      MovingMov(pinX);
      Serial.println(movingFlag);
      Serial.println(" yyyyyyyyoooooooooooo movwe  this huh");
    }
    Serial.println("moving");
  }


  //delay(500);
}

void xPulse(int buttonState) {
  if ((buttonState == HIGH) && ((currentState == STATE::MOVING) || (currentState == STATE::REDO)) && (Xcout >= Ycout)) {
    digitalWrite(pinX, LOW);
    directionCHECK(pulseInitY);
    pulseInitX = pulseInitX + direction;
    Serial.println("X Pulseisworking");
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    currentState = STATE::NORMAL;
    arrX[i] = pulseInitX;
    arrY[i] = pulseInitY;
  }
  Ycout = 0;
  Xcout = 0;
}
void yPulse(int buttonState) {
  if ((buttonState == HIGH) && ((currentState == STATE::MOVING) || (currentState == STATE::REDO)) && (Ycout >= Xcout)) {
    directionCHECK(pulseInitY);
    pulseInitY = pulseInitY + direction;
    digitalWrite(pinY, LOW);
    Serial.println("Y Pulseisworking");
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    currentState = STATE::NORMAL;
    arrX[i] = pulseInitX;
    arrY[i] = pulseInitY;
  }
  Ycout = 0;
  Xcout = 0;
}
void directionCHECK(int numb) {
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
  if (numb % 2 == 0) {
    Serial.println("others 1");
    direction = 1;
    digitalWrite(pinD, LOW);
    return;
  }
  if (numb % 2 == 1) {
    Serial.println("others 1 else");
    direction = -1;
    digitalWrite(pinD, HIGH);
    return;
  }

  //return direction;
}

void MovingMov(int pinSOMETING) {
  if (currentState == STATE::NORMAL) {
    currentState = STATE::MOVING;
    digitalWrite(pinSOMETING, HIGH);
    delay(500);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    if (buttonStateX == LOW && buttonStateY == LOW) {
      Serial.println("buttonStateX == LOW || buttonStateY == LOW");
    } else if (buttonStateX == LOW) {
      while (buttonStateX == LOW) {
        Serial.println("XTime = " + String(Xcout));
        digitalWrite(pinSOMETING, HIGH);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        Xcout++;
        if (buttonStateX == HIGH) {
          Serial.println("BREAKK");
          i++;
          xPulse(buttonStateX);
          yPulse(buttonStateY);
          currentState = STATE::NORMAL;
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
          i++;
          xPulse(buttonStateX);
          yPulse(buttonStateY);
          movingFlag--;
          currentState = STATE::NORMAL;
          break;
        }
      }
    } else {
      Serial.println("Error: not on rail ");
      digitalWrite(pinX, LOW);
      digitalWrite(pinY, LOW);
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      currentState == STATE::NORMAL;
    }
  }
  if (currentState == STATE::REDO || currentState == STATE::RESET) {
    digitalWrite(pinSOMETING, HIGH);
    delay(500);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    DEBUG(pinSOMETING);
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
          i--;
          xPulse(buttonStateX);
          yPulse(buttonStateY);
          currentState = STATE::NORMAL;
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
          i--;
          xPulse(buttonStateX);
          yPulse(buttonStateY);
          movingFlag++;
          currentState = STATE::NORMAL;
          break;
        }
      }
    } else {
      Serial.println("Error: not on rail ");
      digitalWrite(pinX, LOW);
      digitalWrite(pinY, LOW);
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      currentState = STATE::NORMAL;
    }
  }
}
void returnZero() {
  int fiveSEC = 5;
  int Ready = 1;
  buttonReset = digitalRead(pinRS);
  if (buttonReset == LOW) {
    Serial.println("returnZero is working");
    currentState = STATE::RESET;

    if (currentState == STATE::RESET && Ready == 1) {

      left_stepsX = arrX[i];
      left_stepsY = arrY[i];
      directionCHECK(pulseInitY);
      while (left_stepsX != 0 || left_stepsY != 0) {
        MovingMov(pinX);
        MovingMov(pinY);
        left_stepsX--;
        left_stepsY--;
      }
      currentState = STATE::NORMAL;
    }
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
    directionCHECK(pulseInitY);
    if (redoX > 0) {
      Serial.println("doing: redoX > 0");
      MovingMov(pinX);
    } else if (redoX < 0) {
      Serial.println("doing: redoX < 0");
      MovingMov(pinX);
    } else if (redoY > 0) {
      Serial.println("doing: redoY > 0");
      MovingMov(pinY);
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
    }
  } else {
    currentState = STATE::NORMAL;
  }
}
bool Execpthrow() {
  if (pulseInitX == 0 && pulseInitY == 0) {
    //Serial.println("pulseInitX == 0");
    direction = 1;
    digitalWrite(pinD, LOW);
  }
  if (pulseInitX > pulseLimitX || pulseInitY > pulseLimitY) {
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
    delay(random(5, 10));
    digitalWrite(pinA, LOW);
    return;
  }
  if (pinSOMETING == pinY) {
    delay(random(5, 10));
    digitalWrite(pinB, LOW);
    return;
  }
}
