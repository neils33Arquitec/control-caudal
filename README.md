# Sistema Inteligente de Control de Caudal

Controlador **PI digital en lazo cerrado** para regular el flujo de agua mediante una bomba DC y sensor **GFS401**, implementado en **Arduino Uno**.

## Diagrama de bloques

```
SetPoint ──▶ [PI Discreto] ──▶ PWM ──▶ [L298N] ──▶ [Bomba 12V] ──▶ Flujo
   ▲                                                                  │
   └────────────────────── [GFS401] ◄─────────────────────────────────┘
```

## Hardware

| Componente | Especificación |
|---|---|
| Arduino Uno R3 | ATmega328P |
| Sensor GFS401 | 0.3–6 L/min, Hall, 5880 pulsos/L |
| Bomba DC | 12V, ~300–800 mA |
| Puente H L298N | Driver PWM, 2A por canal |
| LCD 16×2 + I2C | Dirección 0x27 |
| Potenciómetro | 10 kΩ, SetPoint 0–6 L/min |

## Pinout rápido

| Arduino | Conexión |
|---|---|
| Pin 2 | GFS401 señal (interrupción RISING) |
| Pin 9 | L298N ENA (PWM) |
| Pin 7 | L298N IN1 |
| Pin 8 | L298N IN2 |
| A0 | Potenciómetro (SetPoint) |
| A4 (SDA) | LCD I2C |
| A5 (SCL) | LCD I2C |

## Instalación (PlatformIO / arduino-cli)

1. Abrir `src/firmware/control-caudal.ino` desde tu editor.
2. Instalar librerías: `LiquidCrystal_I2C`.
3. Conectar hardware según `hardware/schematics/wiring-diagram.md`.
4. Seleccionar placa **Arduino Uno** y puerto COM.
5. Compilar y cargar.

## Programación con Arduino IDE

El proyecto mantiene los archivos fuente en `src/firmware/` como
estructura principal. Si prefieres usar **Arduino IDE** (que requiere
que el folder tenga el mismo nombre que el sketch), usa la carpeta
`ArduinoIDEcontrol-caudal/`:

1. Abre `ArduinoIDEcontrol-caudal/control-caudal.ino` desde Arduino IDE.
2. Verifica que `config.h`, `PIController.h` y `PIController.cpp` estén
   en la misma carpeta (ya están).
3. Instala la librería `LiquidCrystal_I2C` desde el Gestor de librerías.
4. Selecciona **Arduino Uno** como placa.
5. Haz clic en **Subir**.

> **Nota:** Si modificas archivos, replica los cambios también en
> `src/firmware/` para mantener sincronizada la estructura del repositorio.

## Estructura del repositorio

```
├── ArduinoIDEcontrol-caudal/   Copia plana para Arduino IDE
├── src/firmware/               Código fuente principal
├── hardware/                   Esquemáticos, BOM, datasheets
├── scripts/                    MATLAB, Python, Simulink
├── test/                       Pruebas unitarias y de validación
├── docs/                       Documentación técnica
├── sesiones/                   Bitácora de desarrollo
├── assets/                     Diagramas, fotos, gráficos
└── .github/                    CI y templates
```

## Autores

- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

**Universidad Santiago Mariño** — Teoría Moderna de Control — Junio 2026
