import serial

ser = serial.Serial("COM6", 9600, timeout=1)

with open("datos.txt", "w") as archivo:

    try:
        while True:

            linea = ser.readline().decode("utf-8", errors="ignore").strip()

            if linea:
                print(linea)
                linea = linea.replace(".", ",")
                archivo.write(linea + "\n")
                archivo.flush()

    except KeyboardInterrupt:
        pass

ser.close()