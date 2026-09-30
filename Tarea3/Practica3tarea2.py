import serial

ser = serial.Serial("COM6", 9600, timeout=1)

print("Leyendo COM6...")

while True:
    linea = ser.readline().decode("utf-8", errors="ignore").strip()

    if linea:
        print(linea)