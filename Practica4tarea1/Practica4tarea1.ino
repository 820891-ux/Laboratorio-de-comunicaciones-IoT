#include <ArduinoBLE.h> // Librería para utilizar Bluetooth Low Energy (BLE)

// Creamos un servicio BLE con un UUID que identifica al servicio
BLEService servicio("416c756d-6e6f-0000-0000-000000000001");

void setup() {
  Serial.begin(115200); // Inicializamos la comunicación serie

  // Inicializamos el módulo Bluetooth
  if (!BLE.begin()) {
    Serial.println("Error BLE");
    while (1); // Detenemos el programa si falla la inicialización
  }

  // Establecemos el nombre del dispositivo Bluetooth
  BLE.setLocalName("820891");

  // Indicamos qué servicio queremos anunciar mediante advertising
  BLE.setAdvertisedService(servicio);

  // Añadimos el servicio al dispositivo BLE
  BLE.addService(servicio);

  // Comenzamos a emitir paquetes de advertising
  BLE.advertise();
}

void loop() {

}