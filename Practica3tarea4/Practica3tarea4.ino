#include <WiFi.h>
#include <time.h>

// WiFi
const char* ssid = "Redmi Note 9 Pro";
const char* password = "TuPutaMadre";

// Servidor TCP (ordenador)
IPAddress servidor(10, 45, 152, 36); // CAMBIAR por la IP de tu PC
const int puerto = 5000;

WiFiClient cliente;

void setup() {
  Serial.begin(115200);

  // Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP());

  // Sincronización NTP y zona horaria española
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");

  struct tm tiempo;
  while (!getLocalTime(&tiempo)) {
    delay(500);
  }

  // Conexión al servidor TCP
  while (!cliente.connect(servidor, puerto)) {
    Serial.println("Esperando servidor TCP...");
    delay(1000);
  }

  Serial.println("Conectado al servidor TCP");
}

void loop() {
  // Reconectar si se pierde la conexión
  if (!cliente.connected()) {
    cliente.stop();
    cliente.connect(servidor, puerto);
    delay(1000);
    return;
  }

  struct tm tiempo;
  if (getLocalTime(&tiempo)) {
    char hora[9];
    strftime(hora, sizeof(hora), "%H:%M:%S", &tiempo);

    cliente.println(hora);  // Enviar al ordenador
    Serial.println(hora);   // Mostrar también por UART
  }

  delay(1000);
}



