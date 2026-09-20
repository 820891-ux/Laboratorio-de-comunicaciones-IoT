```cpp
#include <Wire.h>

// Pines utilizados para la comunicación I2C
#define SDA_PIN 21
#define SCL_PIN 20

// Pin donde está conectado el LED
#define LED_PIN 1


// Función que se ejecuta cuando se recibe un dato por I2C
void recibirDato(int bytes) {

  // Leemos el dato recibido
  int dato = Wire.read();

  // Si recibimos un 1, encendemos el LED
  if (dato == 1) {
    digitalWrite(LED_PIN, HIGH);
  }

  // Si recibimos un 0, apagamos el LED
  if (dato == 0) {
    digitalWrite(LED_PIN, LOW);
  }
}


void setup() {

  // Configuramos el pin del LED como salida
  pinMode(LED_PIN, OUTPUT);

  // Iniciamos la comunicación I2C como esclavo
  // Dirección 8, pines SDA y SCL y frecuencia de 100 kHz
  Wire.begin(8, SDA_PIN, SCL_PIN, 100000);

  // Indicamos qué función se ejecutará al recibir datos
  Wire.onReceive(recibirDato);
}


void loop() {

  // No es necesario ejecutar nada aquí,
  // ya que la recepción de datos se gestiona con onReceive
}
```
