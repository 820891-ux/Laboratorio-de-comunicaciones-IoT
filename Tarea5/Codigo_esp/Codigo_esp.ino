#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 20
#define LED_PIN 1

void recibirDato(int bytes) {

  int dato = Wire.read();

  if (dato == 1) {
    digitalWrite(LED_PIN, HIGH);
  }

  if (dato == 0) {
    digitalWrite(LED_PIN, LOW);
  }
}

void setup() {

  pinMode(LED_PIN, OUTPUT);

  Wire.begin(8, SDA_PIN, SCL_PIN, 100000);

  Wire.onReceive(recibirDato);
}

void loop() {
}