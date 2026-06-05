# Potenciómetro — SetPoint

## Descripción

Potenciómetro lineal de 10 kΩ usado para definir el caudal deseado (SetPoint) del sistema de control.

## Especificaciones

| Parámetro | Valor |
|---|---|
| **Valor nominal** | 10 kΩ |
| **Tipo** | Lineal (B) |
| **Tolerancia** | ±10% |
| **Potencia** | 0.25 W |
| **Conexión** | 3 pines (VCC, Wiper, GND) |

## Conexión

```
5V ──── Potenciómetro Pin 1 (VCC)
A0 ──── Potenciómetro Pin 2 (Wiper)
GND ─── Potenciómetro Pin 3 (GND)
```

## Mapa de valores (SetPoint)

El ADC de Arduino (10 bits) mapea el voltaje del potenciómetro a un valor de 0–1023.

Rango del SetPoint: **0.0 – 6.0 L/min** (límite superior del sensor GFS401).

```
SetPoint (L/min) = (valor_ADC / 1023.0) × 6.0
```

| Posición potenciómetro | Valor ADC | SetPoint (L/min) |
|---|---|---|
| Mínimo (0°) | 0 | 0.0 |
| 25% | ~256 | 1.5 |
| 50% | ~512 | 3.0 |
| 75% | ~768 | 4.5 |
| Máximo (270°) | 1023 | 6.0 |

## Código de lectura

```cpp
const float SP_MAX = 6.0;
int adc = analogRead(A0);
float setpoint = (adc / 1023.0) * SP_MAX;
```
