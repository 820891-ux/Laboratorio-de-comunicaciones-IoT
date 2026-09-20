#include "BBTimer.hpp"

// Creamos un temporizador usando el TIMER0
BBTimer time0(BB_TIMER0);

// Variable que indica cuándo se debe leer el ADC
// volatile porque se modifica dentro de la función del temporizador
volatile bool leer = false;

int valor;

// Esta función se ejecuta cuando salta el temporizador
void leerADC() {
  leer = true;
}

void setup() {
  // Iniciamos la comunicación serie
  Serial.begin(9600);

  // Configuramos el temporizador para que llame a leerADC
  // periódicamente
  time0.setupTimer(10000000, leerADC);

  // Arrancamos el temporizador
  time0.timerStart();
}

void loop() {

  // Cuando el temporizador pone leer a true,
  // hacemos la lectura del ADC
  if (leer == true) {

    valor = analogRead(0);

    // Mostramos el valor por consola
    Serial.println(valor);

    // Volvemos a ponerlo a false hasta la siguiente interrupción
    leer = false;
  }
}