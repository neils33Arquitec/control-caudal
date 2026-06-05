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

## Instalación

1. Abrir `control-caudal.ino` en Arduino IDE.
2. Instalar librerías: `LiquidCrystal_I2C` (vía Manager de librerías).
3. Conectar hardware según `hardware/schematics/wiring-diagram.md`.
4. Seleccionar placa **Arduino Uno** y puerto COM.
5. Cargar el sketch.

## Estructura del repositorio

```
├── control-caudal.ino   Sketch principal de Arduino
├── src/           Librerías y configuración
├── hardware/      Esquemáticos, BOM, datasheets
├── scripts/       MATLAB, Python, Simulink
├── test/          Pruebas unitarias y de validación
├── docs/          Documentación técnica
├── assets/        Diagramas, fotos, gráficos
└── .github/       CI y templates
```

## Autores

- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

**Universidad Santiago Mariño** — Teoría Moderna de Control — Junio 2026
