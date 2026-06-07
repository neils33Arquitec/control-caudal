# Plan de Mejoras — Sistema de Control de Caudal

> Documento técnico con las correcciones y mejoras detectadas durante la
> prueba del prototipo real. Pendientes de implementar en el próximo montaje.

---

## Índice

1. [Corrección: Escalado de salida del PI](#1-corrección-escalado-de-salida-del-pi)
2. [Mejora: Ciclo de pantallas LCD (3 pantallas cada 10s)](#2-mejora-ciclo-de-pantallas-lcd-3-pantallas-cada-10s)
3. [Orden de implementación](#3-orden-de-implementación)
4. [Resumen de archivos a modificar](#4-resumen-de-archivos-a-modificar)

---

## 1. Corrección: Escalado de salida del PI

### Problema

El controlador PI opera internamente en **L/min**, pero su salida se usa
directamente como **PWM (0-255)** sin escalar. Con `KP=2.0`, un error de
`0.56 L/min` produce solo `~1.12 PWM` de corrección. La bomba no responde con
PWM 1, el error persiste, la integral acumula durante minutos, y cuando la
bomba finalmente arranca lo hace con un PWM desproporcionado.

### Datos del prototipo

| Variable | Valor | Comentario |
|---|---|---|
| SP | 0.6 L/min | SetPoint desde potenciómetro |
| PV | ~0.04 L/min | Caudal real (0.2 con 1 decimal en LCD) |
| Error | 0.56 L/min | SP - PV |
| PWM observado | 150 | Desproporcionado para SP=0.6 |

### Solución

El PI debe trabajar internamente en L/min (0 a `SP_MAX`) y escalar su salida
a PWM al final. Así los gains `KP=2.0`, `KI=0.5` tienen sentido físico y
coinciden con la sintonización Ziegler-Nichols.

#### Cambio 1 — Límites del PI (`control-caudal.ino:7`)

**Actual:**
```cpp
PIController pi(KP, KI, TS, PWM_MIN, PWM_MAX);
```

**Nuevo:**
```cpp
PIController pi(KP, KI, TS, 0.0, SP_MAX);
```

Los límites pasan de PWM (0-255) a L/min (0-`SP_MAX`).

#### Cambio 2 — Escalado de salida (`control-caudal.ino:76-77`)

**Actual:**
```cpp
pwm_out = (int)pi.compute(setpoint, caudal);
```

**Nuevo:**
```cpp
float pi_out = pi.compute(setpoint, caudal);
pwm_out = (int)((pi_out / SP_MAX) * (float)PWM_MAX);
```

La salida del PI (en L/min) se escala linealmente a PWM (0-255).

#### Cambio 3 — Reinicio al cambiar setpoint (`control-caudal.ino`)

Agregar variable:
```cpp
float setpoint_anterior = -1.0;
```

Agregar dentro del `if (dt >= TS*1000)` antes del LCD:
```cpp
if (fabs(setpoint - setpoint_anterior) > 0.05) {
    pi.reset();
    setpoint_anterior = setpoint;
}
```

Evita que la integral arrastre error de un setpoint anterior al girar el
potenciómetro.

#### Cambio 4 — Actualizar tests (`test/unit/test_pi_controller.cpp`)

- Cambiar `#include "../src/PIController.h"` → `"../src/firmware/PIController.h"`
- Cambiar límites de saturación: `PIController pi(2.0, 0.5, 0.1, 0, 6.0)` en vez de `(..., 0, 255)`
- Actualizar verificación de saturación: `out <= 6.0f` en vez de `out <= 255.0f`

### Verificación matemática

El script `scripts/matlab/sintonizar_ZN.m` modela la planta con
`K = 0.02 (L/min)/PWM`. Con el escalado (`×42.5`), la ganancia efectiva es:

```
K_efectiva = 0.02 × (255 / 6) ≈ 0.85 (L/min) / (L/min)
```

Aplicando Ziegler-Nichols:

| Parámetro | ZN teórico | Efectivo con KP=2.0, KI=0.5 |
|---|---|---|
| KP efectivo | ~2.26 | 2.0 |
| KI efectivo | ~0.50 | 0.5 |

Los valores actuales están muy cerca del óptimo ZN una vez aplicado el
escalado.

---

## 2. Mejora: Ciclo de pantallas LCD (3 pantallas cada 10s)

### Objetivo

Implementar un ciclo automático de 3 pantallas en el LCD 16×2 que se repite
cada 10 segundos, mostrando información diferente según el modo:

| Tiempo | Pantalla | Contenido |
|---|---|---|
| 0s → 6s | **Normal** | `SP:3.5 PV:2.8` / `E:0.7 PWM:128` |
| 6s → 8s | **Pot** | `ADC:512 2.50V` / `SP:3.0 L/min` |
| 8s → 10s | **PWM** | `PWM: 128/255` / `50.2% potencia` |

### Cambios en `src/firmware/control-caudal.ino`

#### Cambio 1 — Nueva variable global (después de `serial_anterior`)

```cpp
unsigned long lcd_cycle_start = 0;
```

#### Cambio 2 — Inicializar en `setup()` (junto a `tiempo_anterior`)

```cpp
lcd_cycle_start = millis();
```

#### Cambio 3 — Reemplazar bloque LCD en `loop()`

**Bloque actual** (líneas ~79-91):
```cpp
        lcd.setCursor(0, 0);
        lcd.print("SP:");
        lcd.print(setpoint, 1);
        lcd.print(" PV:");
        lcd.print(caudal, 1);
        lcd.print("      ");

        lcd.setCursor(0, 1);
        lcd.print("E:");
        lcd.print(error, 1);
        lcd.print(" PWM:");
        lcd.print(pwm_out);
        lcd.print("   ");
```

**Nuevo bloque:**
```cpp
        // --- Ciclo de pantallas LCD (10s) ---
        unsigned long ciclo_ms = ahora - lcd_cycle_start;

        if (ciclo_ms < 6000) {
            // Pantalla 1: Normal (SP, PV, Error, PWM)
            lcd.setCursor(0, 0);
            lcd.print("SP:");
            lcd.print(setpoint, 1);
            lcd.print(" PV:");
            lcd.print(caudal, 1);
            lcd.print("      ");

            lcd.setCursor(0, 1);
            lcd.print("E:");
            lcd.print(error, 1);
            lcd.print(" PWM:");
            lcd.print(pwm_out);
            lcd.print("   ");
        }
        else if (ciclo_ms < 8000) {
            // Pantalla 2: Potenciómetro (ADC, voltaje, SP)
            int adc = analogRead(PIN_POT);
            float volt = (adc / 1023.0) * 5.0;

            lcd.setCursor(0, 0);
            lcd.print("ADC:");
            lcd.print(adc);
            lcd.print(" ");
            lcd.print(volt, 1);
            lcd.print("V   ");

            lcd.setCursor(0, 1);
            lcd.print("SP:");
            lcd.print(setpoint, 1);
            lcd.print(" L/min   ");
        }
        else {
            // Pantalla 3: PWM (valor y porcentaje)
            float pct = (pwm_out * 100.0) / 255.0;

            lcd.setCursor(0, 0);
            lcd.print("PWM: ");
            lcd.print(pwm_out);
            lcd.print("/255 ");

            lcd.setCursor(0, 1);
            lcd.print(pct, 1);
            lcd.print("%         ");
        }

        if (ciclo_ms >= 10000)
            lcd_cycle_start = ahora;
```

### Notas técnicas

- No se usa `lcd.clear()` para evitar flicker; cada pantalla sobrescribe
  completamente los 32 caracteres (16×2) con espacios de relleno.
- El ADC se lee fresco en la pantalla del potenciómetro (`analogRead` directo).
- El ciclo se maneja con `millis()` y es independiente del lazo de control.
- Compatible con la corrección de escalado PI (sin conflictos de código).

---

## 3. Orden de implementación

| Paso | Descripción | Depende de |
|---|---|---|
| 1 | Aplicar corrección de escalado PI | — |
| 2 | Actualizar tests unitarios | Paso 1 |
| 3 | Compilar con arduino-cli y verificar | Paso 1 |
| 4 | Ejecutar tests unitarios en PC (`g++`) | Paso 2 |
| 5 | Implementar ciclo de pantallas LCD | — |
| 6 | Compilar y verificar todo junto | Pasos 1-5 |
| 7 | Probar en prototipo real | Pasos 1-6 |

---

## 4. Resumen de archivos a modificar

| Archivo | Cambios | Líneas aprox. |
|---|---|---|
| `src/firmware/control-caudal.ino` | Escalado PI + reinicio por cambio SP + ciclo LCD | ~15 |
| `test/unit/test_pi_controller.cpp` | Include path + límites de saturación | ~4 |

Ningún otro archivo del proyecto requiere modificación.
