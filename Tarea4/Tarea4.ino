String comando;

#include "BBTimer.hpp"
BBTimer time0(BB_TIMER0);
volatile bool leer = false;
int valor;

#include "mbed.h"
#define PWM_PIN 10
#define PWM_FREQUENCY 5000
mbed::PwmOut pwmPin(digitalPinToPinName(PWM_PIN));

void leerADC() {
  leer = true;
}

void setup() {
  Serial.begin(9600);
  time0.setupTimer(1000000, leerADC);
  pwmPin.period(1.0 / PWM_FREQUENCY);
}

void loop() {

  if (Serial.available()) {
    comando = Serial.readStringUntil('\n');
    comando.trim();

    if (comando == "ADC") {
      Serial.println(analogRead(0));
    }

    else if (comando.startsWith("ADC(")) {
      int tiempo = comando.substring(4, comando.length() - 1).toInt();
      if (tiempo == 0) {
        leer = false;
        time0.timerStop();

      } else {
        leer = false;
        time0.setupTimer(tiempo * 1000000, leerADC);
        time0.timerStart();
      }
    }

    else if (comando.startsWith("PWM(")) {
      float duty = comando.substring(4, comando.length() - 1).toInt();
      if (duty >= 0 && duty <= 9) {
        pwmPin.write(duty / 9.0);
      }
    }
  }
  if (leer == true) {
    valor = analogRead(0);
    Serial.println(valor);
    leer = false;
  }
}
