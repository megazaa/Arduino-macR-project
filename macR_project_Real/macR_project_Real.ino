int pinA = 2;
int pinB = 3;
int pinX = 5;
int pinY = 6;
int pinZ = 8;
int pinD = 9;

int movingFlag = 1;
int direction = 1;
int pulseInitX = 0, pulseInitY = 0;
const int pulseLimitX = 4, pulseLimitY = 2;
int STATE = 0;
int Xcout = 0;
int Ycout = 0;
int buttonStateX = digitalRead(pinA);
int buttonStateY = digitalRead(pinB);
int buttonStateDIR = digitalRead(pinZ);
int i = 0;
int redoX,redoX;
bool notFinish = true;
bool error = false;
int arrX[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
int arrY[(pulseLimitX + 1) * (pulseLimitY + 1)] = {};
void setup() {
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(pinZ, INPUT_PULLUP);
  pinMode(pinX, OUTPUT);
  pinMode(pinY, OUTPUT);
  pinMode(pinD, OUTPUT);
  arrX[0]=0;
  arrX[0]=0;
  i++;
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  if (Execpthrow() == false) {
    if (notFinish == true) {
      Serial.println(".");
      DirButton(buttonStateDIR, buttonStateX, buttonStateY);
      Serial.println("i" + String(i));
      //Serial.println("runnning ");
      delay(500);
    }
  }
}

void DirButton(int buttonState, int XState, int YState) {
  buttonState = digitalRead(pinZ);
  if (pulseInitY > 0) {
    directionCHECK(pulseInitY);
  }
  if (pulseInitY >= pulseLimitY && pulseInitX >= pulseLimitX) {
    digitalWrite(pinY, LOW);
    digitalWrite(pinX, LOW);
    Serial.println("Finish ");
    redo();
    notFinish = false;
  }


  if (buttonState == LOW && STATE == 0) {
    if (pulseInitX == 0 and pulseInitY % 2 == 0 || (pulseInitY % 2 == 1 && (pulseInitX >= pulseLimitX))) {  //start End
      MovingMov(XState, YState, pinX);
    } else if (((pulseInitX >= pulseLimitX) || (pulseInitX == 0)) && (movingFlag == 1)) {
      MovingMov(XState, YState, pinY);
      digitalWrite(pinX, LOW);
      movingFlag = 1;
      Serial.println("1 movingFlag =" + String(movingFlag));

    } else {
      MovingMov(XState, YState, pinX);
      Serial.println(movingFlag);
      Serial.println(" yyyyyyyyoooooooooooo movwe  this huh");
    }
    Serial.println("moving");
  }

  //delay(500);
}

void xPulse(int buttonState) {
  if ((buttonState == HIGH) && (STATE == 1) && (Xcout >= Ycout)) {
    digitalWrite(pinX, LOW);
    directionCHECK(pulseInitY);
    pulseInitX = pulseInitX + direction;
    Serial.println("X Pulseisworking");
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    STATE = 0;
      arrX[i] = pulseInitX;
      arrY[i] = pulseInitY;
  }
  Ycout = 0;
  Xcout = 0;
}
void yPulse(int buttonState) {
  if ((buttonState == HIGH) && (STATE == 1) && (Ycout >= Xcout)) {
    pulseInitY++;
    digitalWrite(pinY, LOW);
    Serial.println("Y Pulseisworking");
    Serial.println("current position " + String(pulseInitX) + " " + String(pulseInitY));
    STATE = 0;
      arrX[i] = pulseInitX;
      arrY[i] = pulseInitY;
  }
  Ycout = 0;
  Xcout = 0;
}
int directionCHECK(int numb) {
  if(STATE == 1){}
  if (numb % 2 == 0) {
    direction = 1;
    digitalWrite(pinD, LOW);
  } else {
    direction = -1;
    digitalWrite(pinD, HIGH);
  }
  if ((pulseInitX == 0)) {
    digitalWrite(pinD, LOW);
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
      Serial.println("buttonStateX == LOW || buttonStateY == LOW");
      digitalWrite(pinSOMETING, HIGH);
      buttonStateX = digitalRead(pinA);
      buttonStateY = digitalRead(pinB);
      if (buttonStateX == HIGH || buttonStateY == HIGH) {
        Serial.println("BREAKK");
        xPulse(buttonStateX);
        yPulse(buttonStateY);
        i++;
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
        xPulse(buttonStateX);
        yPulse(buttonStateY);
        i++;
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
        xPulse(buttonStateX);
        yPulse(buttonStateY);
        movingFlag = 0;
        i++;
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
void returnZero() {
}
void redo() {
  redoX = arrX[i] - arrX[i-1];
  redoY = arrY[i] - arrY[i-1];
  if(redoX > 0){

  }
  else if(redoX < 0){
    
  }
  else if(redoY > 0){
    
  }
  else if(redoY < 0){
    
  }else{

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
