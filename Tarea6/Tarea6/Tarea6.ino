```cpp
#include <Arduino_LSM9DS1.h>

// Variables para guardar los datos del acelerómetro
float ax, ay, az;

// Variables para guardar los datos del giroscopio
float gx, gy, gz;

// Variables para guardar los datos del magnetómetro
float mx, my, mz;

// Variables para controlar los tiempos de muestreo y envío
unsigned long tiempoMuestreo = 0;
unsigned long tiempoEnvio = 0;

void setup() {

  // Iniciamos la comunicación serie
  Serial.begin(9600);

  // Esperamos a que se abra el puerto serie
  while (!Serial);

  // Iniciamos la IMU
  if (!IMU.begin()) {

    Serial.println("Error al iniciar la IMU");

    // Si falla la inicialización, detenemos el programa
    while (1);
  }
}

void loop() {

  // Leemos los sensores cada 100 ms
  if (millis() - tiempoMuestreo >= 100) {

    // Guardamos el instante de la última lectura
    tiempoMuestreo = millis();


    // Comprobamos si hay datos disponibles del acelerómetro
    if (IMU.accelerationAvailable()) {

      // Leemos los valores de los tres ejes
      IMU.readAcceleration(ax, ay, az);
    }


    // Comprobamos si hay datos disponibles del giroscopio
    if (IMU.gyroscopeAvailable()) {

      // Leemos los valores de los tres ejes
      IMU.readGyroscope(gx, gy, gz);
    }


    // Comprobamos si hay datos disponibles del magnetómetro
    if (IMU.magneticFieldAvailable()) {

      // Leemos los valores de los tres ejes
      IMU.readMagneticField(mx, my, mz);
    }
  }


  // Mostramos los datos por consola cada 1 segundo
  if (millis() - tiempoEnvio >= 1000) {

    // Guardamos el instante del último envío
    tiempoEnvio = millis();


    // Mostramos los valores del acelerómetro
    Serial.println("----- ACELEROMETRO -----");

    Serial.print("X: ");
    Serial.print(ax);

    Serial.print("  Y: ");
    Serial.print(ay);

    Serial.print("  Z: ");
    Serial.println(az);


    // Mostramos los valores del giroscopio
    Serial.println("----- GIROSCOPIO -----");

    Serial.print("X: ");
    Serial.print(gx);

    Serial.print("  Y: ");
    Serial.print(gy);

    Serial.print("  Z: ");
    Serial.println(gz);


    // Mostramos los valores del magnetómetro
    Serial.println("----- MAGNETOMETRO -----");

    Serial.print("X: ");
    Serial.print(mx);

    Serial.print("  Y: ");
    Serial.print(my);

    Serial.print("  Z: ");
    Serial.println(mz);

    // Dejamos una línea en blanco entre medidas
    Serial.println();
  }
}
```
