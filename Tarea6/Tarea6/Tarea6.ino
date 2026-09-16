#include <Arduino_LSM9DS1.h>

float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

unsigned long tiempoMuestreo = 0;
unsigned long tiempoEnvio = 0;

void setup() {

  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Error al iniciar la IMU");
    while (1);
  }
}

void loop() {

  // Leer sensores cada 100 ms
  if (millis() - tiempoMuestreo >= 100) {

    tiempoMuestreo = millis();

    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(ax, ay, az);
    }

    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(gx, gy, gz);
    }

    if (IMU.magneticFieldAvailable()) {
      IMU.readMagneticField(mx, my, mz);
    }
  }


  // Mostrar datos cada 1 segundo
  if (millis() - tiempoEnvio >= 1000) {

    tiempoEnvio = millis();

    Serial.println("----- ACELEROMETRO -----");
    Serial.print("X: ");
    Serial.print(ax);
    Serial.print("  Y: ");
    Serial.print(ay);
    Serial.print("  Z: ");
    Serial.println(az);

    Serial.println("----- GIROSCOPIO -----");
    Serial.print("X: ");
    Serial.print(gx);
    Serial.print("  Y: ");
    Serial.print(gy);
    Serial.print("  Z: ");
    Serial.println(gz);

    Serial.println("----- MAGNETOMETRO -----");
    Serial.print("X: ");
    Serial.print(mx);
    Serial.print("  Y: ");
    Serial.print(my);
    Serial.print("  Z: ");
    Serial.println(mz);

    Serial.println();
  }
}