# Práctica IoT – Arduino Nano 33 BLE Sense

## Descripción

En esta práctica se trabaja con la placa **Arduino Nano 33 BLE Sense** y distintos periféricos del microcontrolador, incluyendo:

- Conversor analógico-digital (ADC).
- Temporizadores hardware.
- Generación de señales PWM.
- Comunicación serie UART/USB.
- Comunicación I2C entre dos placas.
- Lectura de la IMU integrada:
  - Acelerómetro.
  - Giróscopo.
  - Magnetómetro.

La práctica se divide en varias tareas independientes que permiten comprobar progresivamente el funcionamiento de estos periféricos.

---

## Material utilizado

- Arduino Nano 33 BLE Sense.
- Arduino Nano 33 IoT.
- ESP32.
- Potenciómetro.
- LED externo.
- Resistencias.
- Osciloscopio.
- Cables de conexión.
- Arduino IDE.

---

## Librerías utilizadas

Dependiendo de la tarea se utilizan las siguientes librerías:

```cpp
#include "BBTimer.hpp"
#include "mbed.h"
#include <Wire.h>
#include <Arduino_LSM9DS1.h>
```

### Función de cada librería

- **BBTimer.hpp**: permite utilizar los temporizadores hardware de la placa.
- **mbed.h**: se utiliza para configurar directamente una salida PWM mediante `PwmOut`.
- **Wire.h**: permite realizar comunicaciones mediante el bus I2C.
- **Arduino_LSM9DS1.h**: permite acceder al acelerómetro, giróscopo y magnetómetro de la IMU LSM9DS1 integrada en el Nano 33 BLE Sense.

---

# Tarea 1 – Lectura periódica del ADC

**Archivo:** `Tarea1.ino`

Se realiza una lectura del ADC conectado al pin `A0` cada segundo.

```cpp
valorADC = analogRead(A0);
```

El valor obtenido se envía por el puerto serie a **9600 baudios**.

El objetivo es comprobar el funcionamiento básico del conversor analógico-digital utilizando, por ejemplo, un potenciómetro conectado entre `3V3`, `GND` y `A0`.

---

# Tarea 2 – Lectura del ADC mediante Timer

**Archivo:** `Tarea2.ino`

En esta tarea se utiliza un temporizador hardware para generar un evento cada **10 segundos**.

Se utiliza:

```cpp
BBTimer time0(BB_TIMER0);
```

El temporizador ejecuta la función:

```cpp
void leerADC() {
    leer = true;
}
```

La interrupción únicamente activa una bandera. La lectura del ADC y la transmisión serie se realizan posteriormente dentro de `loop()`.

Esto evita ejecutar operaciones relativamente lentas, como `Serial.println()`, directamente dentro de la interrupción.

---

# Tarea 3 – Generación de PWM proporcional al ADC

**Archivo:** `Tarea3.ino`

Se genera una señal PWM de:

```text
5 kHz
```

en el pin:

```text
D10
```

El ciclo de trabajo depende de la tensión leída mediante el ADC.

Para reducir las variaciones de la medida se realizan **20 lecturas** y se calcula su valor medio.

Posteriormente, el valor ADC se convierte del rango:

```text
0 – 1023
```

al rango:

```text
0.0 – 1.0
```

utilizado por `PwmOut`.

```cpp
float duty = valorADC / 1023.0;
pwmPin.write(duty);
```

El valor del ADC y el duty cycle se muestran también por el puerto serie.

La señal PWM y la tensión analógica pueden comprobarse simultáneamente con un osciloscopio.

---

# Tarea 4 – Control mediante comandos serie

**Archivo:** `Tarea4.ino`

Se implementa un pequeño protocolo de comandos utilizando objetos `String`.

Los comandos disponibles son:

## `ADC`

Realiza una única lectura del ADC y la envía por el puerto serie.

Ejemplo:

```text
ADC
```

---

## `ADC(x)`

Realiza lecturas periódicas del ADC cada `x` segundos mediante un temporizador hardware.

Ejemplo:

```text
ADC(2)
```

realiza una lectura cada 2 segundos.

Para detener las lecturas:

```text
ADC(0)
```

---

## `PWM(x)`

Permite modificar el ciclo de trabajo de la señal PWM.

El parámetro `x` puede tomar valores entre:

```text
0 – 9
```

Por ejemplo:

```text
PWM(0)
```

produce aproximadamente un 0 % de duty cycle.

```text
PWM(9)
```

produce aproximadamente un 100 % de duty cycle.

La conversión utilizada es:

```cpp
pwmPin.write(duty / 9.0);
```

---

# Tarea 5 – Comunicación I2C entre dos placas

**Archivos:**

- `Codigo_nano(1).ino`
- `Codigo_esp(1).ino`

Se establece una comunicación I2C entre dos placas.

## Nano – Maestro

El Nano actúa como maestro I2C:

```cpp
Wire.begin();
```

Envía alternativamente:

```text
1
0
```

cada segundo.

El esclavo utiliza la dirección:

```text
0x08
```

---

## ESP32 – Esclavo

El ESP32 se configura como esclavo I2C con dirección 8:

```cpp
Wire.begin(8, SDA_PIN, SCL_PIN, 100000);
```

Cuando recibe un dato se ejecuta automáticamente la callback:

```cpp
Wire.onReceive(recibirDato);
```

El comportamiento es:

- `1` → LED encendido.
- `0` → LED apagado.

De esta forma se comprueba una comunicación sencilla entre dos dispositivos mediante I2C.

---

# Tarea 6 – Lectura de la IMU

**Archivo:** `Tarea6.ino`

Se utiliza la IMU **LSM9DS1** integrada en el Arduino Nano 33 BLE Sense.

Primero se inicializa mediante:

```cpp
IMU.begin();
```

Se obtienen datos de:

- Acelerómetro.
- Giróscopo.
- Magnetómetro.

Los sensores se consultan cada:

```text
100 ms
```

utilizando `millis()`.

Ejemplo para el acelerómetro:

```cpp
if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
}
```

Los últimos valores almacenados se muestran por el puerto serie cada:

```text
1 segundo
```

Esto permite separar la frecuencia de adquisición de la frecuencia de envío.

---

# Tarea 7 – IMU + comunicación I2C

**Archivos:**

- `Codigo_nano_33_ble(1).ino`
- `Codigo_nano_33_iot(1).ino`

Esta tarea combina la adquisición de la IMU con la comunicación I2C.

---

## Nano 33 BLE Sense – Maestro

El Nano 33 BLE Sense espera a recibir por el puerto serie el comando:

```text
MEDIR
```

Cuando se recibe:

1. Se toman **5 muestras**.
2. Cada muestra está separada **200 ms**.
3. El proceso de adquisición dura aproximadamente **1 segundo**.
4. Se almacenan los datos de:
   - Acelerómetro.
   - Giróscopo.
   - Magnetómetro.
5. Al terminar la adquisición se envían las muestras por I2C.

Cada transmisión contiene:

```text
1 byte  → número de muestra
1 byte  → tipo de sensor
4 bytes → eje X
4 bytes → eje Y
4 bytes → eje Z
```

En total:

```text
14 bytes
```

El tipo de sensor se identifica mediante:

```text
0 → Acelerómetro
1 → Giróscopo
2 → Magnetómetro
```

Los valores `float` se transmiten como una secuencia de bytes:

```cpp
Wire.write((byte*)&x, sizeof(float));
```

---

## Nano 33 IoT – Esclavo

El Nano 33 IoT funciona como esclavo con dirección:

```text
0x08
```

La recepción se gestiona mediante:

```cpp
Wire.onReceive(recibirDatos);
```

En la función de recepción se reconstruyen los valores `float` enviados por el maestro:

```cpp
Wire.readBytes((byte*)&rx, sizeof(float));
```

Cuando se reciben los datos del magnetómetro significa que ya se dispone de los datos de los tres sensores de esa muestra.

Entonces se muestran por consola:

- Número de muestra.
- Aceleración X, Y y Z.
- Velocidad angular X, Y y Z.
- Campo magnético X, Y y Z.

Además, al recibir datos se enciende el LED integrado durante aproximadamente **1 segundo**.

---

# Conexiones I2C

Para realizar la comunicación entre dos placas deben conectarse:

```text
SDA ↔ SDA
SCL ↔ SCL
GND ↔ GND
```

Es imprescindible compartir la referencia de masa entre ambas placas.

La dirección I2C utilizada en los programas es:

```text
0x08
```

---

# Configuración del puerto serie

Los programas utilizan:

```cpp
Serial.begin(9600);
```

Por tanto, el monitor serie debe configurarse inicialmente a:

```text
9600 baudios
```

Para los comandos de la tarea 4 y la tarea 7 se recomienda configurar el terminal para enviar un salto de línea al finalizar el comando, ya que los programas utilizan:

```cpp
Serial.readStringUntil('\n');
```

---

# Comprobaciones principales

Durante la práctica se pueden comprobar los siguientes puntos:

1. Lectura correcta del ADC.
2. Ejecución periódica mediante temporizador hardware.
3. Señal PWM de 5 kHz y variación de su duty cycle.
4. Control mediante comandos recibidos por puerto serie.
5. Comunicación I2C maestro-esclavo.
6. Lectura del acelerómetro, giróscopo y magnetómetro.
7. Transmisión de varias muestras de sensores mediante I2C.
8. Reconstrucción de datos `float` recibidos como bytes.

---

# Archivos del proyecto

```text
Tarea1.ino
Tarea2.ino
Tarea3.ino
Tarea4.ino
Codigo_nano(1).ino
Codigo_esp(1).ino
Tarea6.ino
Codigo_nano_33_ble(1).ino
Codigo_nano_33_iot(1).ino
README.md
```

---

## Autor

Práctica realizada para la asignatura **Laboratorio de Comunicaciones IoT** del Máster en Ingeniería Electrónica.
