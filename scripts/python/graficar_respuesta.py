"""
graficar_respuesta.py
Grafica SP, PV y PWM desde un archivo CSV capturado por log_datos.py.
Uso: python graficar_respuesta.py <archivo.csv>
"""

import matplotlib.pyplot as plt
import pandas as pd
import sys

def main():
    if len(sys.argv) < 2:
        archivos = __import__("glob").glob("../assets/graficos/datos_*.csv")
        if not archivos:
            print("Usar: python graficar_respuesta.py <archivo.csv>")
            return
        archivo = archivos[-1]
        print(f"Usando último archivo: {archivo}")
    else:
        archivo = sys.argv[1]

    df = pd.read_csv(archivo)
    t = df["tiempo_ms"] / 1000

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 6), sharex=True)

    ax1.plot(t, df["SP"], "g--", label="SetPoint (L/min)", linewidth=1.5)
    ax1.plot(t, df["PV"], "b-", label="Caudal real (L/min)", linewidth=1.5)
    ax1.set_ylabel("Caudal (L/min)")
    ax1.legend()
    ax1.grid(True)

    ax2.plot(t, df["PWM"], "r-", label="PWM (0-255)", linewidth=1)
    ax2.set_xlabel("Tiempo (s)")
    ax2.set_ylabel("PWM")
    ax2.legend()
    ax2.grid(True)

    fig.suptitle("Respuesta del sistema de control de caudal")
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
