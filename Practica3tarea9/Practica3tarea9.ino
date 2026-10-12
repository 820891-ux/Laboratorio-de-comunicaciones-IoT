
#include <WiFi.h>
#include <time.h>
#include <ESP32_FTPClient.h>

// WiFi
const char* ssid = "asazet15";
const char* password = "carlosysara371969";

// Servidor FTP
char ftpServer[] = "192.168.1.40";
char ftpUser[] = "esp32";
char ftpPass[] = "Carlitos12@";

ESP32_FTPClient ftp(ftpServer, ftpUser, ftpPass);

void setup() {
  Serial.begin(115200);

  // Conectar al WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Sincronizar hora mediante NTP
  configTime(0, 0, "pool.ntp.org",
             "time.google.com", "time.cloudflare.com");

  struct tm tiempo;
  while (!getLocalTime(&tiempo, 30000)) {
    Serial.println("Esperando hora NTP...");
  }

  Serial.println("WiFi y hora configurados");
}

void loop() {
  // Obtener fecha y hora
  time_t ahora = time(nullptr);
  struct tm tiempo;
  localtime_r(&ahora, &tiempo);

  // Generar temperatura
  float temperatura = random(200, 350) / 10.0;

  // Nombre del archivo
  char nombre[40];
  sprintf(nombre, "grupoXX_%02d%02d%02d.json",
          tiempo.tm_mday, tiempo.tm_min, tiempo.tm_sec);

  // Crear JSON SenML
  char json[128];
  snprintf(json, sizeof(json),
           "[{\"n\":\"temperatura\",\"u\":\"Cel\",\"v\":%.1f,\"t\":%lld}]",
           temperatura, (long long)ahora);

  // Subir archivo por FTP
  ftp.OpenConnection();

  if (ftp.isConnected()) {
    ftp.InitFile("Type I");
    ftp.NewFile(nombre);
    ftp.Write(json);
    ftp.CloseFile();
    ftp.CloseConnection();

    Serial.println(nombre);
    Serial.println(json);
  } else {
    Serial.println("Error FTP");
  }

  delay(10000);
}
