```cpp
#include <Wire.h>

void setup() {

  // Iniciamos la comunicación I2C
  // Al no indicar dirección, esta placa actúa como maestro
  Wire.begin();
}

void loop() {

  // Iniciamos una transmisión al esclavo con dirección 8
  Wire.beginTransmission(8);

  // Enviamos un 1 para indicar que se encienda el LED
  Wire.write(1);

  // Finalizamos la transmisión
  Wire.endTransmission();

  // Esperamos 1 segundo
  delay(1000);


  // Volvemos a iniciar una transmisión al esclavo
  Wire.beginTransmission(8);

  // Enviamos un 0 para indicar que se apague el LED
  Wire.write(0);

  // Finalizamos la transmisión
  Wire.endTransmission();

  // Esperamos 1 segundo
  delay(1000);
}
```
