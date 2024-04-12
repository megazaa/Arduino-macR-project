// Pin definitions
int pinA = 2;
int pinB = 3;
int pinX = 5;
int pinY = 6;
int pinZ = 8;
int pinD = 9;

// State variables
bool LOOP = true;
int movingFlag = 1;
int direction = 1;
int pulseInitX = 0, pulseInitY = 0;
int pulseLimitX = 4, pulseLimitY = 2;
int STATE = 0;
int Xcout = 0;
int Ycout = 0;

void setup() {
  // Setup pin modes
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinZ, INPUT_PULLUP);
  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinD, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read button states
  if (LOOP) {
    int buttonStateX = digitalRead(pinA);
    int buttonStateY = digitalRead(pinB);
    int buttonStateDIR = digitalRead(pinZ);

    // Execute functions
    DirButton(buttonStateDIR, buttonStateX, buttonStateY);
    xPulse(buttonStateX);
    yPulse(buttonStateY);
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
  }
}

void DirButton(int buttonState, int XState, int YState) {
  // Check direction
  if (pulseInitY > 0) {
    directionCHECK(pulseInitY);
  }

  // Check limits
  if (pulseInitY >= pulseLimitY && pulseInitX >= pulseLimitX) {
    digitalWrite(pinY, LOW);
    digitalWrite(pinX, LOW);
    Serial.println("Finish ");
    return;
  }

  // Check button and state
  if (buttonState == LOW && STATE == 0) {
    if (pulseInitX == 0 and pulseInitY % 2 == 0 || (pulseInitY % 2 == 1 && (pulseInitX >= pulseLimitX))) {  //start End
      MovingMov(XState, YState, pinX);
    } else if (((pulseInitX >= pulseLimitX) || (pulseInitX == 0)) && (movingFlag == 1)) {
      MovingMov(XState, YState, pinY);
      digitalWrite(pinX, LOW);
      movingFlag = 1;
    } else {
      MovingMov(XState, YState, pinX);
    }
    Serial.println("moving");
  }
  delay(500);
}

void xPulse(int buttonState) {
  if ((buttonState == HIGH) && (STATE == 1) && (Xcout >= Ycout)) {
    digitalWrite(pinX, LOW);
    directionCHECK(pulseInitY);
    pulseInitX = pulseInitX + direction;
    STATE = 0;
  }
  Ycout = 0;
  Xcout = 0;
}

void yPulse(int buttonState) {
  if ((buttonState == HIGH) && (STATE == 1) && (Ycout >= Xcout)) {
    pulseInitY++;
    digitalWrite(pinY, LOW);
    STATE = 0;
  }
  Ycout = 0;
  Xcout = 0;
}

int directionCHECK(int numb) {
  if (numb % 2 == 0) {
    direction = 1;
    digitalWrite(pinD, LOW);
  } else {
    direction = -1;
    digitalWrite(pinD, HIGH);
  }
  return direction;
}

void MovingMov(int buttonStateX, int buttonStateY, int pinSOMETING) {
  STATE = 1;
  digitalWrite(pinSOMETING, HIGH);
  delay(500);
  buttonStateX = digitalRead(pinA);
  buttonStateY = digitalRead(pinB);

  if (buttonStateX == LOW && buttonStateY == LOW) {
    while (buttonStateX == LOW || buttonStateY == LOW) {
      digitalWrite(pinSOMETING, HIGH);
      buttonStateX = digitalRead(pinA);
      buttonStateY = digitalRead(pinB);
      if (buttonStateX == HIGH || buttonStateY == HIGH) {
        Serial.println("BREAKK");
        break;
      }
    }
  } else if (buttonStateX == LOW) {
    while (buttonStateX == LOW) {
      digitalWrite(pinSOMETING, HIGH);
      buttonStateX = digitalRead(pinA);
      buttonStateY = digitalRead(pinB);
      Xcout++;
      if (buttonStateX == HIGH) {
        Serial.println("BREAKK");
        break;
      }
    }
  } else if (buttonStateY == LOW) {
    while (buttonStateY == LOW) {
      digitalWrite(pinSOMETING, HIGH);
      buttonStateX = digitalRead(pinA);
      buttonStateY = digitalRead(pinB);
      Ycout++;
      if (buttonStateY == HIGH) {
        Serial.println("BREAKK");
        movingFlag = 0;
        break;
      }
    }
  } else {
    Serial.println("Error: not on rail ");
    digitalWrite(pinX, LOW);
    digitalWrite(pinY, LOW);
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    STATE = 0;
  }
}
