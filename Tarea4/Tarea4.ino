```cpp
// Variable donde se guarda el comando recibido por Serial
String comando;

#include "BBTimer.hpp"

// Creamos un temporizador
BBTimer time0(BB_TIMER0);

// Bandera para saber cuándo hay que leer el ADC
bool leer = false;

// Variable donde se guarda la lectura del ADC
int valor;

#include "mbed.h"

// Pin usado para generar la señal PWM
#define PWM_PIN 10

// Frecuencia fija de la señal PWM
#define PWM_FREQUENCY 5000

// Creamos la salida PWM en el pin 10
mbed::PwmOut pwmPin(digitalPinToPinName(PWM_PIN));


// Función llamada por el temporizador
// Solo activa la bandera de lectura
void leerADC() {
  leer = true;
}


void setup() {

  // Iniciamos la comunicación serie
  Serial.begin(9600);

  // Configuramos inicialmente el temporizador a 1 segundo
  time0.setupTimer(1000000, leerADC);

  // Configuramos el periodo de la señal PWM
  pwmPin.period(1.0 / PWM_FREQUENCY);
}


void loop() {

  // Comprobamos si el usuario ha enviado algún comando
  if (Serial.available()) {

    // Leemos el comando hasta encontrar un salto de línea
    comando = Serial.readStringUntil('\n');

    // Eliminamos espacios y saltos de línea sobrantes
    comando.trim();


    // Comando ADC:
    // realiza una única lectura del ADC
    if (comando == "ADC") {

      Serial.println(analogRead(0));
    }


    // Comando ADC(tiempo):
    // configura una lectura periódica del ADC
    else if (comando.startsWith("ADC(")) {

      // Extraemos el valor introducido entre paréntesis
      int tiempo = comando.substring(4, comando.length() - 1).toInt();

      // Si el tiempo es 0, detenemos las lecturas periódicas
      if (tiempo == 0) {

        leer = false;
        time0.timerStop();

      }

      // Si el tiempo es distinto de 0,
      // configuramos el temporizador con ese periodo
      else {

        leer = false;

        // El tiempo recibido está en segundos
        // y setupTimer trabaja en microsegundos
        time0.setupTimer(tiempo * 1000000, leerADC);

        // Arrancamos el temporizador
        time0.timerStart();
      }
    }


    // Comando PWM(valor):
    // permite cambiar el ciclo de trabajo del PWM
    else if (comando.startsWith("PWM(")) {

      // Extraemos el valor introducido entre paréntesis
      float duty = comando.substring(4, comando.length() - 1).toInt();

      // Solo aceptamos valores entre 0 y 9
      if (duty >= 0 && duty <= 9) {

        // PwmOut trabaja con valores entre 0 y 1,
        // por eso convertimos el rango 0-9 a 0.0-1.0
        pwmPin.write(duty / 9.0);
      }
    }
  }


  // Si el temporizador ha activado la bandera,
  // hacemos una lectura del ADC
  if (leer == true) {

    valor = analogRead(0);

    // Mostramos el valor por consola
    Serial.println(valor);

    // Reiniciamos la bandera hasta la siguiente lectura
    leer = false;
  }
}
```
