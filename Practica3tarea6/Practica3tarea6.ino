
#include <WiFi.h>

// Configuracion WiFi

const char* ssid = "Redmi Note 9 Pro";
const char* password = "TuPutaMadre";

// Configuración del servidor Socket
const char* serverIP = "10.45.152.36";
const int serverPort = 450;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Conexion WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectada");
  Serial.println(WiFi.localIP());
}

void loop() {

  // Conectar al servidor TCP
  if (!client.connected()) {
    Serial.println("Conectando al servidor...");

    if (!client.connect(serverIP, serverPort)) {
      Serial.println("Error de conexion");
      delay(1000);
      return;
    }
  }

  // Generar valores aleatorios
  float ax = random(-100, 101) / 100.0;
  float ay = random(-100, 101) / 100.0;
  float az = random(-100, 101) / 100.0;

  // Enviar datos por TCP
  client.print(ax);
  client.print(",");
  client.print(ay);
  client.print(",");
  client.println(az);

  // Mostrar datos por puerto serie
  delay(1000);
}
