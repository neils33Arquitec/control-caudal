# Modelo Simulink — Lazo de Control de Caudal

## Descripción del modelo

Este documento describe la estructura del modelo Simulink `lazo_control.slx` para simular el sistema de control de caudal antes de implementarlo en Arduino.

## Bloques del modelo

```
                      ┌──────────┐
                      │  Paso    │
                      │(SetPoint)│
                      └─────┬────┘
                            │
                            ▼
                    ┌───────────────┐
                    │  Controlador  │
                    │  PI Discreto  │
                    │  Kp, Ki, Ts   │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐
                    │  Saturación   │
                    │  0 – 255      │
                    └───────┬───────┘
                            │
                            ▼
                    ┌───────────────┐       ┌────────────┐
                    │  Planta FOPDT │       │  Retardo   │
                    │  K/(τs+1)     │──────▶│  e^(−θs)   │
                    └───────────────┘       └──────┬─────┘
                                                   │
                                                   ▼
                                            ┌──────────────┐
                                            │  Sensor      │
                                            │  Ganancia    │
                                            └──────┬───────┘
                                                   │
                                                   └───────────►
                                                         │
                                                    (realimentación)
```

## Parámetros

| Bloque | Parámetro | Valor |
|---|---|---|
| Step | Step time | 2 s |
| Step | Final value | 3.0 (L/min) |
| PI Discreto | Proportional gain (Kp) | 2.0 |
| PI Discreto | Integral gain (Ki) | 0.5 |
| PI Discreto | Sample time (Ts) | 0.1 s |
| Saturación | Lower limit | 0 |
| Saturación | Upper limit | 255 |
| Planta FOPDT | Gain (K) | 0.02 |
| Planta FOPDT | Time constant (τ) | 3.2 s |
| Transport Delay | Time delay (θ) | 1.5 s |

## Cómo crear el modelo

1. Abrir MATLAB → Simulink → modelo en blanco.
2. Agregar bloques desde la librería:
   - `Sources` → `Step`
   - `Continuous` → `Transfer Fcn` (para G(s))
   - `Continuous` → `Transport Delay`
   - `Math Operations` → `Saturation`
   - `Discrete` → `Discrete PID Controller` (modo PI)
   - `Sinks` → `Scope`
   - `Math Operations` → `Sum` (para el error)
3. Configurar parámetros según la tabla.
4. Guardar como `lazo_control.slx` en esta carpeta.

## Resultados esperados

- Respuesta estable sin overshoot excesivo.
- Error estacionario < 0.1 L/min.
- Tiempo de establecimiento < 10 s.
- Rechazo de perturbaciones con recuperación en < 5 s.
