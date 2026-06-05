# Diagrama de Conexiones — Sistema de Control de Caudal

## Conexión general

```
                         ┌───────────────────────────────┐
                         │         ARDUINO UNO           │
                         │                               │
    ┌────────────────────┤         ┌─────┐               │
    │                    │  Pin 2  │ INT │               │
    │  ┌─────────────┐   │◄────────┤ 0   │               │
    │  │  GFS401     │   │         └─────┘               │
    │  │ (Sensor)    │   │                               │
    │  │             │   │  Pin 9 ───── L298N ENA ───────┤
    │  │             │   │  Pin 7 ───── L298N IN1 ───────┤
    │  │             │   │  Pin 8 ───── L298N IN2 ───────┤
    │  │ Rojo ─── 5V │   │                               │
    │  │ Negro ── GND│   │  A0 ───── Potenciómetro ──────┤
    │  │ Amarillo ───┼───┤           (SetPoint)        │
    │  └─────────────┘   │                               │
    │                    │                               │
    │                    │  A4 (SDA) ─── LCD I2C ──────┤  │
    │                    │  A5 (SCL) ─── LCD I2C ──────┤  │
    │                    │                              │  │
    │                    │  Vin ───── 12V ─── Fuente ───┤  │
    │                    │  GND ───── GND ─── Fuente ───┤  │
    │                    └──────────────────────────────┘  │
    │                                                      │
    └──────────────────────────────────────────────────────┘
                           │
                           ▼
                 ┌─────────────────┐
                 │     L298N       │
                 │  (Puente H)     │
                 │                 │
                 │  +12V ─── Fuente│
                 │  GND  ─── GND   │
                 │  ENA  ─── Pin 9 │
                 │  IN1  ─── Pin 7 │
                 │  IN2  ─── Pin 8 │
                 │                 │
                 │  OUT1 ─────┐    │
                 │  OUT2 ─────┤    │
                 └────────────┼────┘
                              │
                              ▼
                 ┌─────────────────┐
                 │   Bomba DC 12V  │
                 └─────────────────┘
                                   │
                                   ▼
                              ┌─────────┐
                              │ Tubería │
                              └────┬────┘
                                   │
                                   ▼
                          ┌─────────────────┐
                          │   GFS401        │
                          │ (Sensor flujo)  │
                          └─────────────────┘
                                   │
                              (retorno al Arduino)
```

## Tabla de conexiones detallada

### Arduino Uno → Periféricos

| Pin Arduino | Conectado a | Nota |
|---|---|---|
| **Pin 2** | Sensor GFS401 — Amarillo (OUT) | Interrupción 0 (RISING) |
| **Pin 9** | L298N — ENA (PWM) | PWM velocidad bomba |
| **Pin 7** | L298N — IN1 | Dirección bomba (HIGH) |
| **Pin 8** | L298N — IN2 | Dirección bomba (LOW) |
| **A0** | Potenciómetro — Wiper | SetPoint de caudal (0–6 L/min) |
| **A4 (SDA)** | LCD I2C — SDA | Bus I2C |
| **A5 (SCL)** | LCD I2C — SCL | Bus I2C |
| **5V** | GFS401 — Rojo (VCC), LCD I2C — VCC, Potenciómetro | Alimentación 5V |
| **GND** | GFS401 — Negro, LCD I2C — GND, Potenciómetro, L298N GND | Tierra común |
| **Vin** | Fuente 12V (positivo) | Alimentación del Arduino |
| **GND** | Fuente 12V (negativo) | Tierra |

### L298N → Bomba 12V

| L298N | Conexión |
|---|---|
| **+12V (Vs)** | Fuente 12V (positivo) |
| **GND** | GND común (Arduino + fuente) |
| **ENA** | Arduino Pin 9 (PWM) |
| **IN1** | Arduino Pin 7 |
| **IN2** | Arduino Pin 8 |
| **OUT1** | Bomba 12V (+) |
| **OUT2** | Bomba 12V (−) |

## Fuente de alimentación

| Componente | Voltaje | Corriente estimada |
|---|---|---|
| Arduino Uno | 5V (vía Vin 7-12V) | ~50 mA |
| Sensor GFS401 | 5V | ~10 mA |
| LCD 16×2 + I2C | 5V | ~100 mA (con backlight) |
| Potenciómetro | 5V | ~0.5 mA |
| Bomba DC | 12V | 300 – 1000 mA (según modelo) |
| Driver (L298N) | 12V | ~20 mA (lógica) |

**Total 5V:** ~160 mA (dentro del límite de Arduino)
**Total 12V:** 300–1000 mA (fuente externa)

## Recomendaciones

1. Usar una **fuente 12V** dedicada para la bomba. No alimentar la bomba desde el pin 5V de Arduino.
2. Conectar **tierra común** entre la fuente, Arduino y driver PWM.
3. Agregar un **diodo flyback** (1N4007) en paralelo a la bomba si el driver no lo incluye.
4. Colocar un **capacitor de 100 µF** entre VCC y GND del driver para filtrar picos.
5. Usar cables apantallados para el sensor GFS401 si el entorno tiene ruido eléctrico.
