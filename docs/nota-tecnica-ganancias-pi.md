# Nota Técnica — Ganancia Proporcional e Integral del Controlador PI

**Proyecto:** Sistema de Control de Caudal en Lazo Cerrado  
**Asignatura:** Teoría Moderna de Control — Universidad Santiago Mariño  
**Fecha:** Junio 2026  

---

## 1. Ganancia Proporcional — KP = 2.0

**Función:** reacciona al error *en el instante actual*.

```
P = KP × error = 2.0 × error
```

El controlador PI opera internamente en L/min y escala su salida a PWM al final. La tabla siguiente muestra el comportamiento con SP = 3.0 L/min:

| Situación | Error (L/min) | Aporte P (L/min) | PWM resultante |
|---|---|---|---|
| PV = 1.0 | 2.0 | 4.0 | ~170 |
| PV = 2.5 | 0.5 | 1.0 | ~42 |
| PV = 3.0 | 0.0 | 0.0 | 0 (solo integral) |

**Limitación de KP actuando solo:** cuando el error se acerca a cero, la acción proporcional también se acerca a cero y la bomba no recibe suficiente señal para alcanzar exactamente el SetPoint. Esto genera un **error estacionario permanente**. La acción integral existe para corregirlo.

---

## 2. Ganancia Integral — KI = 0.5

**Función:** acumula el error en el tiempo y lo corrige aunque sea muy pequeño.

Discretización por Backward Euler con Ts = 0.1 s:

```
I[k] = I[k-1] + KI × Ts × error
     = I[k-1] + 0.5 × 0.1 × error
     = I[k-1] + 0.05 × error       (cada 100 ms)
```

**Ejemplo con error estacionario de 0.2 L/min** (SP=3.0, PV se estabiliza en 2.8):

```
t = 0.1 s  →  I = 0.000 + 0.05 × 0.2 = 0.010
t = 0.2 s  →  I = 0.010 + 0.010       = 0.020
t = 0.3 s  →  I = 0.020 + 0.010       = 0.030
...
t = 2.0 s  →  I ≈ 0.200  → bomba recibe más señal
t = 4.0 s  →  I ≈ 0.400  → el error comienza a reducirse
```

KI empuja la bomba de forma progresiva hasta que `error = 0`, eliminando el error permanente.

---

## 3. Acción conjunta en el lazo de control

Implementación en el firmware (`src/firmware/control-caudal.ino`, líneas 84-85):

```cpp
float pi_out = pi.compute(setpoint, caudal);
pwm_out = (int)((pi_out / SP_MAX) * PWM_MAX);
```

Internamente en `PIController.cpp`:

```
error    = SP - PV
integral += KI × Ts × error        →  0.5 × 0.1 × error
pi_out   = KP × error + integral   →  2.0 × error + integral
pwm      = (pi_out / 6.0) × 255    →  escalado a rango 0–255
```

---

## 4. Efecto de variar los parámetros

### KP

| Valor | Efecto |
|---|---|
| KP muy alto (> 5) | Respuesta rápida pero oscilaciones y posible inestabilidad |
| KP = 2.0 (actual) | Equilibrio entre velocidad de respuesta y estabilidad |
| KP muy bajo (< 0.5) | Respuesta lenta, tarda en alcanzar el SetPoint |

### KI

| Valor | Efecto |
|---|---|
| KI muy alto (> 2.0) | Elimina el error rápido pero genera sobreimpulso |
| KI = 0.5 (actual) | Conservador, elimina el error sin sobreimpulso excesivo |
| KI muy bajo (< 0.1) | Tarda minutos en eliminar el error permanente |

---

## 5. Validación con Ziegler-Nichols

El script `scripts/matlab/sintonizar_ZN.m` modela la planta con una ganancia estimada de K = 0.02 (L/min)/PWM. Con el escalado aplicado (factor ×42.5), la ganancia efectiva desde la perspectiva del controlador es:

```
K_efectiva = 0.02 × (255 / 6) ≈ 0.85 (L/min) / (L/min)
```

Aplicando las fórmulas de Ziegler-Nichols para lazo abierto (método de la curva de reacción):

| Parámetro | ZN teórico | Valor actual |
|---|---|---|
| KP | ~2.26 | 2.0 |
| KI | ~0.50 | 0.5 |

Los valores actuales están muy cerca del óptimo teórico. Se ajustarán finalmente con la prueba de escalón real una vez medidos K, τ y θ de la planta.

---

## 6. Próximo paso — Sintonización experimental

Para obtener los valores definitivos de KP y KI:

1. Llevar el sistema a estado estacionario (PWM fijo, caudal estable).
2. Aplicar un escalón en la entrada (ej. PWM = 0 → PWM = 150).
3. Registrar la respuesta con `scripts/python/log_datos.py`.
4. Graficar con `scripts/python/graficar_respuesta.py` y medir K, τ, θ.
5. Calcular KP y KI con `scripts/matlab/sintonizar_ZN.m`.
6. Actualizar `KP` y `KI` en `src/firmware/config.h`.

---

*Nota técnica generada como parte del proyecto académico de Teoría Moderna de Control — Universidad Santiago Mariño — Junio 2026*
