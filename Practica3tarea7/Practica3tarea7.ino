
#include <WiFi.h>       // Librería para conectar el ESP32 a WiFi
#include <WebServer.h>  // Librería para crear el servidor web

// Credenciales de la red WiFi
const char* ssid = "TU_WIFI";
const char* password = "TU_PASSWORD";

// Crear servidor web en el puerto 80 (HTTP)
WebServer server(80);

// Variable para guardar el instante de inicio del reloj
unsigned long inicio = 0;

void setup() {
  // Inicializar comunicación serie
  Serial.begin(115200);

  // Conectar el ESP32 a la red WiFi
  WiFi.begin(ssid, password);

  // Esperar hasta que se establezca la conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Mostrar la dirección IP del ESP32
  Serial.println(WiFi.localIP());

  // Guardar el instante de inicio del reloj
  inicio = millis();

  // Configurar la respuesta cuando se accede a la página principal "/"
  server.on("/", []() {

    // Calcular los segundos transcurridos desde el inicio
    unsigned long t = (millis() - inicio) / 1000;

    // Convertir los segundos a horas, minutos y segundos
    int h = (t / 3600) % 24;
    int m = (t / 60) % 60;
    int s = t % 60;

    // Crear una cadena con formato HH:MM:SS
    char hora[12];
    sprintf(hora, "%02d:%02d:%02d", h, m, s);

    // Crear la página HTML
    // Se actualiza automáticamente cada segundo
    String html = "<meta http-equiv='refresh' content='1'>";

    // Mostrar la hora en la página
    html += "<h1>Hora: " + String(hora) + "</h1>";

    // Crear un formulario que envía una petición POST a /reset
    html += "<form action='/reset' method='POST'>";

    // Añadir el botón para reiniciar el reloj
    html += "<button>Resetear hora</button></form>";

    // Enviar la página HTML al navegador
    server.send(200, "text/html", html);
  });

  // Configurar la acción cuando se pulsa el botón de reset
  server.on("/reset", HTTP_POST, []() {

    // Reiniciar el contador guardando el instante actual
    inicio = millis();

    // Indicar al navegador que vuelva a la página principal
    server.sendHeader("Location", "/");

    // Enviar una respuesta HTTP de redirección
    server.send(303);
  });

  // Iniciar el servidor web
  server.begin();
}

void loop() {
  // Comprobar y atender las peticiones de los navegadores
  server.handleClient();
}
