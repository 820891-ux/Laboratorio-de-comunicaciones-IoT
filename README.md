# Práctica 3 — Comunicaciones WiFi y pila IP con ESP32

**Asignatura:** Laboratorio de Comunicaciones IoT  
**Máster:** Ingeniería Electrónica

## Descripción

En esta práctica se utilizan las comunicaciones WiFi del **ESP32** para intercambiar información con un ordenador, ofrecer una página web y comunicarse con servicios externos. Se trabajan los protocolos **TCP, NTP, HTTP, FTP y MQTT**, junto con el formato **JSON/SenML**.

Este repositorio contiene los programas Arduino de las **tareas 4 a 10**. Las tareas 1 a 3 sirven de preparación (conectividad WiFi, sincronización horaria mediante NTP y comunicación entre sockets), pero no se incluyen como archivos independientes.

## Material y requisitos

- Placa de desarrollo **ESP32** y cable USB.
- **Arduino IDE** con el paquete de placas *ESP32 by Espressif Systems*.
- Red WiFi a la que puedan conectarse los dispositivos implicados.
- **SocketTest** u otro servidor TCP para las tareas 4, 5 y 6.
- Navegador web para la tarea 7.
- Servidor **FTP** accesible desde el ESP32 para la tarea 9 (por ejemplo, FileZilla Server).
- Cuenta y *feed* en **Adafruit IO** para la tarea 10.
- Monitor serie a **115200 baudios**.

### Librerías

| Librería | Uso | Tarea |
| --- | --- | --- |
| `WiFi.h` | Conexión a la red y cliente TCP | 4–9 |
| `time.h` | Sincronización NTP y marcas temporales | 4, 5, 8, 9 |
| `WebServer.h` | Servidor HTTP integrado | 7 |
| `SPIFFS.h` | Almacenamiento de archivos en la flash | 8 |
| `ArduinoJson.h` | Generación de documentos JSON | 8 |
| `ESP32_FTPClient.h` | Transferencia de archivos mediante FTP | 9 |
| `AdafruitIO_WiFi.h` | Envío y recepción de datos con Adafruit IO | 10 |

Las librerías del núcleo ESP32 se instalan con el paquete de placas. Las bibliotecas **ArduinoJson**, **ESP32 FTPClient** y **Adafruit IO Arduino** pueden requerir instalación desde el gestor de librerías o desde sus repositorios.

## Estructura del repositorio

```text
.
├── Practica3tarea4.ino
├── Practica3tarea5.ino
├── Practica3tarea6.ino
├── Practica3tarea7.ino
├── Practica3tarea8.ino
├── Practica3tarea9.ino
├── Practica3tarea10.ino
└── README.md
```

## Tareas realizadas

### Tarea 4 — Envío de la hora local mediante TCP

**Archivo:** [`Practica3tarea4.ino`](Practica3tarea4.ino)

El ESP32 se conecta a la red WiFi, sincroniza su reloj con el servidor NTP `pool.ntp.org` y establece una conexión como **cliente TCP** con un servidor ejecutado en el ordenador.

Una vez conectado, envía la **hora local española** cada segundo en formato `HH:MM:SS` y también la muestra por el monitor serie. Si se pierde la conexión TCP, intenta recuperarla.

**Prueba:** iniciar un servidor TCP en el PC, ajustar su IP y puerto en el programa y verificar la recepción de una línea por segundo.

### Tarea 5 — Control del envío mediante comandos TCP

**Archivo:** [`Practica3tarea5.ino`](Practica3tarea5.ino)

Se amplía el cliente TCP anterior para aceptar comandos enviados desde el servidor:

| Comando | Funcionamiento |
| --- | --- |
| `start` | Activa el envío de la fecha y hora cada segundo. |
| `stop` | Detiene los envíos periódicos. |

Los comandos se procesan al recibir un salto de línea (`\n` o `\r`). Los datos se envían con formato `AAAA-MM-DD HH:MM:SS`. En caso de desconexión, el ESP32 intenta reconectarse y deja el envío desactivado hasta recibir otro `start`.

**Prueba:** desde SocketTest, enviar `start` y comprobar la recepción de mensajes periódicos; después enviar `stop` y verificar que cesan.

### Tarea 6 — Envío de datos simulados de aceleración por TCP

**Archivo:** [`Practica3tarea6.ino`](Practica3tarea6.ino)

El ESP32 genera **valores aleatorios que simulan** las componentes `ax`, `ay` y `az` de un acelerómetro, y los envía cada segundo a un servidor TCP. Se utiliza una línea de texto con los tres valores separados por comas:

```text
0.23,-0.15,0.87
```

Este programa permite comprobar la comunicación TCP sin conectar un sensor físico. **No realiza una lectura real de un acelerómetro**. El enunciado también solicita recibir y representar los datos mediante Python, pero el script Python no forma parte de los archivos incluidos.

### Tarea 7 — Servidor web con reloj reiniciable

**Archivo:** [`Practica3tarea7.ino`](Practica3tarea7.ino)

El ESP32 implementa un **servidor HTTP en el puerto 80**, accesible desde un navegador de la misma red WiFi.

- La ruta `/` muestra un contador en formato `HH:MM:SS`.
- La página se recarga automáticamente cada segundo.
- Un botón envía una petición `POST` a `/reset` para devolver el contador a `00:00:00`.
- Tras el reinicio, el servidor redirige de nuevo a la página principal.

**Prueba:** introducir en el navegador la dirección IP mostrada en el monitor serie y pulsar **Resetear hora**.

> El valor mostrado es el **tiempo transcurrido desde el inicio o último reinicio**, calculado con `millis()`, no la hora real obtenida por NTP.

### Tarea 8 — Generación de archivos JSON con SenML

**Archivo:** [`Practica3tarea8.ino`](Practica3tarea8.ino)

El ESP32 sincroniza su reloj mediante NTP y, cada **10 segundos**, genera una medida de temperatura simulada en un documento JSON con campos de tipo SenML:

- `n`: nombre de la magnitud (`temperatura`).
- `u`: unidad (`Cel`, grados Celsius).
- `v`: valor de temperatura generado aleatoriamente.
- `t`: marca temporal en segundos Unix.

Ejemplo ilustrativo de la estructura:

```json
[
  {
    "n": "temperatura",
    "u": "Cel",
    "v": 25.4,
    "t": 1790000000
  }
]
```

El archivo se almacena en la memoria flash mediante **SPIFFS**, bajo la ruta `/temperatura.json`, y su contenido se muestra en el monitor serie.

**Importante:** en cada iteración se **sobrescribe** el mismo archivo; no se acumula un histórico.

### Tarea 9 — Subida de archivos JSON a un servidor FTP

**Archivo:** [`Practica3tarea9.ino`](Practica3tarea9.ino)

El ESP32 genera una temperatura aleatoria y una marca temporal NTP, construye una cadena JSON con campos SenML y la envía a un **servidor FTP** cada 10 segundos mediante `ESP32_FTPClient`.

El nombre del archivo sigue una estructura como:

```text
grupoXX_130530.json
```

El programa abre la conexión FTP, crea el archivo remoto, escribe el JSON y cierra la conexión. El JSON se **genera en memoria** para esta tarea; no se recupera el archivo SPIFFS de la tarea 8.

**Prueba:** configurar el servidor FTP, comprobar la conexión y verificar desde el servidor que llegan los archivos `.json` y contienen las medidas.

> En la versión incluida, las tres parejas de dígitos del nombre corresponden a **día, minuto y segundo**. Si el formato exigido requiere día, mes y segundo, debe ajustarse el campo central.

### Tarea 10 — Comunicación con Adafruit IO mediante MQTT

**Archivo:** [`Practica3tarea10.ino`](Practica3tarea10.ino)

El ESP32 se conecta a **Adafruit IO** y utiliza el *feed* `temperatura` para enviar y recibir datos.

- Establece la conexión WiFi y con Adafruit IO.
- Se suscribe a las actualizaciones del feed mediante `onMessage()`.
- Consulta el último valor almacenado mediante `get()`.
- Publica una temperatura aleatoria cada **10 segundos** con `save()`.
- Mantiene la conexión y procesa los mensajes entrantes mediante `io.run()`.

**Prueba:** modificar el valor del feed desde la web de Adafruit IO y comprobar que aparece como `Dato recibido` en el monitor serie. Verificar también que los valores enviados desde el ESP32 se muestran en el feed.

## Puesta en marcha

1. Abrir en Arduino IDE el archivo de la tarea que se desea probar.
2. Seleccionar la placa ESP32 y el puerto serie correspondiente.
3. Configurar los datos necesarios en el programa: **SSID, contraseña WiFi, IP y puerto TCP, servidor FTP o credenciales de Adafruit IO**, según la tarea.
4. Cargar el programa en la placa.
5. Abrir el monitor serie a **115200 baudios**.
6. Preparar la aplicación externa correspondiente (SocketTest, navegador, FTP o Adafruit IO) y verificar el intercambio de datos.

### Seguridad de las credenciales

Los archivos `.ino` contienen campos de configuración de red y, en algunos casos, credenciales de servicios. **No se deben publicar contraseñas ni claves de API en un repositorio público.** Antes de subir los archivos a GitHub, sustituir las credenciales reales por valores de ejemplo o moverlas a un archivo local excluido mediante `.gitignore`. Si alguna contraseña real ya se ha publicado, conviene cambiarla.

## Resumen

| Tarea | Tecnología | Resultado |
| --- | --- | --- |
| 4 | WiFi + NTP + TCP | Envío periódico de la hora local al ordenador. |
| 5 | TCP | Inicio y detención del envío mediante `start` y `stop`. |
| 6 | TCP | Transmisión de tres componentes de aceleración simuladas. |
| 7 | HTTP | Página web con contador y botón de reinicio. |
| 8 | JSON + SenML + SPIFFS | Creación periódica de un archivo de temperatura con marca temporal. |
| 9 | FTP | Subida periódica de archivos JSON al servidor. |
| 10 | MQTT + Adafruit IO | Publicación de temperaturas y recepción de cambios del feed. |

**Resultado de la práctica:** familiarización con la comunicación WiFi del ESP32 y con distintos protocolos para el intercambio local y remoto de información IoT.
