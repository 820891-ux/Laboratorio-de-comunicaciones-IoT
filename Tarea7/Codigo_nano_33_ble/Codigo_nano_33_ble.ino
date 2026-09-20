```cpp
#include <Wire.h>
#include <Arduino_LSM9DS1.h>

// Dirección I2C del esclavo
#define DIRECCION_ESCLAVO 0x08

// Guardamos 5 muestras de cada eje de los tres sensores
// porque medimos cada 200 ms durante 1 segundo
float ax[5], ay[5], az[5];
float gx[5], gy[5], gz[5];
float mx[5], my[5], mz[5];

void setup() {

  // Iniciamos la comunicación serie
  Serial.begin(9600);

  // Iniciamos I2C como maestro
  Wire.begin();

  // Iniciamos la IMU
  if (!IMU.begin()) {
    Serial.println("Error al iniciar la IMU");
    while (1);
  }

  Serial.println("Escribe MEDIR para comenzar");
}

void loop() {

  // Esperamos a que el usuario escriba un comando
  if (Serial.available()) {

    // Leemos el comando recibido
    String comando = Serial.readStringUntil('\n');
    comando.trim();

    // Si el usuario escribe MEDIR comenzamos la toma de datos
    if (comando == "MEDIR") {

      Serial.println("Midiendo...");

      // Tomamos 5 muestras separadas 200 ms
      for (int i = 0; i < 5; i++) {

        // Leemos el acelerómetro si hay datos disponibles
        if (IMU.accelerationAvailable()) {
          IMU.readAcceleration(ax[i], ay[i], az[i]);
        }

        // Leemos el giroscopio si hay datos disponibles
        if (IMU.gyroscopeAvailable()) {
          IMU.readGyroscope(gx[i], gy[i], gz[i]);
        }

        // Leemos el magnetómetro si hay datos disponibles
        if (IMU.magneticFieldAvailable()) {
          IMU.readMagneticField(mx[i], my[i], mz[i]);
        }

        // Esperamos 200 ms antes de la siguiente muestra
        delay(200);
      }

      Serial.println("Medicion terminada. Enviando por I2C...");

      // Enviamos por I2C las 5 muestras almacenadas
      for (int i = 0; i < 5; i++) {

        // Enviamos los datos del acelerómetro
        enviarDatos(i, 0, ax[i], ay[i], az[i]);

        // Enviamos los datos del giroscopio
        enviarDatos(i, 1, gx[i], gy[i], gz[i]);

        // Enviamos los datos del magnetómetro
        enviarDatos(i, 2, mx[i], my[i], mz[i]);

        delay(10);
      }

      Serial.println("Datos enviados");
      Serial.println("Escribe MEDIR para volver a medir");
    }
  }
}


// Función para enviar una muestra por I2C
void enviarDatos(byte muestra, byte sensor, float x, float y, float z) {

  // Iniciamos la transmisión al esclavo
  Wire.beginTransmission(DIRECCION_ESCLAVO);

  // Indicamos qué número de muestra estamos enviando
  Wire.write(muestra);

  // Indicamos qué sensor estamos enviando:
  // 0 = acelerómetro
  // 1 = giroscopio
  // 2 = magnetómetro
  Wire.write(sensor);

  // Enviamos los tres valores float como bytes
  Wire.write((byte*)&x, sizeof(float));
  Wire.write((byte*)&y, sizeof(float));
  Wire.write((byte*)&z, sizeof(float));

  // Finalizamos la transmisión
  Wire.endTransmission();
}
```
