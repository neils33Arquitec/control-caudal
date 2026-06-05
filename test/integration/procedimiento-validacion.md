# Procedimiento de Validación — Sistema de Control de Caudal

## Equipo necesario

- Sistema armado completo (Arduino + GFS401 + L298N + bomba 12V + LCD + potenciómetro)
- Fuente 12V
- Computador con monitor serial
- Cronómetro
- Obstrucción parcial para prueba de perturbación (pinza para manguera)

---

## Prueba 1: Respuesta al escalón (cambio de SetPoint)

### Procedimiento

1. Conectar Arduino por USB y abrir monitor serial.
2. Girar potenciómetro al mínimo (SP = 0 L/min).
3. Esperar 10 segundos para estabilización.
4. Girar potenciómetro a 50% (SP ≈ 3.0 L/min) de forma brusca.
5. Registrar datos por 30 segundos.

### Métricas a evaluar

| Métrica | Fórmula | Aceptable | Bueno |
|---|---|---|---|
| Tiempo de subida (Tr) | 10% → 90% del valor final | < 5 s | < 3 s |
| Sobreimpulso (Mp) | (pico − final) / final × 100% | < 20% | < 10% |
| Tiempo de establecimiento (Ts) | Dentro de ±2% del valor final | < 15 s | < 10 s |
| Error estacionario (Ess) | |SP − PV| en estado estable | < 0.3 L/min | < 0.1 L/min |

---

## Prueba 2: Rechazo de perturbaciones

### Procedimiento

1. Fijar SP en 3.0 L/min (potenciómetro al 50%).
2. Esperar a que el sistema se estabilice (> 15 s).
3. Aplicar obstrucción parcial en la manguera (apretar con pinza ~50%).
4. Mantener la obstrucción por 5 segundos, luego soltar.
5. Registrar datos por 30 segundos.

### Métricas

| Métrica | Aceptable |
|---|---|
| Desviación máxima | < 1.0 L/min |
| Tiempo de recuperación | < 8 s |
| Error estacionario post-perturbación | < 0.2 L/min |

---

## Prueba 3: Rango completo del SetPoint

### Procedimiento

1. Girar potenciómetro lentamente de mínimo a máximo (0 → 6 L/min).
2. Observar que el caudal real siga al SetPoint en todo el rango.
3. Girar de máximo a mínimo.
4. Verificar que la bomba se apague (PWM = 0) cuando SP ≈ 0.

### Criterio

- El caudal debe seguir la referencia con error < 0.5 L/min en todo el rango.
- Sin oscilaciones sostenidas en ningún punto de operación.

---

## Prueba 4: Anti-Windup

### Procedimiento

1. Fijar SP en 5.0 L/min.
2. Con la bomba funcionando, cerrar completamente la manguera (obstrucción total).
3. Mantener 10 segundos.
4. Soltar la obstrucción abruptamente.
5. Observar la respuesta.

### Criterio

- No debe haber sobreimpulso excesivo (> 2 L/min por encima del SP) al liberar la obstrucción.
- El sistema debe estabilizarse en < 10 s después de liberar.

---

## Registro de resultados

| Prueba | Tr (s) | Mp (%) | Ts (s) | Ess (L/min) | Observaciones |
|---|---|---|---|---|---|
| 1. Escalón 0→3 L/min | | | | | |
| 2. Perturbación | — | — | | | Desviación: __ L/min |
| 3. Rango completo | — | — | — | — | |
| 4. Anti-Windup | — | — | | | |

---

## Criterios de aceptación

El sistema se considera validado si:

- [ ] Tr < 5 s en prueba de escalón
- [ ] Mp < 20%
- [ ] Ts < 15 s
- [ ] Ess < 0.3 L/min
- [ ] Recuperación ante perturbación < 8 s
- [ ] Sin oscilaciones sostenidas
- [ ] Anti-Windup funciona correctamente
