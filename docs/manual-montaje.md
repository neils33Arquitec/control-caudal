# Manual de Montaje

## Sistema de Control de Caudal PI

---

## Índice

1. [Lista de materiales](#1-lista-de-materiales)
2. [Herramientas necesarias](#2-herramientas-necesarias)
3. [Paso 1: Preparación de la protoboard](#3-paso-1-preparación-de-la-protoboard)
4. [Paso 2: Conexión del Arduino](#4-paso-2-conexión-del-arduino)
5. [Paso 3: Conexión del sensor GFS401](#5-paso-3-conexión-del-sensor-gfs401)
6. [Paso 4: Conexión del L298N y la bomba](#6-paso-4-conexión-del-l298n-y-la-bomba)
7. [Paso 5: Conexión del LCD I2C](#7-paso-5-conexión-del-lcd-i2c)
8. [Paso 6: Conexión del potenciómetro](#8-paso-6-conexión-del-potenciómetro)
9. [Paso 7: Cableado de alimentación](#9-paso-7-cableado-de-alimentación)
10. [Paso 8: Carga del firmware](#10-paso-8-carga-del-firmware)
11. [Paso 9: Prueba inicial](#11-paso-9-prueba-inicial)
12. [Diagrama completo](#12-diagrama-completo)
13. [Solución de problemas](#13-solución-de-problemas)

---

## 1. Lista de materiales

| # | Componente | Cantidad |
|---|---|---|
| 1 | Arduino Uno R3 | 1 |
| 2 | Sensor de flujo GFS401 (YF-S401) | 1 |
| 3 | Bomba DC 12V | 1 |
| 4 | Módulo puente H L298N | 1 |
| 5 | LCD 16×2 con módulo I2C | 1 |
| 6 | Potenciómetro lineal 10 kΩ | 1 |
| 7 | Fuente de alimentación 12V DC, ≥1A | 1 |
| 8 | Protoboard 830 puntos | 1 |
| 9 | Cables jumper M-M y M-H | 20 |
| 10 | Capacitor 100 µF, 25V | 1 |
| 11 | Diodo 1N4007 | 1 |
| 12 | Resistencia 10 kΩ (opcional) | 1 |
| 13 | Cable USB A-B | 1 |
| 14 | Manguera de 7 mm de diámetro | 1 m |

---

## 2. Herramientas necesarias

- Multímetro digital
- Cautín y estaño (para conexiones permanentes, opcional)
- Pelacables
- Destornillador pequeño (para bornes del L298N)
- Computador con Arduino IDE instalado

---

## 3. Paso 1: Preparación de la protoboard

Colocar la protoboard en posición vertical (buses de alimentación a los lados).

**Antes de insertar cualquier componente:**
1. Verificar continuidad de los buses con el multímetro.
2. Identificar los buses positivo (rojo) y negativo (azul) a cada lado.
3. No conectar alimentación hasta tener todo el cableado revisado.

---

## 4. Paso 2: Conexión del Arduino

Insertar el Arduino Uno en la protoboard (opcional, también puede quedar fuera).

| Conexión | Destino |
|---|---|
| Pin GND | Bus negativo (−) de la protoboard |
| Pin 5V | Bus positivo (+) de la protoboard |

> **Importante:** El pin 5V del Arduino solo alimenta componentes de baja corriente (sensor, LCD, potenciómetro). La bomba se alimenta desde la fuente externa de 12V.

---

## 5. Paso 3: Conexión del sensor GFS401

**Identificación de cables:**

| Color | Señal |
|---|---|
| Rojo | VCC (+5V) |
| Negro | GND |
| Amarillo | Señal de pulsos (OUT) |

**Conexión:**

| Cable GFS401 | Conectar a |
|---|---|
| Rojo | Bus 5V de la protoboard |
| Negro | Bus GND de la protoboard |
| Amarillo | Pin digital 2 del Arduino |

**Configuración:** El pin 2 del Arduino se configura como entrada con pull-up interno (`INPUT_PULLUP`). La interrupción se dispara en flanco de subida (`RISING`).

**Instalación hidráulica:**

```
  Bomba ──── manguera ──── GFS401 ──── manguera ──── salida/drenaje
                              │
                           (flecha = dirección del flujo)
```

- La flecha marcada en el cuerpo del sensor debe apuntar en la dirección del flujo.
- Orientación vertical recomendada (inclinación máxima ≤ 5°).
- Las conexiones son de 7 mm de diámetro (barba). Usar manguera de silicona.

---

## 6. Paso 4: Conexión del L298N y la bomba

**Bornera de potencia del L298N:**

| Terminal L298N | Conectar a |
|---|---|
| +12V (Vs) | Fuente 12V (+) |
| GND | Bus GND protoboard (y fuente 12V −) |
| +5V (Vss) | NO conectar (usar 5V del Arduino) |

**Bornera de control del L298N:**

| Terminal L298N | Conectar a |
|---|---|
| ENA (Enable A) | Pin 9 del Arduino (PWM) |
| IN1 | Pin 7 del Arduino |
| IN2 | Pin 8 del Arduino |
| ENB | No conectar |
| IN3 | No conectar |
| IN4 | No conectar |

**Bornera de salida (motor):**

| Terminal | Conectar a |
|---|---|
| OUT1 | Bomba 12V (+) |
| OUT2 | Bomba 12V (−) |

**Configuración de jumpers:**
- Retirar el jumper de ENA (para recibir PWM desde el pin 9).
- Mantener los demás jumpers según configuración de fábrica.

**Protección:**
- Colocar el capacitor de 100 µF entre +12V y GND lo más cerca posible del L298N.
- Conectar el diodo 1N4007 en paralelo a la bomba: cátodo al (+), ánodo al (−).

```
Esquema del diodo flyback:

  OUT1 ──►───┬───►─── Bomba (+)
              │
             ▲
        1N4007│ (cátodo arriba)
              │
  OUT2 ───────┴─────── Bomba (−)
```

---

## 7. Paso 5: Conexión del LCD I2C

El LCD 16×2 con adaptador I2C usa solo 4 pines:

| Pin LCD I2C | Conectar a |
|---|---|
| VCC | Bus 5V protoboard |
| GND | Bus GND protoboard |
| SDA | Pin A4 (SDA) del Arduino |
| SCL | Pin A5 (SCL) del Arduino |

**Dirección I2C:** Por defecto es `0x27`. Si no funciona, usar un scanner I2C para verificarla.

---

## 8. Paso 6: Conexión del potenciómetro

| Patilla | Conectar a |
|---|---|
| Izquierda | Bus 5V protoboard |
| Centro (wiper) | Pin A0 del Arduino |
| Derecha | Bus GND protoboard |

El potenciómetro mide el SetPoint de caudal (0 a 6 L/min) mediante el ADC de 10 bits del Arduino.

---

## 9. Paso 7: Cableado de alimentación

**Diagrama de distribución de alimentación:**

```
Fuente 12V DC (≥1A)
    │
    ├── (+) ──── L298N (+12V)
    │          ─── Arduino (Vin)
    │
    └── (−) ──── L298N (GND)
               ─── Arduino (GND)
               ─── Bus GND protoboard

Arduino 5V ─── Bus 5V protoboard (alimenta sensor, LCD, potenciómetro)
```

**Verificaciones con multímetro (sin conectar la fuente):**
1. [ ] Resistencia entre 5V y GND del Arduino > 100 Ω (sin corto).
2. [ ] Resistencia entre +12V y GND del L298N > 1 kΩ (sin corto).
3. [ ] Continuidad entre todos los GND.

---

## 10. Paso 8: Carga del firmware

1. Abrir `src/firmware/control-caudal.ino` en Arduino IDE.
2. Las librerías `PIController.h` y `config.h` se incluyen automáticamente al estar en las carpetas `lib/` e `include/`.
3. Ir a **Herramientas → Gestor de librerías** e instalar `LiquidCrystal I2C` de Frank de Brabander.
4. Seleccionar placa: **Arduino Uno**.
5. Seleccionar puerto COM correspondiente.
6. Verificar compilación (✓).
7. Cargar el sketch.

---

## 11. Paso 9: Prueba inicial

1. Conectar la fuente 12V al L298N (la bomba NO debe arrancar aún si el potenciómetro está al mínimo).
2. Conectar el USB del Arduino al computador.
3. Abrir el monitor serial (9600 baud).
4. El LCD debe mostrar el mensaje "Iniciando..." y luego los valores de SP, PV, Error y PWM.
5. Girar lentamente el potenciómetro — la bomba debe comenzar a girar y el caudal aumentar.
6. Verificar que el LCD muestre:
   - `SP: 3.0 PV: 2.9` (valores aproximados)
   - `E: 0.1 PWM: 150`

---

## 12. Diagrama completo

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
    │  │ Amarillo ───┼───┤           (SetPoint)          │
    │  └─────────────┘   │                               │
    │                    │                               │
    │                    │  A4 (SDA) ─── LCD I2C ────────┤
    │                    │  A5 (SCL) ─── LCD I2C ────────┤
    │                    │                               │
    │                    │  Vin ───── 12V ─── Fuente ────┤
    │                    │  GND ───── GND ─── Fuente ────┤
    │                    └───────────────────────────────┘
    │
    └────────────────────────────────────────────────────┐
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
```

---

## 13. Solución de problemas

### El LCD no muestra nada
- Verificar que el potenciómetro de contraste del LCD esté ajustado.
- Verificar dirección I2C (usar scanner I2C).
- Verificar conexiones SDA/SCL.

### La bomba no gira
- Verificar que ENA tenga jumper o esté recibiendo señal PWM.
- Verificar IN1 = HIGH, IN2 = LOW (o viceversa).
- Medir voltaje en OUT1/OUT2 con multímetro.
- Verificar fuente 12V.

### No se lee caudal
- Verificar conexión del cable amarillo al pin 2.
- Verificar que la flecha del sensor esté en dirección del flujo.
- Abrir monitor serial para ver si llegan pulsos.

### Lecturas de caudal erráticas
- Purgar el aire del sistema antes de medir.
- Verificar que no haya fugas en las conexiones de manguera.
- El sensor requiere un caudal mínimo de ~0.3 L/min para funcionar.
