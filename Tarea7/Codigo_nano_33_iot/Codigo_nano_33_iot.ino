```cpp
#include <Wire.h>

// Dirección I2C utilizada por el esclavo
#define DIRECCION_ESCLAVO 0x08

// Variables para guardar los datos de los tres sensores
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;

// Bandera que indica si se han recibido nuevos datos
volatile bool datosRecibidos = false;

// Variables para identificar la muestra y el sensor recibido
byte muestraRecibida;
byte sensorRecibido;

// Variables temporales donde se guardan los datos recibidos
float rx;
float ry;
float rz;

// Variables para controlar el tiempo que permanece encendido el LED
unsigned long tiempoLED = 0;
bool ledEncendido = false;


void setup() {

  // Iniciamos la comunicación serie
  Serial.begin(9600);

  // Configuramos el LED integrado como salida
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Iniciamos I2C como esclavo con dirección 0x08
  Wire.begin(DIRECCION_ESCLAVO);

  // Indicamos la función que se ejecuta al recibir datos
  Wire.onReceive(recibirDatos);

  Serial.println("Esclavo preparado");
}


void loop() {

  // Si hemos recibido nuevos datos
  if (datosRecibidos) {

    datosRecibidos = false;

    // Si el sensor recibido es el acelerómetro
    if (sensorRecibido == 0) {

      ax = rx;
      ay = ry;
      az = rz;
    }

    // Si el sensor recibido es el giroscopio
    else if (sensorRecibido == 1) {

      gx = rx;
      gy = ry;
      gz = rz;
    }

    // Si el sensor recibido es el magnetómetro
    else if (sensorRecibido == 2) {

      mx = rx;
      my = ry;
      mz = rz;

      // Como el magnetómetro se envía el último,
      // en este punto ya tenemos los datos de los tres sensores

      Serial.print("Muestra ");
      Serial.println(muestraRecibida + 1);

      Serial.print("Acelerometro: ");
      Serial.print(ax);
      Serial.print(" ");
      Serial.print(ay);
      Serial.print(" ");
      Serial.println(az);

      Serial.print("Giroscopio: ");
      Serial.print(gx);
      Serial.print(" ");
      Serial.print(gy);
      Serial.print(" ");
      Serial.println(gz);

      Serial.print("Magnetometro: ");
      Serial.print(mx);
      Serial.print(" ");
      Serial.print(my);
      Serial.print(" ");
      Serial.println(mz);

      Serial.println("----------------------");
    }

    // Encendemos el LED al recibir datos
    digitalWrite(LED_BUILTIN, HIGH);

    // Guardamos el instante en el que se ha encendido
    tiempoLED = millis();
    ledEncendido = true;
  }


  // Apagamos el LED cuando haya pasado 1 segundo
  if (ledEncendido && millis() - tiempoLED >= 1000) {

    digitalWrite(LED_BUILTIN, LOW);
    ledEncendido = false;
  }
}


// Función que se ejecuta automáticamente al recibir datos por I2C
void recibirDatos(int numeroBytes) {

  // Esperamos recibir:
  // 1 byte de muestra + 1 byte de sensor + 3 float de 4 bytes = 14 bytes
  if (numeroBytes >= 14) {

    // Leemos el número de muestra
    muestraRecibida = Wire.read();

    // Leemos el tipo de sensor
    sensorRecibido = Wire.read();

    // Reconstruimos los tres valores float recibidos
    Wire.readBytes((byte*)&rx, sizeof(float));
    Wire.readBytes((byte*)&ry, sizeof(float));
    Wire.readBytes((byte*)&rz, sizeof(float));

    // Indicamos al loop que hay nuevos datos
    datosRecibidos = true;
  }
}
```
