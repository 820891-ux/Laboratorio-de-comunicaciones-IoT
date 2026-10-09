#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

#define LED 23

float ax, ay, az;

unsigned long tiempoMuestreo = 0;
unsigned long tiempoEnvio = 0;
unsigned long tiempoLED = 0;

bool ledEncendido = false;

void setup() {

  Serial.begin(9600);

  while (!Serial);

  // SDA = GPIO 21
  // SCL = GPIO 22
  Wire.begin(21, 22);

  if (!mpu.begin()) {

    Serial.println("No se encuentra el MPU6050");

    while (1);
  }

  Serial.println("MPU6050 iniciado");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {

  // Leer aceleración cada 100 ms
  if (millis() - tiempoMuestreo >= 100) {

    tiempoMuestreo = millis();

    sensors_event_t a, g, temp;

    mpu.getEvent(&a, &g, &temp);

    ax = a.acceleration.x;
    ay = a.acceleration.y;
    az = a.acceleration.z;
  }


  // Enviar aceleración cada 1 segundo
  if (millis() - tiempoEnvio >= 1000) {

    tiempoEnvio = millis();

    Serial.print("Accel X: ");
    Serial.print(ax);

    Serial.print(" Y: ");
    Serial.print(ay);

    Serial.print(" Z: ");
    Serial.println(az);

    // Encender LED cuando se envían los datos
    digitalWrite(LED, HIGH);

    tiempoLED = millis();
    ledEncendido = true;
  }


  // Apagar LED después de 200 ms
  if (ledEncendido && millis() - tiempoLED >= 200) {

    digitalWrite(LED, LOW);
    ledEncendido = false;
  }
}


