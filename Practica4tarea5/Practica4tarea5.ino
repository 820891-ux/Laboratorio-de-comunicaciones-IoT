
#include <BluetoothSerial.h> // Librería para Bluetooth Classic

// Creamos el objeto para la comunicación Bluetooth
BluetoothSerial SerialBT;

void setup() {
  // Inicializamos la comunicación serie con el ordenador
  Serial.begin(115200);

  // Habilitamos el emparejamiento SSP
  SerialBT.enableSSP(false, true);

  // Inicializamos Bluetooth con el nombre del dispositivo
  SerialBT.begin("ESP32_820891");

  Serial.println("Bluetooth iniciado");
  Serial.println("Esperando conexion...");
}

void loop() {

  // Si recibimos datos desde el ordenador
  if (Serial.available()) {
    // Enviamos el carácter recibido al móvil por Bluetooth
    SerialBT.write(Serial.read());
  }

  // Si recibimos datos desde el móvil
  if (SerialBT.available()) {
    // Mostramos el carácter recibido en el monitor serie
    Serial.write(SerialBT.read());
  }
}
