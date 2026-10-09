#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "esp_freertos_hooks.h"

Adafruit_MPU6050 mpu;

#define LED 23

float ax, ay, az;



bool funcionIdle()
{
  // Aquí el micro está sin trabajo útil.
  // De momento no hacemos nada bloqueante.

  return true;
}




void tareaSensor(void *parameter)
{
  sensors_event_t a, g, temp;

  while (1)
  {
    mpu.getEvent(&a, &g, &temp);

    ax = a.acceleration.x;
    ay = a.acceleration.y;
    az = a.acceleration.z;

    // La tarea queda bloqueada durante 100 ms
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}


void tareaUART(void *parameter)
{
  while (1)
  {
    Serial.print("Accel X: ");
    Serial.print(ax);

    Serial.print(" Y: ");
    Serial.print(ay);

    Serial.print(" Z: ");
    Serial.println(az);


    // Encender LED cada vez que enviamos
    digitalWrite(LED, HIGH);

    // Esperar 200 ms
    vTaskDelay(pdMS_TO_TICKS(200));

    // Apagar LED
    digitalWrite(LED, LOW);


    // Ya hemos esperado 200 ms.
    // Esperamos otros 800 ms para completar 1 segundo.
    vTaskDelay(pdMS_TO_TICKS(800));
  }
}


void setup()
{
  Serial.begin(9600);

  Wire.begin(21, 22);

  if (!mpu.begin())
  {
    Serial.println("No se encuentra el MPU6050");

    while (1);
  }

  Serial.println("MPU6050 iniciado");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);


  // Registrar nuestro Idle Hook
  esp_register_freertos_idle_hook(funcionIdle);


  // Crear tarea del sensor
  xTaskCreate(
    tareaSensor,
    "Sensor",
    2048,
    NULL,
    1,
    NULL
  );


  // Crear tarea UART + LED
  xTaskCreate(
    tareaUART,
    "UART",
    2048,
    NULL,
    1,
    NULL
  );
}


void loop()
{
  // No necesitamos hacer nada aquí
  vTaskDelay(pdMS_TO_TICKS(1000));
}