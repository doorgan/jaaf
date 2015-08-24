#JAAF para arduino
JAAF(Just Another Arduino Framework) es un protocolo pensado para ser lo más pequeño, flexible, fácil de comprender y extender para comunicar placas Arduino con computadoras vía comunicación serial.


##Instalación
Descargar jaaf.ino, abrir con el IDE de Arduino y subirlo a la placa Arduino.

##Cómo usar
- Dentro de la función `setup` crear una conexión serial.
- Dentro de la función `loop` llamar a la función `jaaf()`.

Para agregar funciones se debe modificar la función `jaaf_switch`.
Dentro de ella hay declaradas tres variables por defecto: `com` `pin` y `val`, cuyo contenido es obvio.
Esta función toma como único argumento un array multidimensional de caracteres producto de la función `jaaf_parse`.
Dentro de este array `args` está el contenido del mensaje recibido, siendo cada índice del array un segmento del mensaje, empezando por el índice `1`.
Por ejemplo:
```
Mensaje recibido: #dw,13,1!

args[1] = "dw";
args[2] = "13";
args[3] = "1";
```

Sabiendo esto, y que el contenido de `args` son arrays de caracteres, si queremos agregar una nueva función deberemos agregar o modificar un bloque `if` dentro de `jaaf_switch`.

Por ejemplo:
```
// Agregamos la función fade, la cual toma como argumentos el pin, el valor final y el tiempo que demora en llegar hasta dicho valor
if(strcmp(com,"fade")==0){
  fade(pin, val, args[3]);
}

// --resto del código--

// Esta es la función fade
void fade(char *pin, char *val, char *time) {
  // Convertimos los caracteres a números enteros
  int p = atoi(pin);
  int v = atoi(val);
  int t = atoi(time);
  
  // Lógica de la función, es decir, el fade en sí
}
```

##Generalidades

Los mensajes que recibe el arduino cargado con este protocolo deberán ser de la forma
`#accion,val1,val2,val3,...,valN!`

Siendo `#` el caracter que indica el inicio de la cadena, `!` su fin y `,` el separador de los parámetros.
Por defecto el mensaje tiene un máximo de 20 caracteres incluyendo los delimitadores(`#!,`).
Para cambiar la longitud máxima del mensaje basta con modificar la constante `MAX_SERIAL_LEN`.

**Ejemplos**
```
#pm,10,0!
#dr,10!
#ar,A0!
#as,9!
#st,9,180!
```

##Funciones
A continuación se incluye una lista con las acciones predeterminadas de JAAF

###pinMode

`#pm,pin,valor!`

`valor` puede ser:

0 = INPUT

1 = OUTPUT

###digitalRead

`#dr,pin!`

devuelve `#dr,pin,valor!`

###digitalWrite

`#dw,pin,valor!`

###analogRead

`#ar,pin!`

devuelve `#ar,pin,valor!`

###analogWrite

`#aw,pin,valor!`

###attachServo

`#as,pin!`

Asigna un servo al pin `pin`

###detachServo

`#ds,pin!`

Lo opuesto a attachServo

###servoToPosition

`#st,pin,valor!`

Mueve el servo asignado al pin `pin` hacia el ángulo `valor`

##Licencia

Copyright (c) 2015, doorgan <>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
