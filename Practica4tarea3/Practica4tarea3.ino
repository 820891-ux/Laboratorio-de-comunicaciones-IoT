
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

// Servicio BLE
BLEService servicio("38323038-3931-0000-0000-000000000001");

// Característica de aceleración: solo lectura y notificaciones
BLEStringCharacteristic aceleracion(
  "38323038-3931-0000-0000-000000000003",
  BLERead | BLENotify, 20
);

unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);

  // Inicializamos el acelerómetro
  if (!IMU.begin()) {
    Serial.println("Error IMU");
    while (1);
  }

  // Inicializamos Bluetooth
  if (!BLE.begin()) {
    Serial.println("Error BLE");
    while (1);
  }

  // Configuramos el dispositivo BLE
  BLE.setLocalName("820891");
  BLE.setAdvertisedService(servicio);

  // Añadimos la característica al servicio
  servicio.addCharacteristic(aceleracion);
  BLE.addService(servicio);

  // Valor inicial de la característica
  aceleracion.writeValue("0.00,0.00,0.00");

  // Iniciamos el advertising
  BLE.advertise();

  Serial.println("Esperando conexion BLE...");
}

void loop() {
  BLE.poll(); // Procesamos los eventos Bluetooth

  // Actualizamos las aceleraciones cada 100 ms
  if (millis() - ultimoEnvio >= 100 && IMU.accelerationAvailable()) {
    ultimoEnvio = millis();

    float x, y, z;

    // Leemos los tres ejes del acelerómetro
    if (IMU.readAcceleration(x, y, z)) {

      // Convertimos las medidas en una cadena de texto
      String datos = String(x, 2) + "," +
                     String(y, 2) + "," +
                     String(z, 2);

      // Actualizamos la característica GATT
      aceleracion.writeValue(datos);

      // Mostramos los datos por el monitor serie
      Serial.println(datos);
    }
  }
}
