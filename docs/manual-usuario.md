# Manual de Usuario

## Sistema Inteligente de Control de Caudal

---

## 1. Descripción general

Este sistema regula automáticamente el flujo de agua mediante un **controlador PI digital**. El usuario define el caudal deseado (SetPoint) girando un potenciómetro, y el sistema ajusta la velocidad de la bomba para mantener ese caudal constante, incluso ante perturbaciones externas.

---

## 2. Componentes del sistema

```
 ┌──────────┐     ┌──────────┐     ┌──────────┐     ┌──────────┐
 │          │     │          │     │          │     │          │
 │Potencióm.│────▶│ Arduino  │────▶│  L298N   │────▶│  Bomba   │───▶ Flujo
 │(SetPoint)│     │  + PI    │     │(Puente H)│     │  12V DC  │
 └──────────┘     └────┬─────┘     └──────────┘     └──────────┘
                       │                                    │
                       │    ┌──────────┐                    │
                       │    │  LCD 16×2│                    │
                       │    │ (SP/PV)  │                    │
                       │    └──────────┘                    │
                       │                                    │
                       └──────────┬─────────────────────────┘
                                  │
                           ┌──────┴──────┐
                           │  GFS401     │
                           │ (Sensor)    │
                           └─────────────┘
```

---

## 3. Puesta en marcha

### 3.1 Conexiones iniciales

1. Conectar la **fuente 12V** al módulo L298N y al Arduino (Vin).
2. Conectar el **cable USB** del Arduino al computador (alimenta la lógica de control).
3. Verificar que el **potenciómetro** esté girado al mínimo (posición 0).

### 3.2 Encendido

Al conectar la alimentación:
1. El LCD muestra el mensaje **"Sistema Caudal"** y luego **"Iniciando..."** durante 1.5 segundos.
2. Pasada la inicialización, el LCD muestra en la **línea superior**:
   ```
   SP:  0.0  PV:  0.0
   ```
   Y en la **línea inferior**:
   ```
   E:  0.0  PWM:   0
   ```

3. La bomba permanece apagada hasta que se gire el potenciómetro.

---

## 4. Interfaz de usuario

### 4.1 Pantalla LCD

```
┌─────────────────────────────────┐
│ SP:  3.0  PV:  2.9              │  ← Línea 1
│ E:   0.1  PWM: 150              │  ← Línea 2
└─────────────────────────────────┘
```

| Indicador | Significado | Rango |
|---|---|---|
| **SP** | SetPoint — caudal deseado | 0.0 – 6.0 L/min |
| **PV** | Process Variable — caudal real | 0.0 – 6.0 L/min |
| **E** | Error (SP − PV) | −6.0 – 6.0 |
| **PWM** | Señal aplicada a la bomba | 0 – 255 |

### 4.2 Potenciómetro (control de caudal)

Girando el potenciómetro se define el caudal deseado:

| Posición | SetPoint (L/min) |
|---|---|
| Mínimo (0°) | 0.0 |
| 25% | ~1.5 |
| 50% | ~3.0 |
| 75% | ~4.5 |
| Máximo (270°) | 6.0 |

**Regla práctica:** A mitad de vuelta, caudal medio (~3 L/min).

### 4.3 Monitor serial (opcional)

Para ver datos detallados en tiempo real:

1. Conectar el Arduino por USB.
2. Abrir **Arduino IDE → Herramientas → Monitor Serial** (9600 baud).
3. Se mostrarán líneas como:
   ```
   SP:3.00,PV:2.85,E:0.15,PWM:142
   SP:3.00,PV:2.92,E:0.08,PWM:138
   SP:3.00,PV:2.97,E:0.03,PWM:135
   ```

Formato: `SP:<valor>,PV:<valor>,E:<valor>,PWM:<valor>`

---

## 5. Operación normal

### 5.1 Estabilización

Después de cambiar el SetPoint:

| Tiempo | Evento |
|---|---|
| 0 – 2 s | La bomba acelera o desacelera |
| 2 – 10 s | El caudal se aproxima al SetPoint |
| 10 – 15 s | El sistema se estabiliza en el valor deseado |

### 5.2 Cambio de caudal deseado

Girar el potenciómetro lentamente. El sistema responde automáticamente ajustando la velocidad de la bomba.

> **Nota:** Si se gira muy rápido, puede haber un ligero sobreimpulso temporal. Esto es normal y el controlador lo corrige en segundos.

### 5.3 Apagado

1. Girar el potenciómetro al mínimo (SP = 0).
2. Esperar 5 segundos a que la bomba se detenga.
3. Desconectar la fuente 12V.
4. Desconectar el USB.

---

## 6. Respuesta ante perturbaciones

El sistema está diseñado para mantener el caudal constante aunque ocurran cambios externos:

| Perturbación | Comportamiento esperado |
|---|---|
| **Obstrucción parcial** (apretar manguera) | El caudal baja momentáneamente, el PI aumenta el PWM para compensar. En < 8 s vuelve al SetPoint. |
| **Obstrucción total** (cerrar manguera) | El caudal cae a 0. El PI aumenta PWM al máximo (255). Al liberar, el caudal puede sobrepasar el SetPoint brevemente y luego estabilizarse. |
| **Variación de voltaje** | El PI compensa automáticamente el cambio en la velocidad de la bomba. |

---

## 7. Indicadores de estado

| Situación | LCD | Acción recomendada |
|---|---|---|
| SP = 0, PV = 0 | PWM = 0 | Normal — sistema en reposo |
| SP > PV | PWM aumentando | Normal — el sistema está acelerando |
| SP < PV | PWM disminuyendo | Normal — el sistema está desacelerando |
| PWM = 255, PV < SP | Saturación | El sistema llegó al límite de la bomba. Reducir SP. |
| PV salta a 0 con bomba funcionando | 0.0 | Verificar que el sensor GFS401 tenga agua y la flecha apunte en dirección del flujo. |

---

## 8. Mantenimiento

### 8.1 Limpieza del sensor GFS401

Con el tiempo, el impulsor del sensor puede acumular residuos. Si las lecturas son erráticas:

1. Desconectar la alimentación.
2. Desmontar el sensor del circuito de mangueras.
3. Limpiar el interior con agua limpia.
4. Volver a instalar respetando la dirección de flujo.

### 8.2 Verificación del nivel de agua

La bomba no debe funcionar en seco. Asegurarse de que:

- La bomba esté sumergida o cebada antes de encender.
- No haya aire en las mangueras (purgar si es necesario).

---

## 9. Solución de problemas rápida

| Problema | Causa probable | Solución |
|---|---|---|
| LCD apagado | Sin alimentación 5V | Verificar cable USB |
| LCD muestra `###` | Dirección I2C incorrecta | Usar scanner I2C para encontrar la dirección |
| Bomba no gira | Sin PWM o dirección incorrecta | Verificar ENA, IN1, IN2 en el L298N |
| SP no cambia | Potenciómetro mal conectado | Verificar cable al pin A0 |
| PV siempre 0 | Sensor desconectado o flujo inverso | Verificar cable amarillo al pin 2 y dirección de flecha |
| Caudal oscila | Kp demasiado alto | Reducir Kp en `config.h` |
| Error permanente alto | Ki demasiado bajo | Aumentar Ki en `config.h` |

---

## 10. Especificaciones técnicas

| Parámetro | Valor |
|---|---|
| Rango de caudal | 0.0 – 6.0 L/min |
| Precisión típica | ±0.3 L/min |
| Tiempo de respuesta | < 5 s (10% → 90%) |
| Sobreimpulso máximo | < 20% (típico < 10%) |
| Error estacionario | < 0.1 L/min |
| Voltaje de alimentación | 12V DC (bomba) + 5V USB (lógica) |
| Potencia máxima | ~12W (bomba + lógica) |
| Temperatura de operación | 0 – 60 °C |

---

## 11. Contacto

Proyecto desarrollado para la asignatura **Teoría Moderna de Control** de la **Universidad Santiago Mariño**.

**Autores:**
- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

*Junio 2026*
