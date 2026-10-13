
#include <ArduinoBLE.h>

BLEService servicio("38323038-3931-0000-0000-000000000001");


BLEByteCharacteristic color("38323038-3931-0000-0000-000000000002", BLERead | BLEWrite);

void setup() {
  Serial.begin(115200);

  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);

  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDB, HIGH);

  if (!BLE.begin()) {
    Serial.println("Error BLE");
    while (1);
  }

  BLE.setLocalName("820891");
  BLE.setAdvertisedService(servicio);

  servicio.addCharacteristic(color);
  BLE.addService(servicio);

  color.writeValue(0); // Valor inicial

  BLE.advertise();
  Serial.println("Esperando conexion BLE...");
}

void loop() {
  // Esperamos a que un dispositivo se conecte
  BLEDevice central = BLE.central();

  if (central) {
    Serial.println("Movil conectado");

    // Mientras el movil siga conectado
    while (central.connected()) {

      // Comprobamos si se ha escrito en la caracteristica
      if (color.written()) {
        byte valor = color.value();

        Serial.print("Valor recibido: ");
        Serial.println(valor);

        // Apagamos los tres LED
        digitalWrite(LEDR, HIGH);
        digitalWrite(LEDG, HIGH);
        digitalWrite(LEDB, HIGH);

        switch (valor) {
          case 1:
            digitalWrite(LEDR, LOW);
            break;
          case 2:
            digitalWrite(LEDG, LOW);
            break;
          case 3:
            digitalWrite(LEDB, LOW);
            break;
          case 4:
            digitalWrite(LEDR, LOW);
            digitalWrite(LEDG, LOW);
            digitalWrite(LEDB, LOW);
            break;
        }
      }
    }

    Serial.println("Movil desconectado");
  }
}
