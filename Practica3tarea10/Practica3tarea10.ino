
#include "AdafruitIO_WiFi.h"

// Credenciales WiFi
#define WIFI_SSID "Redmi Note 9 Pro"
#define WIFI_PASS "TuPutaMadre"

// Credenciales Adafruit IO
#define IO_USERNAME "ESCRIBIR_USUARIO"
#define IO_KEY "ESCRIBIR_CLAVE"

// Conexion Adafruit IO
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Feed para escribir y leer
AdafruitIO_Feed *temperatura = io.feed("temperatura");

unsigned long ultimoEnvio = 0;

// Funcion para recibir datos
void recibirDato(AdafruitIO_Data *data) {
  Serial.print("Dato recibido: ");
  Serial.println(data->value());
}

void setup() {
  Serial.begin(115200);

  // Suscribirse al feed
  temperatura->onMessage(recibirDato);

  // Conectar a Adafruit IO
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nConectado a Adafruit IO");

  // Obtener ultimo valor almacenado
  temperatura->get();

  ultimoEnvio = millis();
}

void loop() {
  // Mantener conexion y recibir mensajes
  io.run();

  // Enviar datos cada 10 segundos
  if (millis() - ultimoEnvio >= 10000) {
    ultimoEnvio = millis();

    // Generar temperatura aleatoria
    float valor = random(200, 301) / 10.0;

    // Enviar valor al feed
    if (temperatura->save(valor)) {
      Serial.print("Dato enviado: ");
      Serial.println(valor);
    } else {
      Serial.println("Error al enviar");
    }
  }
}
