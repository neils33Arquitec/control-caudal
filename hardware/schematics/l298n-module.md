# Módulo Puente H L298N

## Descripción

El L298N es un driver dual de puente H para cargas inductivas como motores DC, capaz de manejar hasta 2 A por canal. Se utiliza para controlar la velocidad (PWM) y dirección de la bomba DC de 12V.

## Especificaciones

| Parámetro | Valor |
|---|---|
| **Circuito integrado** | L298N (STMicroelectronics) |
| **Voltaje lógico (Vss)** | 5V (desde Arduino) |
| **Voltaje motor (Vs)** | 5 – 35V DC |
| **Corriente máxima por canal** | 2 A |
| **Corriente máxima total** | 4 A |
| **Consumo lógico** | ~20 mA |
| **Caída de tensión** | ~2V (saturación) |

## Pines del módulo

### Terminal de potencia

| Pin | Conexión |
|---|---|
| **+12V / Vs** | Fuente externa 12V (positivo) |
| **GND** | Fuente externa 12V (negativo) y GND común con Arduino |
| **+5V (out)** | Salida 5V regulada (no usar si Vs > 12V) |

### Terminal de control

| Pin | Conexión | Función |
|---|---|---|
| **ENA** | Arduino Pin 9 (PWM) | Enable canal A (PWM bomba) |
| **IN1** | Arduino Pin 7 | Dirección motor A (H/L) |
| **IN2** | Arduino Pin 8 | Dirección motor A (L/H) |
| **ENB** | — | Enable canal B (no usado) |
| **IN3** | — | Dirección motor B (no usado) |
| **IN4** | — | Dirección motor B (no usado) |

### Terminal de salida (motor)

| Pin | Conexión |
|---|---|
| **OUT1** | Bomba DC (+) |
| **OUT2** | Bomba DC (−) |

## Control de dirección y velocidad

| IN1 | IN2 | ENA | Función |
|---|---|---|---|
| HIGH | LOW | PWM | Adelante (velocidad = PWM) |
| LOW | HIGH | PWM | Atrás (velocidad = PWM) |
| LOW | LOW | X | Freno (coast) |
| HIGH | HIGH | X | Freno (brake) |

Para la bomba de agua solo se usa un sentido de giro:

```
IN1 = HIGH, IN2 = LOW, ENA = PWM (Pin 9 Arduino)
```

## Esquema de conexión

```
                    ┌──────────────┐
                    │   L298N      │
                    │              │
                    │  +12V ───────┼─── Fuente 12V (+)
                    │  GND  ───────┼─── Fuente 12V (−) y Arduino GND
                    │              │
  Arduino ── PWM ──▶│  ENA         │
  Arduino ── Pin 7 ─▶│  IN1         │
  Arduino ── Pin 8 ─▶│  IN2         │
                    │              │
                    │  OUT1 ───────┼─── Bomba 12V (+)
                    │  OUT2 ───────┼─── Bomba 12V (−)
                    └──────────────┘
```

## Recomendaciones de uso

1. **Jumpers:** Retirar el jumper de ENA si se usa PWM por separado; dejarlo puesto si se quiere 5V fijo.
2. **Tierra común:** Conectar GND del L298N al GND de Arduino y al negativo de la fuente 12V.
3. **Capacitor de filtro:** Colocar un capacitor de 100 µF entre +12V y GND cerca del L298N.
4. **Disipación:** El L298N puede calentar a corrientes >1A. Usar disipador si es necesario.
5. **5V Out:** Si se usa Vs ≤ 12V, el pin +5V del L298N puede alimentar el Arduino (no recomendado si la bomba consume mucha corriente).
