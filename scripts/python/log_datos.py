"""
log_datos.py
Captura datos seriales desde Arduino y los guarda en CSV.
Uso: python log_datos.py [puerto] [duracion_segundos]
"""

import serial
import time
import csv
import sys
import os

def main():
    puerto = sys.argv[1] if len(sys.argv) > 1 else "COM3"
    duracion = int(sys.argv[2]) if len(sys.argv) > 2 else 30

    timestamp = time.strftime("%Y%m%d_%H%M%S")
    archivo = os.path.join("..", "assets", "graficos", f"datos_{timestamp}.csv")

    os.makedirs(os.path.dirname(archivo), exist_ok=True)

    ser = serial.Serial(puerto, 9600, timeout=1)
    time.sleep(2)

    print(f"Conectado a {puerto}")
    print(f"Guardando en {archivo} por {duracion} s...")

    with open(archivo, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["tiempo_ms", "SP", "PV", "Error", "PWM"])
        inicio = time.time()

        while time.time() - inicio < duracion:
            linea = ser.readline().decode("utf-8").strip()
            if not linea:
                continue
            try:
                partes = {p.split(":")[0]: float(p.split(":")[1])
                          for p in linea.split(",")}
                t_ms = (time.time() - inicio) * 1000
                writer.writerow([
                    t_ms,
                    partes.get("SP", 0),
                    partes.get("PV", 0),
                    partes.get("E", 0),
                    partes.get("PWM", 0)
                ])
                print(f"\r{t_ms:.0f} ms  SP={partes['SP']:.2f}  PV={partes['PV']:.2f}", end="")
            except (ValueError, KeyError):
                continue

    ser.close()
    print(f"\n\nDatos guardados: {archivo}")

if __name__ == "__main__":
    main()
