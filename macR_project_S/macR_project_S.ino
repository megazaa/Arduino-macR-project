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
int STATE = -1;
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
  if (true == true){
    initStart(buttonStateDIR,buttonStateX,buttonStateY);
    DirButton(buttonStateDIR,buttonStateX,buttonStateY);
    xPulse(buttonStateX);
    yPulse(buttonStateY);
    Serial.println(STATE);
  }else{
    Serial.println("something wrong");
  }
}

void xPulse(int buttonState){
  if ((buttonState == HIGH) && (STATE == 1)) {
    digitalWrite(pinX , LOW);
    digitalWrite(pinY , LOW);
    directionCHECK(pulseInitY);
    pulseInitX = pulseInitX + direction;
    //Serial.println("buttonState 1");
    Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
    STATE = 0;
  }
  delay(100);
}
void yPulse(int buttonState){
  if ((buttonState == HIGH) && (STATE == 1)) {
    pulseInitY ++;
    digitalWrite(pinY , LOW);
    digitalWrite(pinX , LOW);
    //Serial.println("2");
    Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
    STATE = 0;    
  }
  delay(100);
}
void DirButton(int buttonState,int XState,int YState){
  
  if (buttonState == LOW && STATE == 0) {
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
  //Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
  delay(500);
}
void initStart(int buttonState, int buttonStateX, int buttonStateY){
  if (buttonState == LOW && STATE == -1){
    if (pulseInitX == 0 and pulseInitY == 0){
      digitalWrite(pinX , HIGH);
      STATE = 1;
      delay(5000);    
      buttonStateX = digitalRead(pinA);
      buttonStateY = digitalRead(pinB);
      //pulseInitX ++;
    }
    if(buttonStateX == HIGH && buttonStateY == HIGH){
      digitalWrite(pinX , LOW);
      Serial.println("Error: not on rail ");
      pulseInitY = 0;
      pulseInitX = 0;
      Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
      STATE = -1;
    }else{
      Serial.println("READY");
      //digitalWrite(pinX , LOW);
      Serial.println("current position " + String(pulseInitX)+" "+String(pulseInitY));
      STATE = 0;
    }
  }
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

bool error(int StateX,int StateY) {
  if (StateX == HIGH && StateY == HIGH && STATE == 1){
    Serial.println("NOt on the rail I guess");

    return false;
  }else if(pulseInitY > pulseLimitY && pulseInitX > pulseLimitX){
    Serial.println("AYO how?");
    return false;
  }else{
    return true;
  }

}
