/*
 * JAAF es un micro-framework para comunicar placas Arduino
 * con computadoras mediante comunicación serial.
 */

#include <Servo.h>

Servo servos[48];

#define hash 0x23
#define semicolon 0x3b 
#define tagLength 5

int tagIndex = 0;

char com[tagLength], val[tagLength], pin[tagLength];

bool comComplete = false;
bool valComplete = false;
bool pinComplete = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("READY");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) {
    delay(15);
    readData();
  }
}

void readData(){
  char thisChar = Serial.read();
  
  if(thisChar == hash){
     while(comComplete == false && Serial.available() > 0){
        char thisChar = Serial.read();
        if(thisChar == semicolon){
           comComplete = true;
           break;
        }
        else {
          com[tagIndex] = thisChar;
          tagIndex++;
        }
     }
     tagIndex = 0;
     while(pinComplete == false && Serial.available() > 0){
        char thisChar = Serial.read();
        if(thisChar == semicolon){
          pinComplete = true;
          break;
        }
        else {
          pin[tagIndex] = thisChar;
          tagIndex++;
        }
     }
     tagIndex = 0;
     while(valComplete == false && Serial.available() > 0){
        char thisChar = Serial.read();
        if(thisChar == semicolon){
          valComplete = true;
          break;
        }
        else {
          val[tagIndex] = thisChar;
          tagIndex++;
        }
     }
     tagIndex = 0;
     
     if(comComplete && valComplete && pinComplete){
       
       if(strcmp(com,"pm")==0){    // pin mode
         pm(pin, val);             // 0 = input; 1 = output
       }
       
       if(strcmp(com,"dr")==0){    // digital read
         dr(pin);
       }
       if(strcmp(com,"dw")==0){    // digital write
         dw(pin, val);             // 0 = low; 1=high;
       }
       
       if(strcmp(com,"ar")==0){    // analog read
         ar(pin);
       }
       if(strcmp(com,"aw")==0){    // analog write
         aw(pin, val);
       }
       
       if(strcmp(com,"as")==0){    // attach servo
         as(pin);
       }
       
       if(strcmp(com,"ds")==0){    // detach servo
         ds(pin);
       }
       
       if(strcmp(com,"st")==0){    // servo to position
         st(pin, val);
       }
       
       // Serial.print("com: ");Serial.print(com);Serial.print("; pin: ");Serial.print(pin);Serial.print("; val: ");Serial.println(val);
     }
     for (int i=0;i<5;i++){
       com[i] = 0x00;
       pin[i] = 0x00;
       val[i] = 0x00;
     }
     
     comComplete = false;
     pinComplete = false;
     valComplete = false;
  }
}

// Pinmode
void pm(char *pin, char *val){
  int p = atoi(pin);
  int v = atoi(val);
  switch(v){
    case 0:
      pinMode(p,INPUT);    break;
    case 1:
      pinMode(p,OUTPUT);   break;
  }
}

// DIGITAL
// Digital read
void dr(char *pin){
  int p = atoi(pin);
  Serial.println( digitalRead(p) );
}
// Digital write
void dw(char *pin, char *val){
  int p = atoi(pin);
  int v = atoi(val);
  switch(v){
    case 0:
      digitalWrite(p,LOW);    break;
    case 1:
      digitalWrite(p,HIGH);   break;
  }
}

// ANALOG
// Analog read
void ar(char *pin){
  int p = getPin(pin);
  if(p == -1) return;
  pinMode(p, INPUT);
  int rval = analogRead(p);
  char m[8];
  char v [12];
  sprintf(m, "%03d", rval);
  sprintf(v,"#ar;%s;%i;!",pin,rval);
  Serial.println( v );
}
// Analog write
void aw(char *pin, char *val){
  int p = getPin(pin);
  if(p == -1) return;
  int v = atoi(val);
  pinMode(p,OUTPUT);
  if(v >= 0 && v <= 1023){
    analogWrite(p, v);
  }
}

// SERVO

// Attach servo
void as(char *pin){
  int p = atoi(pin);
  servos[p].attach(p);
}
// Detach servo
void ds(char *pin){
  int p = atoi(pin);
  servos[p].detach();
}
// Servo to position
void st(char *pin, char *val){
  int p = atoi(pin);
  int pos = atoi(val);
  if(pos >= 0 && pos <= 180){
    servos[p].write(pos);
  }
}


int getPin(char *pin) {                  // Convierte a A0-A5, devuelve -1 en caso de error
  int r = -1;
  if(pin[0] == 'A' || pin[0] == 'a') {
    switch(pin[1]) {
      case '0':  r = A0; break;
      case '1':  r = A1; break;
      case '2':  r = A2; break;
      case '3':  r = A3; break;
      case '4':  r = A4; break;
      case '5':  r = A5; break;
      default:             break;
    }
  } else {
    r = atoi(pin);
    if(r == 0 && (pin[0] != '0' || pin[1] != '0')) {
      r = -1;
    }
  }
  return r;
}
