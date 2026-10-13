# Práctica 4 — Comunicaciones BLE y Bluetooth Classic

**Asignatura:** Laboratorio de Comunicaciones IoT  
**Máster en Ingeniería Electrónica**

## Descripción

En esta práctica se implementan comunicaciones inalámbricas utilizando **Bluetooth Low Energy (BLE)** y **Bluetooth Classic**. Las tareas 1 a 4 se realizan con un **Arduino Nano 33 BLE Sense**, mientras que la tarea 5 utiliza un **ESP32 compatible con Bluetooth Classic**.

Se trabaja con el *advertising* BLE, servicios y características **GATT**, lectura y notificaciones del acelerómetro, control remoto de la IMU y comunicación serie bidireccional mediante Bluetooth Classic.

## Material y software

- Arduino Nano 33 BLE Sense con IMU **LSM9DS1** (versión original de la placa).
- ESP32 con soporte para **Bluetooth Classic** (por ejemplo, ESP32-WROOM-32; **no** ESP32-S3).
- Ordenador con **Arduino IDE** y conexión USB a las placas.
- Móvil Android con las aplicaciones **nRF Connect** y **Serial Bluetooth Terminal**.
- Librerías de Arduino: `ArduinoBLE`, `Arduino_LSM9DS1` y `Wire`.
- Para la tarea 5: `BluetoothSerial`, incluida en el paquete de placas ESP32 de Espressif.

> **Nota:** Los programas que utilizan `Arduino_LSM9DS1` y la dirección I2C `0x6B` están preparados para la versión del Nano 33 BLE Sense que incorpora el sensor LSM9DS1. La revisión Rev2 utiliza sensores diferentes.

## Archivos

| Archivo | Contenido | Placa |
| --- | --- | --- |
| [`Practica4tarea1.ino`](Practica4tarea1.ino) | Advertising e identificación del dispositivo BLE | Nano 33 BLE Sense |
| [`Practica4tarea2.ino`](Practica4tarea2.ino) | Control del LED RGB desde una característica GATT | Nano 33 BLE Sense |
| [`Practica4tarea3.ino`](Practica4tarea3.ino) | Lectura y notificaciones de los ejes del acelerómetro | Nano 33 BLE Sense |
| [`Practica4tarea4.ino`](Practica4tarea4.ino) | Activación y desactivación remota del acelerómetro | Nano 33 BLE Sense |
| [`Practica4tarea5.ino`](Practica4tarea5.ino) | Chat bidireccional mediante Bluetooth Classic | ESP32 |

## Desarrollo de las tareas

### Tarea 1. Advertising BLE

El Nano 33 BLE Sense se anuncia mediante BLE con el nombre de dispositivo **`820891`**. Se crea un servicio identificado mediante un UUID y se inicia el *advertising* para que el móvil pueda descubrirlo con nRF Connect.

**Comprobación:** buscar `820891` en nRF Connect y verificar que aparece el servicio anunciado.

> En este primer archivo, el UUID `416c756d-6e6f-0000-0000-000000000001` contiene la palabra **«Alumno»** codificada en hexadecimal; el identificador `820891` se utiliza como nombre visible del dispositivo.

### Tarea 2. Control del LED RGB mediante GATT

Se añade una característica de tipo `BLEByteCharacteristic` con permisos **`BLERead | BLEWrite`**. El móvil escribe un byte para seleccionar el color del LED RGB integrado en la placa.

| Valor decimal (`UINT8`) | Resultado |
| --- | --- |
| `0` | LED apagado |
| `1` | Rojo |
| `2` | Verde |
| `3` | Azul |
| `4` | Blanco |

Los LED RGB del Nano 33 BLE Sense son **activos a nivel bajo**, por lo que `LOW` los enciende y `HIGH` los apaga.

**Comprobación:** conectarse desde nRF Connect, escribir los valores en la característica terminada en `0002` y observar el cambio de color.

### Tarea 3. Lectura del acelerómetro y notificaciones BLE

Se utiliza la IMU **LSM9DS1** para leer las aceleraciones en los ejes **X, Y y Z** cada **100 ms**. Los valores, expresados en **g**, se convierten a una cadena de texto y se publican en una característica GATT con permisos **`BLERead | BLENotify`**, sin permiso de escritura.

Ejemplo de dato transmitido:

```text
0.02,-0.04,1.01
```

**Comprobación:** activar las notificaciones de la característica terminada en `0003` en nRF Connect. Los valores deben actualizarse automáticamente al mover la placa.

### Tarea 4. Control remoto del acelerómetro

Se incorpora una segunda característica de tipo `BLEByteCharacteristic` con permiso **`BLEWrite`**, identificada por el UUID terminado en `0004`.

| Comando decimal (`UINT8`) | Acción |
| --- | --- |
| `0` | Desactiva el acelerómetro y detiene las actualizaciones |
| `1` | Activa el acelerómetro y reanuda las actualizaciones |

El estado del sensor se modifica a través de **I2C**, escribiendo en el registro `CTRL_REG6_XL` (`0x20`) del LSM9DS1, con dirección `0x6B` en `Wire1`. Así se evita ejecutar `IMU.end()` y `IMU.begin()` en cada cambio, manteniendo el bus I2C inicializado.

**Comprobación:** activar las notificaciones de la característica `0003`, escribir `0` y después `1` en la característica `0004`, y verificar que se detienen y reanudan sin desconectar el móvil.

### Tarea 5. Chat con Bluetooth Classic

Se utiliza `BluetoothSerial` en el ESP32 para establecer una comunicación serie bidireccional mediante **Bluetooth Classic (SPP)**. El programa habilita el emparejamiento **SSP** e identifica el dispositivo como **`ESP32_820891`**.

- Los caracteres escritos en el **monitor serie** del ordenador se envían al móvil.
- Los caracteres recibidos desde el **móvil** se muestran en el monitor serie.

**Comprobación:** emparejar el móvil con `ESP32_820891`, conectarse desde Serial Bluetooth Terminal y enviar mensajes en ambos sentidos.

## Cómo ejecutar los programas

1. Abrir el archivo `.ino` correspondiente en **Arduino IDE**.
2. Seleccionar la **placa** y el **puerto USB** adecuados e instalar las librerías necesarias.
3. Compilar y cargar el programa.
4. Abrir el **monitor serie a 115200 baudios** para consultar los mensajes de depuración.
5. Para las tareas **1–4**, conectarse desde **nRF Connect** al dispositivo `820891` y utilizar los servicios y características indicados.
6. Para la tarea **5**, utilizar un ESP32 compatible con Bluetooth Classic y conectarse con **Serial Bluetooth Terminal** al dispositivo `ESP32_820891`.

**Importante:** al cambiar el programa GATT entre las tareas 2, 3 y 4, si nRF Connect muestra características antiguas, es recomendable desconectarse y **redescubrir o actualizar los servicios**.

## Hitos evaluables

- **Tarea 1:** mostrar en nRF Connect el dispositivo anunciado con su nombre.
- **Tarea 4:** comprobar la lectura y las notificaciones automáticas del acelerómetro, verificar que la característica de datos no admite escrituras y demostrar su activación/desactivación.
- **Tarea 5:** enviar y recibir mensajes entre el móvil y el ESP32 mediante Bluetooth Classic.

## Referencias

- [Documentación de ArduinoBLE](https://docs.arduino.cc/libraries/arduinoble/)
- [Documentación de Arduino_LSM9DS1](https://docs.arduino.cc/libraries/arduino_lsm9ds1/)
- [Ejemplos de BluetoothSerial para ESP32](https://github.com/espressif/arduino-esp32/tree/master/libraries/BluetoothSerial/examples)
- [nRF Connect para Android](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp)
- [Serial Bluetooth Terminal para Android](https://play.google.com/store/apps/details?id=de.kai_morich.serial_bluetooth_terminal)
