int pinA = 2;
int pinB = 3;
int pinX = 5;
int pinY = 6;
int pinZ = 8;
int pinD = 9;

int movingFlag = 0;
int direction = 1;
int pulseInitX = 0, pulseInitY = 0;
int pulseLimitX = 4,pulseLimitY = 2; 
int STATE = 0;
void setup() {
  pinMode(pinA, INPUT_PULLUP  );
  pinMode(pinB, INPUT_PULLUP  );
  pinMode(pinZ, INPUT_PULLUP  );
  pinMode(pinX, OUTPUT  );
  pinMode(pinY, OUTPUT  );
  pinMode(pinD, OUTPUT  );

  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  int buttonStateX = digitalRead(pinA);
  int buttonStateY = digitalRead(pinB);
  int buttonStateDIR = digitalRead(pinZ);
  DirButton(buttonStateDIR,buttonStateX,buttonStateY);
  xPulse(buttonStateX);
  yPulse(buttonStateY);
  Serial.println(STATE);
}

void xPulse(int buttonState){
  if ((buttonState == HIGH) && (STATE == 1)) {
    digitalWrite(pinX , LOW);
    digitalWrite(pinY , LOW);
    directionCHECK(pulseInitY);
    pulseInitX = pulseInitX + direction;
    Serial.println("1");
    Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
    STATE = 0;
    delay(100);
  }
}
void yPulse(int buttonState){
  if ((buttonState == HIGH) && (STATE == 1)) {
    pulseInitY ++;
    digitalWrite(pinY , LOW);
    digitalWrite(pinX , LOW);
    Serial.println("2");
    Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
    STATE = 0;
    delay(100);
  }
}
void DirButton(int buttonState,int XState,int YState){
  
  if (buttonState == LOW) {
    STATE = 1;
    //digitalWrite(pinX , HIGH);
    Serial.println("moving");
    
    if (pulseInitX == 0 and pulseInitY % 2 == 0){ //start End
      digitalWrite(pinX , HIGH);
    }
    else if (((pulseInitX >= pulseLimitX)||(pulseInitX == 0)) && movingFlag<= 0){
        directionCHECK(pulseInitY);    
        digitalWrite(pinY , HIGH);
        digitalWrite(pinX , LOW);
        movingFlag++;
        
        //Serial.println("GO GO ");
    }
    else{
        digitalWrite(pinX , HIGH);
        movingFlag =0;
    }
    if (pulseInitY >= pulseLimitY && pulseInitX >= pulseLimitX){
        digitalWrite(pinY , LOW);
        digitalWrite(pinX , LOW);
        Serial.println("Finish ");
    }
  }
  
  
  delay(500);
}

int directionCHECK(int numb) {
  if (numb % 2 == 0 ) {
    direction = 1;
    digitalWrite(pinD , LOW);
  } else {
    direction = -1;
    digitalWrite(pinD , HIGH);
  }
  return direction;
}
