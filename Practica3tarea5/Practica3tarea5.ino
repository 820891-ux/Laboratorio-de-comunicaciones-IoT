
#include <WiFi.h>
#include <time.h>

// Configuración de red WiFi
const char* ssid = "Redmi Note 9 Pro";
const char* password = "TuPutaMadre";

// Configuración del servidor Socket
const char* serverIP = "10.45.152.36";
const int serverPort = 450;

// Configuración NTP
const char* ntpServer = "pool.ntp.org";
const char* tzInfo = "CET-1CEST,M3.5.0,M10.5.0/3";

WiFiClient client;

unsigned long lastSendTime = 0;
unsigned long lastReconnectTime = 0;

bool enviarHora = false;
String comando = "";

void setup() {
  Serial.begin(115200);

  // 1. Conexión WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Conectado.");
  Serial.println(WiFi.localIP());

  // 2. Sincronización NTP
  configTzTime(tzInfo, ntpServer);

  struct tm timeinfo;

  while (!getLocalTime(&timeinfo)) {
    Serial.println("Esperando sincronización NTP...");
    delay(1000);
  }

  Serial.println("Hora sincronizada correctamente.");

  // 3. Conexión al servidor TCP
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Conectado al servidor socket correctamente.");
  } else {
    Serial.println("Error de conexión con el servidor.");
  }
}

void loop() {

  // Reconectar si se pierde la conexión
  if (!client.connected()) {
    enviarHora = false;
    comando = "";

    if (millis() - lastReconnectTime >= 1000) {
      lastReconnectTime = millis();

      client.stop();
      Serial.println("Reconectando al servidor...");
      client.connect(serverIP, serverPort);
    }
    return;
  }

  // 4. Leer comandos recibidos por TCP
  while (client.available()) {
    char c = client.read();

    if (c == '\n' || c == '\r') {
      comando.trim();

      if (comando == "start") {
        enviarHora = true;
        lastSendTime = millis();
        Serial.println("Envio de hora activado.");
      }

      else if (comando == "stop") {
        enviarHora = false;
        Serial.println("Envio de hora detenido.");
      }

      comando = "";
    }
    else {
      comando += c;
    }
  }

  // 5. Enviar la hora cada segundo si está activado
  if (enviarHora && millis() - lastSendTime >= 1000) {
    lastSendTime = millis();

    struct tm timeinfo;

    if (getLocalTime(&timeinfo, 10)) {
      char timeStringBuff[50]; //Realmente con 20 vale

      strftime(timeStringBuff, sizeof(timeStringBuff),
               "%Y-%m-%d %H:%M:%S", &timeinfo);

      client.println(timeStringBuff);

      Serial.print("Enviado: ");
      Serial.println(timeStringBuff);
    }
  }
}
