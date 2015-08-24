#include <string.h>
#include <Servo.h>

#define MAX_SERIAL_LEN 20 // Tamaño máximo del mensaje
#define BAUDRATE 9600
char j_inData[MAX_SERIAL_LEN];
char **j_args;
byte j_index = 0;
Servo j_servos[48];

void setup() {
  Serial.begin(BAUDRATE);
}

void loop() {
  jaaf();
}


// Función principal de jaaf
void jaaf(){
  if(Serial.available() > 0)
  {
    delay(MAX_SERIAL_LEN);
    char currChar = Serial.read();
    if(currChar == '#')
    {
      while(Serial.available() > 0){
        currChar = Serial.read();
        if(currChar == '!')
        {
          // Terminamos de leer, ahora parseamos
          jaaf_parse(j_inData);
          jaaf_switch(j_args);
        }
        else
        {
          // Comenzamos a guardar los datos
          j_inData[j_index] = currChar;
          j_index++;
          j_inData[j_index] = '\0';
        }
      }
      // Vaciamos el array dejandolo libre para la próxima iteración
      for( int i = 0; i < j_index;  i++ ){
        j_inData[i] = '\0'; 
      }
      j_index = 0;
      j_args = {};
    }
  }
}

// Función para parsear el mensaje recibido
void jaaf_parse(char *data){
  char *buf;
  int i = 0;
  buf = strtok(data, ",");
  while(buf != NULL){
    j_args[++i] = buf;
    buf = strtok(NULL, ",");
  }
}

// Dado un array de parámetros, elegimos la acción correspondiente
void jaaf_switch(char **args){
  char *com = args[1];
  char *pin = args[2];
  char *val = args[3];
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
}


// Funciones generales de arduino

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
  int rval = digitalRead(p);
  char v[12];
  sprintf(v,"#dr,%i,%i!", p, rval);
  Serial.println( v );
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
  char v [12];
  sprintf(v,"#ar,%s,%i!",pin,rval);
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
  j_servos[p].attach(p);
}
// Detach servo
void ds(char *pin){
  int p = atoi(pin);
  j_servos[p].detach();
}
// Servo to position
void st(char *pin, char *val){
  int p = atoi(pin);
  int pos = atoi(val);
  if(pos >= 0 && pos <= 180){
    j_servos[p].write(pos);
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
