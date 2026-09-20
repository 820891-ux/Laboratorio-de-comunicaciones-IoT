```cpp
#include "mbed.h"

// Pin donde sacamos la señal PWM
#define PWM_PIN 10

// Frecuencia de la señal PWM
#define PWM_FREQUENCY 5000

// Creamos la salida PWM usando el pin 10
mbed::PwmOut pwmPin(digitalPinToPinName(PWM_PIN));

void setup() {

  // Iniciamos la comunicación serie
  Serial.begin(9600);

  // Configuramos el periodo del PWM a partir de la frecuencia
  pwmPin.period(1.0 / PWM_FREQUENCY);

}

void loop() {

  long suma = 0;

  // Hacemos 20 lecturas del ADC para obtener un valor medio
  // y reducir un poco las variaciones de la lectura
  for (int i = 0; i < 20; i++) {

    suma += analogRead(A0);

    // Pequeña espera entre lecturas
    delayMicroseconds(100);

  }

  // Calculamos el valor medio del ADC
  int valorADC = suma / 20;

  // Pasamos el valor del ADC de 0-1023 a un valor entre 0 y 1
  // que es el formato que necesita pwmPin.write()
  float duty = valorADC / 1023.0;

  // Aplicamos ese valor como ciclo de trabajo del PWM
  pwmPin.write(duty);

  // Mostramos el valor del ADC y el duty cycle por consola
  Serial.print("ADC: ");
  Serial.print(valorADC);

  Serial.print("   Duty: ");
  Serial.print(duty * 100.0);

  Serial.println(" %");

  delay(10);

}
```
