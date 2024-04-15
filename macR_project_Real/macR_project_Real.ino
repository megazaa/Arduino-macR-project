int pinA = 2;
int pinB = 3;
int pinX = 5;
int pinY = 6;
int pinR = 7;
int pinZ = 8;
int pinD = 9;

enum class STATE{
  NORMAL,
  MOVING,
  REDO,
  RESET,
  ERROR
};

STATE currentState = STATE::NORMAL;
int movingFlag = 1;
int direction = 1;
int pulseInitX = 0, pulseInitY = 0;
const int pulseLimitX = 4, pulseLimitY = 2;
int Xcout = 0;
int Ycout = 0;
int buttonStateX = digitalRead(pinA);
int buttonStateY = digitalRead(pinB);
int buttonDIR = digitalRead(pinZ);
int buttonRedo = digitalRead(pinR);
int buttonReset = digitalRead(pinR);
int i = 0;
int redoX, redoY;
bool notFinish = true;
bool error = false;
int arrX[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
int arrY[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinZ, INPUT_PULLUP);
  pinMode(pinR, INPUT_PULLUP);
  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinD, OUTPUT);
  //arrX[0] = 0;
  //arrX[0] = 0;
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  currentState = STATE::NORMAL;
  if (Execpthrow() == false) {
    if (notFinish == true) {
      Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
      Serial.println("current position(arr): "+ String(arrX[i]) +" "+ String(arrY[i]));
      //Serial.println("STATE : NORMAL" );
      stateCheck();
      DirButton();
      redoButton();
      Serial.println("i" + String(i));
      //Serial.println("runnning ");
      delay(500);
    }
  }
}

void DirButton() {
  buttonDIR = digitalRead(pinZ);
  if (pulseInitY > 0 && currentState != STATE::REDO) {
    directionCHECK(pulseInitY);
  }
  if (pulseInitY >= pulseLimitY && pulseInitX >= pulseLimitX) {
    digitalWrite(pinY, LOW);
    digitalWrite(pinX, LOW);
    Serial.println("Finish ");
    //redo();
    notFinish = false;
  }


  if (buttonDIR == LOW && currentState == STATE::NORMAL) {
    if (pulseInitX == 0 and pulseInitY % 2 == 0 || (pulseInitY % 2 == 1 && (pulseInitX >= pulseLimitX))) {  //start End
      MovingMov(pinX);
    } else if (((pulseInitX >= pulseLimitX) || (pulseInitX == 0)) && (movingFlag == 1)) {
      MovingMov(pinY);
      digitalWrite(pinX, LOW);
      movingFlag = 1;
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
  if (currentState == STATE::REDO) {
    direction = -1;
    digitalWrite(pinD, HIGH);
    return;
  }
  if (numb % 2 == 0) {
    direction = 1;
    digitalWrite(pinD, LOW);
  } else {
    direction = -1;
    digitalWrite(pinD, HIGH);
  }
  if ((pulseInitX == 0)) {
    direction = 1;
    digitalWrite(pinD, LOW);
  }
  //return direction;
}

void MovingMov(int pinSOMETING) {
  if (currentState == STATE::NORMAL) {
    currentState = STATE::MOVING ;
    digitalWrite(pinSOMETING, HIGH);
    delay(500);
    buttonStateX = digitalRead(pinA);
    buttonStateY = digitalRead(pinB);
    if (buttonStateX == LOW && buttonStateY == LOW) {
      while (buttonStateX == LOW || buttonStateY == LOW) {
        Serial.println("buttonStateX == LOW || buttonStateY == LOW");
        digitalWrite(pinSOMETING, HIGH);
        buttonStateX = digitalRead(pinA);
        buttonStateY = digitalRead(pinB);
        if (buttonStateX == HIGH || buttonStateY == HIGH) {
          Serial.println("BREAKK");
          i++;
          xPulse(buttonStateX);
          yPulse(buttonStateY);
          
          break;
        }
      }
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
          movingFlag = 0;
          
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
  if (currentState == STATE::REDO) {
    digitalWrite(pinSOMETING, HIGH);
    delay(500);
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
          i--;
          xPulse(buttonStateX);
          yPulse(buttonStateY);          
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
          movingFlag = 0;          
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
  if (currentState == STATE::RESET){
    
  }
}
void redoButton() {
buttonRedo = digitalRead(pinR);
  if (buttonRedo == LOW && currentState == STATE::NORMAL && pulseInitX != 0 ) {
    currentState = STATE::REDO;
    directionCHECK(pulseInitY);
    redoX = arrX[i] - arrX[i - 1];
    Serial.println("x current: "+ String(arrX[i]) +"x before: "+ String(arrX[i - 1]));
    redoY = arrY[i] - arrY[i - 1];
    Serial.println("y current: "+ String(arrY[i]) +"y before: "+ String(arrY[i - 1]));
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
      while(i>=0){
        Serial.print("(");
        Serial.print(String(arrX[i]));
        Serial.print(",");
        Serial.print(String(arrY[i]));
        Serial.print(")");
      }
    }
  }else{
    //Serial.println("else for redoButton");
  }
}
bool Execpthrow() {
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
        case  STATE::NORMAL:
            Serial.println("STATE : NORMAL");
            break;
        case  STATE::MOVING:
            Serial.println("STATE : MOVING");
            break;
        case  STATE::REDO:
            Serial.println("STATE : REDO");
            break;
        case  STATE::RESET:
            Serial.println("STATE : RESET");
            break;
        case  STATE::ERROR:
            Serial.println("STATE : ERROR");
            break;
    }
}
