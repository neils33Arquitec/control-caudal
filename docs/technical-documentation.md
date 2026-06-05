# Documentación Técnica e Investigación

## Sistema Inteligente de Control de Caudal mediante Control PI en Lazo Cerrado

---

# Marco Teórico

## 1. Sensor de Caudal GFS401 (equivalente YF-S401)

### 1.1 Principio de funcionamiento

El GFS401 es un sensor de flujo de agua basado en el **efecto Hall**. Internamente contiene un rotor (impulsor) con un imán incorporado. Cuando el agua fluye a través del cuerpo del sensor, el rotor gira y el campo magnético del imán pasa frente a un sensor Hall, que genera un **pulso digital** por cada paso del imán.

### 1.2 Especificaciones técnicas

| Parámetro | Valor |
|---|---|
| Voltaje de operación | 5 – 24 V DC |
| Corriente máxima | 10 mA (a 5V) |
| Rango de caudal | 0.3 – 6 L/min |
| Presión máxima | 0.8 MPa |
| Temperatura de operación | -25 °C a +80 °C |
| Pulsos por litro (factor K) | ~5880 pulsos/L |
| Conexión hidráulica | Barba 7 mm (1/4″) |
| Tipo de salida | Digital TTL (onda cuadrada) |

### 1.3 Relación frecuencia-caudal

El sensor genera ~5880 pulsos por litro. La frecuencia de salida es linealmente proporcional al caudal:

```
Q(L/min) = (pulsos_por_segundo × 60) / 5880
```

Simplificando:

```
Q(L/min) = F(Hz) / 98
```

Para convertir a L/hora:

```
Q(L/h) = (pulsos_por_segundo × 3600) / 5880
```

### 1.4 Diagrama de conexión con Arduino

```
GFS401            Arduino Uno
────────────────  ───────────
Rojo  (VCC)  ───  5V
Negro (GND)  ───  GND
Amarillo (OUT) ─  Pin 2 (Interrupción 0)
```

### 1.5 Cálculo de caudal en firmware

Estrategia de medición por interrupción:

1. Se configura una interrupción por flanco de subida en el pin de datos.
2. Un contador (`flow_frequency`) se incrementa en cada interrupción.
3. Cada segundo se calcula:
   ```
   l_min = (flow_frequency × 60) / 5880.0
   ```
4. Se resetea el contador y se repite el ciclo.

*Referencias:* [YF-S401 Datasheet — Mantech], [YF-S401 — DigiKey/DFRobot]

---

## 2. Modelado FOPDT (First Order Plus Dead Time)

### 2.1 Definición

El modelo FOPDT es una representación simplificada de sistemas dinámicos estables que presentan un retardo de transporte. Describe la relación entre una entrada (variable manipulada) y una salida (variable de proceso) mediante tres parámetros:

```
G(s) = K × e^(-θs) / (τs + 1)
```

Donde:
- **K** = Ganancia del proceso (Δsalida / Δentrada en estado estacionario)
- **τ** = Constante de tiempo (tiempo en alcanzar el 63.2% del valor final)
- **θ** = Tiempo muerto o retardo (tiempo antes de que la salida comience a responder)

### 2.2 Identificación experimental — Método de la curva de reacción

Procedimiento para obtener los parámetros FOPDT de la planta (bomba + tubería + sensor):

1. **Paso 1**: Llevar el sistema a un estado estacionario inicial (ej. PWM = 0 → caudal = 0).
2. **Paso 2**: Aplicar un escalón en la entrada (ej. PWM = 0 → PWM = 150).
3. **Paso 3**: Registrar la respuesta temporal del caudal.
4. **Paso 4**: Determinar gráficamente:
   - **θ**: Tiempo desde el escalón hasta que la variable de proceso comienza a cambiar visiblemente.
   - **τ**: Tiempo desde que termina θ hasta que se alcanza el 63.2% del cambio total.
   - **K**: (Δ caudal) / (Δ PWM) en estado estacionario.

### 2.3 Aplicación al sistema bomba DC + tubería

La planta del proyecto (bomba DC + conducto hidráulico + fluido + sensor) se comporta como un sistema **FOPDT** porque:

- La bomba DC tiene una dinámica electromecánica de primer orden (constante de tiempo eléctrica y mecánica).
- El fluido presenta un retardo de transporte (tiempo en recorrer la tubería desde la bomba hasta el sensor).
- El sensor de flujo añade un pequeño retardo de procesamiento.

*Referencias:* [Control Global — R. Russell Rhinehart], [APMonitor — FOPDT], [Towards AI — Identificación FOPDT]

---

## 3. Controlador PI Discreto

### 3.1 Ecuación del controlador PI continuo

```
u(t) = Kp × e(t) + Ki × ∫ e(τ) dτ
```

O en forma paralela con constante de tiempo integral:

```
u(t) = Kp × [ e(t) + (1/Ti) × ∫ e(τ) dτ ]
```

Donde:
- `Kp` = Ganancia proporcional
- `Ki` = Ganancia integral = `Kp / Ti`
- `Ti` = Tiempo integral
- `e(t)` = Error = SetPoint − Variable de Proceso

### 3.2 Discretización — Método de Euler hacia atrás (Backward Euler)

Para implementar el PI en un microcontrolador digital (Arduino), se discretiza la ecuación continua:

**Forma recursiva (positional form):**

```
u[k] = Kp × e[k] + I[k]

I[k] = I[k-1] + Ki × Ts × e[k]
```

Donde:
- `Ts` = Período de muestreo (sampling time)
- `k` = Índice de la muestra actual
- `e[k]` = Error en la muestra actual
- `I[k]` = Acumulador integral en la muestra actual

### 3.3 Algoritmo en pseudocódigo

```
// Parámetros
Kp = 2.0    // Ajustar según sintonización
Ki = 0.5    // Ajustar según sintonización
Ts = 0.1    // Período de muestreo en segundos (100 ms)

// Variables globales
integral = 0
error_anterior = 0
pwm_min = 0
pwm_max = 255

// Ejecutar en cada ciclo de control
PI_Controller(setpoint, medicion):
    error = setpoint - medicion
    integral = integral + Ki * Ts * error
    // Anti-Windup: saturar integral
    integral = constrain(integral, pwm_min - Kp*error, pwm_max - Kp*error)
    u = Kp * error + integral
    // Saturación de salida
    u = constrain(u, pwm_min, pwm_max)
    return u
```

### 3.4 Anti-Windup (Protección antivuelco integral)

El **windup integral** ocurre cuando el actuador se satura (ej. PWM = 255) pero el error persiste, haciendo que el término integral siga creciendo. Al desaturarse, el controlador responde con un exceso de acción integral, provocando sobreimpulso y oscilaciones.

**Técnicas de Anti-Windup implementadas:**

| Técnica | Descripción |
|---|---|
| **Saturación condicional** | El término integral solo se actualiza si la salida del controlador no está saturada. |
| **Back-calculation** | Se realimenta la diferencia entre la salida saturada y no saturada para corregir el integrador. |
| **Clamping** | El acumulador integral se limita a un rango predefinido (más simple, usado en el pseudocódigo). |

*Referencias:* [Analog Devices — Implementation of PI Controllers], [MathWorks — PI Controller with Integral Anti-Windup], [GitHub CrissCCL — Digital Control Anti-Windup]

---

## 4. Sintonización Ziegler-Nichols

### 4.1 Método del lazo cerrado (Ultimate Gain Method)

Procedimiento:

1. **Poner Ki = 0 y Kd = 0** (solo control proporcional).
2. **Incrementar Kp gradualmente** hasta que el sistema oscile con amplitud constante (frecuencia última).
3. Registrar:
   - **Ku** = Ganancia última (Kp en el punto de oscilación sostenida).
   - **Tu** = Período de oscilación última (en segundos).
4. Aplicar las reglas de Ziegler-Nichols:

| Tipo de control | Kp | Ki | Kd |
|---|---|---|---|
| **P** | 0.5 × Ku | — | — |
| **PI** | 0.45 × Ku | 0.54 × Ku / Tu | — |
| **PID** | 0.6 × Ku | 1.2 × Ku / Tu | 0.075 × Ku × Tu |

### 4.2 Método de la curva de reacción (Open-Loop Method)

Basado en la respuesta al escalón (modelo FOPDT):

1. Obtener `K`, `τ` y `θ` de la curva de reacción.
2. Calcular el parámetro `a = K × θ / τ`.
3. Aplicar reglas:

| Tipo | Kp | Ti | Td |
|---|---|---|---|
| **P** | 1 / a | — | — |
| **PI** | 0.9 / a | 3 × θ | — |
| **PID** | 1.2 / a | 2 × θ | 0.5 × θ |

### 4.3 Consideraciones para el flujo

- Los lazos de flujo suelen necesitar **poca acción integral** (Ti grande) porque responden rápido.
- El método Ziegler-Nichols tiende a producir **overshoot del 25-50%** (quarter-wave decay). Para aplicaciones donde se prefiere respuesta sin sobreimpulso, se pueden usar factores de seguridad (reducir Kp en 30-50% como punto de partida).
- Se recomienda un **período de muestreo (Ts)** de 50–200 ms para lazo de flujo con Arduino.

*Referencias:* [Wikipedia — Ziegler-Nichols method], [NI — Ziegler-Nichols Autotuning], [OptiControls — Ziegler-Nichols Closed-Loop Tuning Method], [IPCOS — Guide to PID Tuning]

---

## 5. Arquitectura del Sistema de Control

### 5.1 Diagrama de bloques funcional

```
                ┌──────────────────────────────────────────────────────┐
                │                   ARDUINO UNO                       │
                │                                                      │
SetPoint ──────▶│  ADC (Pot) ──▶ SP ──┐                               │
(Potenciómetro) │                      ▼                               │
                │               ┌──────────────┐                       │
                │               │  Controlador │                       │
                │               │  PI Discreto │───▶ PWM ──▶ Bomba DC ─┼──▶ Flujo
                │               └──────┬───────┘                       │
                │                      ▲                               │
                │                      │                               │
                │               ┌──────┴───────┐                       │
                │               │    Cálculo    │                       │
                │               │   Caudal     │                       │
                │   Interrupción│  (Q = F/7.5) │                       │
Sensor ────────▶│  ◀───────────┴──────────────┘                       │
(GFS401)        │                                                      │
                │                      LCD 16×2                        │
                │         SP: xx.x L/min  PV: xx.x L/min              │
                │         Error: xx.x      PWM: xxx                    │
                └──────────────────────────────────────────────────────┘
```

### 5.2 Componentes del sistema

| Componente | Función |
|---|---|
| Arduino Uno | Microcontrolador central, ejecuta el algoritmo de control |
| GFS401 (YF-S401) | Sensor de flujo Hall, retroalimentación (0.3–6 L/min) |
| Bomba DC | Actuador, genera el flujo de agua |
| Puente H (L298N / MOSFET) | Driver PWM para la bomba |
| Potenciómetro (10 kΩ) | Entrada analógica — SetPoint del usuario |
| LCD 16×2 + I2C | Visualización de variables del sistema |
| Fuente 12V / 5V | Alimentación de bomba, Arduino y sensores |

### 5.3 Modos de operación

- **Modo Manual (Open-Loop):** El usuario controla el PWM directamente para caracterización de la planta.
- **Modo Automático (Closed-Loop):** El controlador PI regula el caudal al valor de SetPoint.
- **Modo Perturbación:** Se obstruye parcialmente la tubería para evaluar la capacidad de rechazo de perturbaciones.

---

## 6. Criterios de Evaluación del Desempeño

| Criterio | Descripción | Fórmula |
|---|---|---|
| **Tiempo de subida (Tr)** | Tiempo para pasar de 10% a 90% del valor final | — |
| **Sobreimpulso (Mp)** | Porcentaje de exceso sobre el valor final | `(Mp / valor_final) × 100%` |
| **Tiempo de establecimiento (Ts)** | Tiempo hasta que la respuesta permanece dentro del ±2% del valor final | — |
| **Error estacionario (Ess)** | Diferencia entre SP y PV en estado estacionario | `|SP − PV|` |
| **IAE** | Integral del valor absoluto del error | `∫ |e(t)| dt` |
| **ISE** | Integral del error al cuadrado | `∫ e(t)² dt` |

---

## 7. Referencias Bibliográficas

1. **Ziegler, J. G. & Nichols, N. B.** (1942). Optimum Settings for Automatic Controllers. *ASME Transactions*, 64, 759–768.
2. **Åström, K. J. & Hägglund, T.** (2006). *Advanced PID Control*. ISA Society.
3. **Ogata, K.** (2010). *Modern Control Engineering* (5th ed.). Prentice Hall.
4. **Smith, C. A. & Corripio, A. B.** (2005). *Principles and Practice of Automatic Process Control* (3rd ed.). Wiley.
5. **Wang, L.** (2020). *PID Controller Implementation with Anti-windup Mechanisms*. RMIT University.
6. **Mantech.** YF-S401 High Precision PVC Water Flow Sensor Datasheet. https://www.mantech.co.za/Datasheets/Products/YF-S401_SEA.pdf
7. **DFRobot.** Water Flow Sensor SEN0216 (YF-S401). https://wiki.dfrobot.com/sen0216/
8. **APMonitor.** First Order Plus Dead Time (FOPDT). https://apmonitor.com/pdc/index.php/Main/FirstOrderSystems
9. **Rhinehart, R. R.** (2016). Testing a First-Order Plus Deadtime Model. *Control Global*.
10. **MathWorks.** PI Controller with Integral Anti-Windup. https://www.mathworks.com/help/sps/ref/picontrollerwithintegralantiwindupdiscreteorcontinuous.html
11. **Analog Devices.** Implementation of PI Controllers. Application Note ADSP-21990.
12. **OptiControls.** Ziegler-Nichols Closed-Loop Tuning Method. https://blog.opticontrols.com/archives/133

---

*Documento generado como parte del proyecto académico de Teoría Moderna de Control — Universidad Santiago Mariño — Junio 2026*
