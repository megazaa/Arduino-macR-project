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
  DirButton(buttonStateDIR);
  xPulse(buttonStateX);
  yPulse(buttonStateY);
}

void xPulse(int buttonState){
  if (buttonState == LOW) {
    digitalWrite(pinX , LOW);
    directionCHECK(pulseInitY);
    pulseInitX = pulseInitX + direction;
    Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
    delay(100);
  }
}
void yPulse(int buttonState){
  if (buttonState == LOW) {
    pulseInitY ++;
    digitalWrite(pinY , LOW);
    
    Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
    delay(100);
  }
}
void DirButton(int buttonState){
  if (buttonState == LOW) {
    digitalWrite(pinX , HIGH);
    Serial.println("moving");
    if (pulseInitX >= pulseLimitX and pulseLimitY% 2 == 0 and movingFlag<= 0){
        
        digitalWrite(pinY , HIGH);
        digitalWrite(pinX , LOW);
        movingFlag++;
        
        //Serial.println("GO GO ");
    }else{
        digitalWrite(pinX , HIGH);
        movingFlag =0;
    }
    if (pulseInitY > = pulseLimitY){
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
  } else {
    direction = -1;
  }
  return direction;
}
