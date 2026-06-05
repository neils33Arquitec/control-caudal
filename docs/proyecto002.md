# SISTEMA INTELIGENTE DE CONTROL DE CAUDAL MEDIANTE CONTROL PI EN LAZO CERRADO

## Universidad Santiago Mariño

### Asignatura: Teoría Moderna de Control

**Autores:**

* Neil Edickson Suarez Arevalo
* Jose Fabian Salas García

**Última actualización:** Sesión 002 — Junio 2026

---

# 1. PLANTEAMIENTO DEL PROBLEMA

En aplicaciones hidráulicas es común la necesidad de mantener un caudal constante independientemente de perturbaciones externas como:

* Variaciones de presión.
* Restricciones en la tubería.
* Cambios de voltaje de alimentación.
* Desgaste mecánico de la bomba.

Los sistemas tradicionales de control manual no garantizan precisión ni estabilidad.

Por ello se propone el desarrollo de un sistema de control automático capaz de regular el flujo de agua en tiempo real utilizando técnicas de Teoría Moderna de Control.

---

# 2. OBJETIVO GENERAL

Diseñar e implementar un sistema de control de caudal en lazo cerrado basado en un controlador PI capaz de mantener un flujo de agua deseado mediante la regulación automática de la velocidad de una bomba DC.

---

# 3. OBJETIVOS ESPECÍFICOS

## Objetivo 1

Medir el caudal instantáneo mediante un sensor de flujo digital.

## Objetivo 2

Permitir que el usuario establezca una consigna de flujo mediante un potenciómetro.

## Objetivo 3

Implementar un controlador PI discreto para minimizar el error entre el flujo deseado y el flujo real.

## Objetivo 4

Visualizar variables del sistema en una interfaz LCD.

## Objetivo 5

Evaluar la estabilidad y robustez del sistema frente a perturbaciones externas.

---

# 4. ARQUITECTURA GENERAL DEL SISTEMA

```text
                  SETPOINT
                (Potenciómetro)
                        │
                        ▼
                ┌─────────────┐
                │ Controlador │
                │     PI      │
                └──────┬──────┘
                       │
                 PWM de Control
                       │
                       ▼
                ┌─────────────┐
                │ Driver PWM  │
                │  Puente H   │
                └──────┬──────┘
                       │
                       ▼
                ┌─────────────┐
                │  Bomba DC   │
                └──────┬──────┘
                       │
                    Flujo
                       │
                       ▼
                ┌─────────────┐
                │ Sensor de   │
                │   Flujo     │
                └──────┬──────┘
                       │
                       ▼
                 Retroalimentación
                       │
                       └─────────────►
```

---

# 5. IDENTIFICACIÓN DEL SISTEMA DE CONTROL

## Entrada del Sistema

**SetPoint (SP)**

Representa el caudal deseado por el usuario.

Unidad:

```text
L/min
```

## Salida del Sistema

**Variable de Proceso (PV)**

Representa el caudal real medido por el sensor.

Unidad:

```text
L/min
```

## Variable Manipulada

**PWM aplicado a la bomba**

Unidad:

```text
0 - 255
```

o

```text
0% - 100%
```

## Error de Control

[
e(t)=SP(t)-PV(t)
]

---

# 6. MODELADO DE LA PLANTA

La planta está formada por:

* Bomba DC.
* Conducto hidráulico.
* Fluido.
* Sensor de flujo.

Desde el punto de vista dinámico se comporta como un sistema de primer orden con tiempo muerto (FOPDT).

[
G(s)=\frac{K e^{-\theta s}}{\tau s+1}
]

Donde:

* **K** = Ganancia de la planta.
* **τ** = Constante de tiempo.
* **θ** = Tiempo muerto.

---

# 7. JUSTIFICACIÓN DEL CONTROLADOR PI

## Acción Proporcional (Kp)

Permite responder rápidamente a cambios de error.

### Beneficios

* Menor tiempo de subida.
* Respuesta inmediata.

## Acción Integral (Ki)

Permite eliminar el error permanente.

### Beneficios

* Error estacionario prácticamente nulo.
* Compensación automática de pérdidas hidráulicas.

## Razón para no utilizar la acción derivativa (D)

El sensor de flujo genera ruido inherente.

La acción derivativa:

* Amplifica el ruido.
* Produce oscilaciones.
* Reduce la estabilidad del sistema.

Por esta razón se selecciona un controlador PI.

---

# 8. METODOLOGÍA DE DESARROLLO

## Fase I — Caracterización de la Planta

Pruebas en lazo abierto:

* PWM 25 %
* PWM 50 %
* PWM 75 %
* PWM 100 %

Variables registradas:

* Caudal.
* Tiempo de estabilización.
* Respuesta dinámica.

Resultado esperado:

Obtención de la curva característica de la bomba.

---

## Fase II — Implementación del Sistema de Medición

Integración de:

* Sensor de flujo.
* Arduino.
* Conversión de pulsos a L/min.

Validación de la exactitud de medición.

---

## Fase III — Implementación del Control PI

Etapas:

1. Lectura del SetPoint.
2. Lectura del flujo real.
3. Cálculo del error.
4. Acción proporcional.
5. Acción integral.
6. Generación del PWM.

---

## Fase IV — Sintonización

Métodos:

* Ziegler-Nichols.
* Ajuste experimental.

Parámetros:

* Kp
* Ki

---

## Fase V — Validación

### Cambio de SetPoint

Verificar seguimiento de referencia.

### Perturbación Externa

Obstrucción parcial de la tubería.

### Saturación

Comprobación del algoritmo Anti-Windup.

---

# 9. INTERFAZ HOMBRE-MÁQUINA (HMI)

Variables mostradas en la LCD:

### Línea 1

```text
SP = Flujo Deseado
PV = Flujo Real
```

### Línea 2

```text
Error
PWM Aplicado
```

---

# 10. CRITERIOS DE EVALUACIÓN

* Tiempo de subida.
* Sobreimpulso.
* Tiempo de establecimiento.
* Error estacionario.
* Rechazo de perturbaciones.
* Estabilidad del sistema.

---

# 11. APORTES DE TEORÍA MODERNA DE CONTROL

Este proyecto demuestra la aplicación práctica de:

* Sistemas SISO.
* Lazo cerrado.
* Retroalimentación negativa.
* Modelado dinámico de plantas.
* Sistemas discretos.
* Control digital.
* Estabilidad.
* Rechazo de perturbaciones.
* Control PI industrial.

---

# CONCLUSIÓN

El proyecto se presenta como un **Sistema Embebido de Control de Caudal en Tiempo Real basado en un Controlador PI Digital**, diseñado bajo principios de Teoría Moderna de Control, capaz de regular automáticamente el flujo hidráulico mediante realimentación continua y compensación automática de perturbaciones externas.