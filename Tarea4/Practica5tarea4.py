import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import time

# Abrimos el puerto serie
ser = serial.Serial("COM6", 9600, timeout=1)

# Listas para acumular datos durante 5 segundos
ax_datos = []
ay_datos = []
az_datos = []

# Listas para guardar las medias de cada bloque de 5 segundos
medias_ax = []
medias_ay = []
medias_az = []

# Listas para guardar las desviaciones estándar
desv_ax = []
desv_ay = []
desv_az = []

# Momento en el que empieza el primer bloque
inicio_bloque = time.time()


def actualizar(frame):

    global inicio_bloque

    # Leer todos los datos disponibles del puerto serie
    while ser.in_waiting > 0:

        linea = ser.readline().decode("utf-8", errors="ignore").strip()

        if not linea:
            continue

        try:
            # Esperamos recibir:
            # ax;ay;az;gx;gy;gz;mx;my;mz

            datos = linea.split(";")

            ax = float(datos[0])
            ay = float(datos[1])
            az = float(datos[2])

            # Guardamos los datos del acelerómetro
            ax_datos.append(ax)
            ay_datos.append(ay)
            az_datos.append(az)

        except:
            # Si llega una línea incorrecta, la ignoramos
            continue


    # Comprobamos si han pasado 5 segundos
    if time.time() - inicio_bloque >= 5:

        if len(ax_datos) > 0:

            # Calcular medias
            media_ax = np.mean(ax_datos)
            media_ay = np.mean(ay_datos)
            media_az = np.mean(az_datos)

            # Calcular desviaciones estándar
            std_ax = np.std(ax_datos)
            std_ay = np.std(ay_datos)
            std_az = np.std(az_datos)

            # Guardamos los resultados
            medias_ax.append(media_ax)
            medias_ay.append(media_ay)
            medias_az.append(media_az)

            desv_ax.append(std_ax)
            desv_ay.append(std_ay)
            desv_az.append(std_az)

            # Mostrar resultados por consola
            print("----- BLOQUE DE 5 SEGUNDOS -----")

            print("Media AX:", media_ax)
            print("Media AY:", media_ay)
            print("Media AZ:", media_az)

            print("Desviación AX:", std_ax)
            print("Desviación AY:", std_ay)
            print("Desviación AZ:", std_az)

            print()

            # Borrar las muestras del bloque anterior
            ax_datos.clear()
            ay_datos.clear()
            az_datos.clear()

            # Reiniciar el tiempo para el siguiente bloque
            inicio_bloque = time.time()

            # Actualizar la gráfica
            ax_grafica.clear()

            ax_grafica.plot(medias_ax, label="Media AX")
            ax_grafica.plot(medias_ay, label="Media AY")
            ax_grafica.plot(medias_az, label="Media AZ")

            ax_grafica.set_title("Media del acelerómetro cada 5 segundos")
            ax_grafica.set_xlabel("Bloques de 5 segundos")
            ax_grafica.set_ylabel("Aceleración")

            ax_grafica.legend()
            ax_grafica.grid()


# Crear la gráfica
fig, ax_grafica = plt.subplots()

# Ejecutar actualizar() cada 100 ms
ani = FuncAnimation(
    fig,
    actualizar,
    interval=100,
    cache_frame_data=False
)

# Mostrar la ventana
plt.show()

# Cerrar el puerto cuando cierres la gráfica
ser.close()