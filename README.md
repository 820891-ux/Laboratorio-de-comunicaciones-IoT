# Práctica 2 — Firmware con FreeRTOS en ESP32

**Asignatura:** Laboratorio de Comunicaciones IoT  
**Titulación:** Máster en Ingeniería Electrónica — Universidad de Zaragoza

## Descripción

En esta práctica se trabaja con un **ESP32** y un **sensor inercial MPU6050**, conectado mediante **I²C**, para adquirir las tres componentes de la aceleración y enviarlas al ordenador por **UART**. Cada envío se indica mediante el encendido de un LED durante 200 ms.

Se incluyen dos implementaciones:

- **Tarea 2 (`Practica2Tarea2.ino`):** lectura y envío periódico mediante temporización con `millis()`.
- **Tarea 3 (`Practica2Tarea3.ino`):** implementación mediante tareas de **FreeRTOS**, incorporando un *Idle Hook* como punto de partida para estudiar el bajo consumo.

## Objetivos

- Leer la aceleración en los ejes X, Y y Z del MPU6050 cada **100 ms**.
- Enviar los valores por el puerto serie cada **1 s**.
- Activar un LED durante **200 ms** al transmitir los datos.
- Utilizar las tareas y los mecanismos de temporización de FreeRTOS.
- Explorar el uso del *Idle Hook* para detectar los períodos en los que el sistema no tiene trabajo útil.

## Material necesario

- Placa de desarrollo **ESP32**.
- Sensor **MPU6050**.
- LED externo y resistencia limitadora de corriente adecuada.
- Cables de conexión y cable USB.
- Ordenador con **Arduino IDE**.

## Conexiones

| ESP32 | Componente | Función |
|---|---|---|
| GPIO 21 | SDA del MPU6050 | Datos I²C |
| GPIO 22 | SCL del MPU6050 | Reloj I²C |
| GPIO 23 | LED externo (con resistencia) | Señalización del envío |
| GND | GND del MPU6050 | Masa común |
| 3V3 | Alimentación del módulo MPU6050, si es compatible | Alimentación |

El LED se conecta de forma que se encienda cuando GPIO 23 está a nivel alto. Comprueba la tensión admitida por tu módulo MPU6050 antes de alimentarlo.

## Software y librerías

Los programas están desarrollados para el **framework Arduino sobre ESP32** y utilizan:

- `Wire.h`: comunicación I²C.
- `Adafruit_MPU6050.h`: manejo del sensor MPU6050.
- `Adafruit_Sensor.h`: estructuras de datos de los sensores.
- `esp_freertos_hooks.h`: registro del *Idle Hook* (solo en la tarea 3).

Instala desde el gestor de bibliotecas de Arduino IDE **Adafruit MPU6050** y **Adafruit Unified Sensor**, junto con sus dependencias. También es necesario tener instalado el soporte de placas **ESP32 de Espressif Systems**.

## Archivos del proyecto

```text
.
├── Practica2Tarea2.ino
├── Practica2Tarea3.ino
└── README.md
```

### Tarea 2 — Temporización con `millis()`

Archivo: [`Practica2Tarea2.ino`](Practica2Tarea2.ino)

El programa inicializa el MPU6050 y utiliza el tiempo transcurrido, obtenido con `millis()`, para realizar tres acciones sin utilizar `delay()`:

| Acción | Intervalo |
|---|---|
| Lectura de aceleración | Cada 100 ms |
| Envío por UART | Cada 1.000 ms |
| Apagado del LED tras cada envío | 200 ms después del encendido |

Los últimos valores leídos de los ejes X, Y y Z se envían por el puerto serie. Esta implementación **no crea tareas FreeRTOS explícitas**; organiza las acciones mediante comprobaciones periódicas dentro de `loop()`.

### Tarea 3 — Tareas FreeRTOS e *Idle Hook*

Archivo: [`Practica2Tarea3.ino`](Practica2Tarea3.ino)

En esta versión se crean dos tareas con `xTaskCreate()`:

| Tarea | Función | Temporización | Prioridad |
|---|---|---|---|
| `tareaSensor` | Actualiza la aceleración X, Y y Z | `vTaskDelay()` de 100 ms | 1 |
| `tareaUART` | Envía los datos y controla el LED | LED encendido 200 ms y espera adicional de 800 ms | 1 |

Ambas tareas se crean con una profundidad de pila de **2048 bytes** según el argumento utilizado en `xTaskCreate()`.

Además, se registra la función `funcionIdle()` mediante `esp_register_freertos_idle_hook()`. Esta función devuelve `true` sin realizar operaciones adicionales.

**Importante:** el código registra un *Idle Hook*, pero **no implementa por sí mismo la entrada en un modo de bajo consumo** ni demuestra una reducción del consumo eléctrico. Para completar ese objetivo opcional sería necesario implementar y comprobar una estrategia de ahorro energético compatible con el ESP32 y las tareas activas.

## Puesta en marcha

1. Realiza las conexiones del MPU6050 y el LED.
2. Instala el soporte ESP32 y las librerías indicadas.
3. Abre el archivo `.ino` que quieras probar con Arduino IDE.
4. Selecciona la placa ESP32 y el puerto serie correspondiente.
5. Compila y carga el programa.
6. Abre el **Monitor Serie a 9600 baudios**.
7. Comprueba que aparecen las lecturas de aceleración aproximadamente cada segundo y que el LED se enciende durante 200 ms en cada envío.

El mensaje de datos tiene este formato:

```text
Accel X: <ax> Y: <ay> Z: <az>
```

Los valores de aceleración proporcionados por la biblioteca Adafruit se expresan en **m/s²**.

Si el módulo no responde por I²C, el programa muestra `No se encuentra el MPU6050` y queda detenido.

## Diferencias entre implementaciones

| Característica | Tarea 2 | Tarea 3 |
|---|---|---|
| Lectura del sensor | `millis()` en `loop()` | `tareaSensor` |
| Envío serie y LED | `millis()` en `loop()` | `tareaUART` |
| Temporización | Comparación de tiempos | `vTaskDelay()` |
| Tareas FreeRTOS explícitas | No | Sí |
| *Idle Hook* registrado | No | Sí |
| Bajo consumo implementado y verificado | No | No |

## Observaciones

- Se envía **la lectura de aceleración más reciente**, no el conjunto de las diez muestras adquiridas durante cada segundo.
- En la tarea 3, las variables `ax`, `ay` y `az` son compartidas entre las dos tareas y no se utiliza un mecanismo explícito de sincronización. Para aplicaciones más exigentes podrían emplearse colas o mecanismos de protección de datos de FreeRTOS.
- La tarea 3 realiza esperas de 200 ms y 800 ms; por tanto, el período real también incluye el tiempo de ejecución del envío y de las instrucciones.
- La comprobación experimental del ahorro energético propuesta en el enunciado es **opcional** y no queda resuelta por el *Idle Hook* actual.

## Referencias

- [Documentación de FreeRTOS](https://www.freertos.org/Documentation/RTOS_book.html)
- [Arduino ESP32 — Espressif](https://github.com/espressif/arduino-esp32)
- [Librería Adafruit MPU6050](https://github.com/adafruit/Adafruit_MPU6050)
- [Manual técnico del ESP32](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)

---

*Práctica 2 de Laboratorio de Comunicaciones IoT: diseño de firmware basado en un sistema operativo de tiempo real (RTOS) con ESP32.*
