#include "mbed.h"

#define PWM_PIN 10
#define PWM_FREQUENCY 5000

mbed::PwmOut pwmPin(digitalPinToPinName(PWM_PIN));

void setup() {
  Serial.begin(9600);

  pwmPin.period(1.0 / PWM_FREQUENCY);
}

void loop() {

  long suma = 0;

  // Promedio de 20 lecturas
  for (int i = 0; i < 20; i++) {
    suma += analogRead(A0);
    delayMicroseconds(100);
  }

  int valorADC = suma / 20;

  float duty = valorADC / 1023.0;

  pwmPin.write(duty);

  Serial.print("ADC: ");
  Serial.print(valorADC);
  Serial.print("   Duty: ");
  Serial.print(duty * 100.0);
  Serial.println(" %");

  delay(10);
}