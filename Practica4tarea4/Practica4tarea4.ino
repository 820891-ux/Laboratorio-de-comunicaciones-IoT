
#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>
#include <Wire.h>

// Servicio BLE
BLEService servicio("38323038-3931-0000-0000-000000000001");

// Característica de lectura y notificaciones
BLEStringCharacteristic aceleracion(
  "38323038-3931-0000-0000-000000000003",
  BLERead | BLENotify, 20
);

// Característica de escritura para controlar el acelerómetro
BLEByteCharacteristic control(
  "38323038-3931-0000-0000-000000000004",
  BLEWrite
);

unsigned long ultimoEnvio = 0;
bool acelerometroActivo = true;

// Activamos o desactivamos el acelerómetro por I2C
bool configurarAcelerometro(bool activar) {
  Wire1.beginTransmission(0x6B);
  Wire1.write(0x20);                  // Registro CTRL_REG6_XL
  Wire1.write(activar ? 0x70 : 0x10); // Activar o apagar
  return Wire1.endTransmission() == 0;
}

void setup() {
  Serial.begin(115200);

  // Inicializamos la IMU una sola vez
  if (!IMU.begin()) {
    Serial.println("Error IMU");
    while (1);
  }

  if (!BLE.begin()) {
    Serial.println("Error BLE");
    while (1);
  }

  // Configuramos Bluetooth
  BLE.setLocalName("820891");
  BLE.setAdvertisedService(servicio);

  servicio.addCharacteristic(aceleracion);
  servicio.addCharacteristic(control);
  BLE.addService(servicio);

  aceleracion.writeValue("0.00,0.00,0.00");

  BLE.advertise();
  Serial.println("Esperando conexion BLE...");
}

void loop() {
  BLE.poll();

  // Comprobamos si se recibe un comando
  if (control.written()) {
    byte valor = control.value();

    if ((valor == 0 || valor == 1) &&
        (valor == 1) != acelerometroActivo) {

      bool activar = (valor == 1);

      if (configurarAcelerometro(activar)) {
        acelerometroActivo = activar;
        ultimoEnvio = millis();

        Serial.println(activar ?
          "Acelerometro activado" :
          "Acelerometro desactivado");
      } else {
        Serial.println("Error I2C");
      }
    }
  }

  // Enviamos los datos solo cuando está activado
  if (acelerometroActivo &&
      millis() - ultimoEnvio >= 100 &&
      IMU.accelerationAvailable()) {

    ultimoEnvio = millis();

    float x, y, z;

    if (IMU.readAcceleration(x, y, z)) {
      String datos = String(x, 2) + "," +
                     String(y, 2) + "," +
                     String(z, 2);

      aceleracion.writeValue(datos);
      Serial.println(datos);
    }
  }
}
