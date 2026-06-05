# LCD 16×2 con Módulo I2C

## Descripción

Pantalla de cristal líquido (LCD) de 16 columnas × 2 filas con adaptador I2C integrado (PCF8574). Permite la comunicación con Arduino usando solo 2 pines (SDA y SCL).

## Especificaciones

| Parámetro | Valor |
|---|---|
| **Tipo** | LCD 16×2 caracteres |
| **Controlador** | Hitachi HD44780 |
| **Adaptador** | PCF8574 (I2C) |
| **Dirección I2C** | 0x27 (por defecto, configurable) |
| **Voltaje** | 5V |
| **Pines usados** | SDA (A4), SCL (A5) en Arduino Uno |
| **Retroiluminación** | LED, controlable por jumper |

## Asignación de pines

| Pin I2C | Conexión Arduino Uno |
|---|---|
| **VCC** | 5V |
| **GND** | GND |
| **SDA** | A4 (o pin SDA en Uno R3) |
| **SCL** | A5 (o pin SCL en Uno R3) |

## Librerías necesarias (Arduino)

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
```

## Distribución de la información

```
┌─────────────────────────────────┐
│ SP:  xx.x L/min  PV: xx.x L/min │  ← Línea 0
│ E:   xx.x        PWM: xxx       │  ← Línea 1
└─────────────────────────────────┘
```

| Campo | Descripción |
|---|---|
| **SP** | SetPoint — caudal deseado (L/min) |
| **PV** | Process Variable — caudal real (L/min) |
| **E** | Error de control (SP − PV) |
| **PWM** | Señal PWM aplicada a la bomba (0–255) |
