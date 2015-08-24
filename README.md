#JAAF para arduino
JAAF(Just Another Arduino Framework) es un protocolo pensado para ser lo más pequeño, flexible, fácil de comprender y extender para comunicar placas Arduino con computadoras vía comunicación serial.


##Instalación
Descargar jaaf.ino, abrir con el IDE de Arduino y subirlo a la placa Arduino.


##Generalidades

Los mensajes que recibe el arduino cargado con este protocolo deberán ser de la forma
`#accion,val1,val2,val3,...,valN!`

Siendo `#` el caracter que indica el inicio de la cadena, `!` su fin y `,` el separador de los parámetros.
Por defecto el mensaje tiene un máximo de 20 caracteres incluyendo los delimitadores(`#!,`)

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
