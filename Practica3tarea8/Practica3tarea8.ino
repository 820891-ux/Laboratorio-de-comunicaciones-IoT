
#include <WiFi.h>
#include <time.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>

// Credenciales WiFi
const char* ssid = "asazet15";
const char* password = "carlosysara371969";

void setup() {
  Serial.begin(115200);

  // Inicializar el sistema de archivos
  if (!SPIFFS.begin(true)) {
    Serial.println("Error al iniciar SPIFFS");
    return;
  }

  // Conectar al WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi conectado");

  // Sincronizar el reloj mediante NTP
  configTime(0, 0, "pool.ntp.org");

  struct tm tiempo;
  if (!getLocalTime(&tiempo, 10000)) {
    Serial.println("Error al obtener la hora");
    while (true) delay(1000);
  }
}

void loop() {
  // Generar temperatura aleatoria entre 20 y 35 grados
  float temperatura = random(200, 350) / 10.0;

  // Crear documento JSON con formato SenML
  JsonDocument doc;
  JsonArray datos = doc.to<JsonArray>();
  JsonObject medida = datos.add<JsonObject>();

  medida["n"] = "temperatura";
  medida["u"] = "Cel";
  medida["v"] = temperatura;
  medida["t"] = time(nullptr);

  // Crear o sobrescribir el archivo JSON
  File archivo = SPIFFS.open("/temperatura.json", "w");

  if (archivo) {
    serializeJsonPretty(doc, archivo);
    archivo.close();

    // Mostrar los datos en el monitor serie
    serializeJsonPretty(doc, Serial);
    Serial.println();
  } else {
    Serial.println("Error al crear el archivo");
  }

  // Esperar 10 segundos
  delay(10000);
}
