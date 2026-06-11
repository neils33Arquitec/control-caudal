# Protocolo de Sesiones

## Sistema de Control de Caudal PI — Memoria de Recuperación

---

## Índice de sesiones

| Sesión | Fecha | Descripción |
|---|---|---|
| [Sesión 001](#sesión-001) | Junio 2026 | Creación del proyecto, estructura, marco teórico |
| [Sesión 002](#sesión-002) | Junio 2026 | Documentación de hardware, BOM, wiring |
| [Sesión 003](#sesión-003) | Junio 2026 | Código fuente, scripts, tests, archivos raíz, GitHub, assets, manuales |
| [Sesión 004](#sesión-004) | Junio 2026 | Nota técnica: corrección de escalado PI en prototipo real |

---

## Protocolo general

### Formato de registro

Cada sesión debe documentar:

```
## Sesión XXX — Fecha: DD/MM/AAAA

### Objetivo
¿Qué se buscaba lograr?

### Participantes
- Nombre del integrante 1
- Nombre del integrante 2

### Archivos creados
Lista de archivos generados en la sesión.

### Archivos modificados
Lista de archivos editados.

### Decisiones técnicas
Decisiones importantes tomadas durante la sesión.

### Problemas encontrados
Problemas y cómo se resolvieron.

### Pendientes
Tareas que quedan para la próxima sesión.

### Commits (si aplica)
- hash: mensaje
```

### Convenciones

- **Idioma:** Español (consistente con el resto del proyecto).
- **Numeración:** Sesiones correlativas (001, 002, 003...).
- **Archivos:** Cada sesión se registra en este mismo documento, agregando al índice y al contenido.
- **Estados:** Cada sesión puede tener estado `Completada`, `En progreso` o `Pendiente`.

---

## Sesión 001

- **Fecha:** Junio 2026
- **Estado:** Completada

### Objetivo
Iniciar el proyecto y establecer la estructura base del repositorio siguiendo estándares profesionales de presentación en GitHub.

### Participantes
- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

### Archivos creados

| Archivo | Descripción |
|---|---|
| `docs/proyecto002.md` | Documento original del proyecto (planteamiento, objetivos, arquitectura, metodología) |
| `docs/structure.md` | Árbol de directorios del proyecto con descripción de cada carpeta |
| `docs/technical-documentation.md` | Marco teórico completo: sensor GFS401, FOPDT, PI discreto, anti-windup, Ziegler-Nichols, criterios de evaluación, 12 referencias |

### Archivos modificados
Ninguno (proyecto nuevo).

### Decisiones técnicas

| Decisión | Justificación |
|---|---|
| Sensor GFS401 en lugar de YF-S201 | Definido por el usuario. Rango 0.3–6 L/min, 5880 pulsos/L. |
| Controlador PI sin acción derivativa | El sensor de flujo genera ruido que la acción derivativa amplifica. |
| Modelo FOPDT para la planta | La bomba DC + tubería + sensor se comporta como un sistema de primer orden con tiempo muerto. |
| Anti-windup por clamping | Técnica simple de implementar en Arduino, efectiva para evitar saturación del integrador. |
| Sintonización Ziegler-Nichols | Método heurístico estándar para obtener valores iniciales de Kp y Ki. |

### Problemas encontrados
Ninguno.

### Pendientes
- [x] Estructura de carpetas (`src/`, `hardware/`, `scripts/`, `test/`, `.github/`)
- [x] Documentación de hardware (GFS401, L298N, bomba, LCD, potenciómetro, wiring, BOM)
- [ ] Código fuente del firmware
- [ ] Scripts de simulación
- [ ] Tests y validación
- [ ] Archivos raíz (README, LICENSE, .gitignore)
- [ ] CI/CD y templates de GitHub
- [ ] Assets (diagramas, fotos)

---

## Sesión 002

- **Fecha:** Junio 2026
- **Estado:** Completada

### Objetivo
Completar la documentación de hardware y establecer la lista de materiales definitiva.

### Participantes
- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

### Archivos creados

| Archivo | Descripción |
|---|---|
| `hardware/schematics/gfs401-datasheet.md` | Datasheet del sensor: especificaciones, pinout, fórmula de caudal, instalación |
| `hardware/schematics/l298n-module.md` | Documentación del L298N: pines, control de dirección y velocidad, conexiones |
| `hardware/schematics/bomba-12v.md` | Especificaciones de la bomba DC 12V, protecciones (flyback, capacitor) |
| `hardware/schematics/lcd-i2c.md` | Documentación del LCD 16×2 + I2C: dirección, librerías, layout de pantalla |
| `hardware/schematics/potenciometro-setpoint.md` | Potenciómetro de SetPoint: rango 0–6 L/min, tabla de valores |
| `hardware/schematics/wiring-diagram.md` | Diagrama de conexiones ASCII completo con tabla de pines y alimentación |
| `hardware/bom/bill-of-materials.md` | BOM con 15 componentes, cantidades, especificaciones y costo estimado ($35–65 USD) |

### Archivos modificados
- `hardware/schematics/wiring-diagram.md` — Agregadas conexiones específicas del L298N (ENA, IN1, IN2), removido pulsador de modo manual
- `hardware/bom/bill-of-materials.md` — Actualizado con bomba 12V y L298N específicos

### Decisiones técnicas

| Decisión | Justificación |
|---|---|
| Potenciómetro de función única (SetPoint) | El usuario descartó la doble función Auto/Manual. El potenciómetro solo define el caudal deseado. |
| Pines IN1=HIGH, IN2=LOW fijos | La bomba de agua solo gira en un sentido. No se necesita control de dirección. |
| ENA desde pin 9 (PWM) | El pin 9 del Arduino Uno es compatible con `analogWrite()` para generar PWM. |

### Problemas encontrados
- El BOM tenía numeración incorrecta en los componentes pasivos (salto de #9 a #12). Corregido a secuencia #10–#15.
- La configuración inicial del proyecto usaba el sensor YF-S201. Reemplazado por GFS401 con sus parámetros reales (5880 pulsos/L, 0.3–6 L/min).

### Pendientes
- [x] Código fuente del firmware y librerías
- [x] Scripts de simulación
- [x] Tests y validación
- [x] Archivos raíz (README, LICENSE, .gitignore)
- [x] CI/CD y templates de GitHub
- [x] Assets (diagramas, fotos)
- [ ] Manuales de montaje y usuario

---

## Sesión 003

- **Fecha:** Junio 2026
- **Estado:** Completada

### Objetivo
Completar la totalidad del proyecto: código fuente funcional, scripts de simulación, tests, archivos raíz, integración continua, assets visuales y manuales de usuario.

### Participantes
- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

### Archivos creados (17 archivos)

#### Código fuente (src/)

| Archivo | Descripción |
|---|---|
| `src/firmware/control-caudal.ino` | Sketch principal de Arduino. `setup()`: init LCD, pines, interrupción. `loop()`: lectura SP/PV, PI → PWM, LCD, serial cada 1s. |
| `src/firmware/PIController.h` | Clase `PIController` con métodos: `compute()`, `reset()`, `setGains()`, `getIntegral()`. |
| `src/firmware/PIController.cpp` | Implementación: PI discreto por Backward Euler, anti-windup por clamping con saturación condicional del integrador. |
| `src/firmware/config.h` | Constantes del sistema: pines (2, 7, 8, 9, A0), Kp=2.0, Ki=0.5, Ts=0.1s, 5880 pulsos/L, LCD 0x27. |

#### Scripts (scripts/)

| Archivo | Descripción |
|---|---|
| `scripts/matlab/identificar_fopdt.m` | Identificación de planta FOPDT a partir de respuesta al escalón. Calcula K, τ, θ gráficamente. |
| `scripts/matlab/sintonizar_ZN.m` | Cálculo de Kp y Ki por Ziegler-Nichols (lazo abierto) a partir de parámetros FOPDT. Simula lazo cerrado. |
| `scripts/python/log_datos.py` | Captura datos seriales desde Arduino y guarda en CSV. Uso: `python log_datos.py COM3 30`. |
| `scripts/python/graficar_respuesta.py` | Lee CSV y grafica SP, PV y PWM en dos paneles. |
| `scripts/simulink/modelo_lazo_control.md` | Documentación del modelo Simulink con descripción de bloques y parámetros. |

#### Tests (test/)

| Archivo | Descripción |
|---|---|
| `test/unit/test_pi_controller.cpp` | 8 tests unitarios: inicialización, error cero, error positivo, saturación superior/inferior, anti-windup, cambio de ganancias, reset. |
| `test/integration/procedimiento-validacion.md` | 4 pruebas de validación: escalón, perturbación, rango completo, anti-windup. Con tabla de registro y criterios de aceptación. |

#### Archivos raíz

| Archivo | Descripción |
|---|---|
| `README.md` | Descripción, diagrama de bloques, pinout rápido, instalación, autores. |
| `LICENSE` | Licencia MIT. |
| `.gitignore` | Reglas para Arduino (.o, .elf, build/), Python (__pycache__), MATLAB (.asv), Simulink (.slxc), VS Code, Windows. |

#### GitHub (.github/)

| Archivo | Descripción |
|---|---|
| `.github/workflows/arduino-compile.yml` | CI que compila `control-caudal.ino` con `arduino-cli` en pushes y PRs a main. |
| `.github/ISSUE_TEMPLATE/bug_report.md` | Template con secciones: descripción, pasos, comportamiento esperado/actual, hardware, datos seriales. |
| `.github/ISSUE_TEMPLATE/feature_request.md` | Template con: problema relacionado, solución propuesta, alternativas consideradas. |

#### Assets (assets/)

| Archivo | Descripción |
|---|---|
| `assets/diagrama-bloques.md` | Diagrama de bloques en sintaxis Mermaid con señales del sistema y flujo de control. |

#### Manuales (docs/)

| Archivo | Descripción |
|---|---|
| `docs/manual-montaje.md` | 13 secciones: lista de materiales, herramientas, 9 pasos detallados de montaje, diagrama ASCII, solución de problemas (6 casos). |
| `docs/manual-usuario.md` | 11 secciones: inicio, interfaz LCD, operación normal, perturbaciones, mantenimiento, solución rápida, especificaciones. |

### Archivos modificados

| Archivo | Cambio |
|---|---|
| `hardware/bom/bill-of-materials.md` | Numeración de componentes pasivos corregida (#12→#17 → #10→#15). |
| `src/firmware/config.h` | Eliminadas macros no utilizadas (`CAUDAL_MAXIMO`, `PIN_LCD_SDA`, `PIN_LCD_SCL`). |
| `test/unit/test_pi_controller.cpp` | Ruta de includes corregida. |
| `docs/structure.md` | Actualizado el árbol de directorios para reflejar el estado final del proyecto. |

### Decisiones técnicas

| Decisión | Justificación |
|---|---|
| PI con anti-windup por clamping | El integrador se satura condicionalmente usando los límites de salida menos Kp*error. Previene windup sin complejidad adicional. |
| Ts = 100 ms | La dinámica del flujo (τ ~2–5 s) permite un período de muestreo de 100 ms. Suficiente para lazo cerrado estable. |
| Serial cada 1000 ms, control cada 100 ms | Separar la salida serial del lazo de control evita que la impresión serial afecte el tiempo de muestreo. |
| Kp = 2.0, Ki = 0.5 como valores iniciales | Valores de partida conservadores. Deberán sintonizarse experimentalmente con Ziegler-Nichols. |
| LCD con librería LiquidCrystal I2C | Reduce el uso de pines a solo 2 (SDA/SCL) y simplifica el cableado. |

### Problemas encontrados

| Problema | Solución |
|---|---|
| Número de archivos del proyecto desactualizado en structure.md | Se reescribió `structure.md` con el estado final (30 archivos, tabla resumen). |
| BOM con numeración saltada | Los componentes pasivos comenzaban en #12 pero la tabla principal terminaba en #9. Corregido a secuencia continua. |
| Config redundante | `config.h` tenía macros definidas pero no referenciadas en ningún lado. Limpiadas para mantener el archivo mínimo. |
| Ruta de include en test | El test C++ hacía referencia a directorios que no existían en la estructura. Corregido a `-I../src/lib`. |

### Pendientes
- [ ] Sintonización fina de Kp y Ki tras pruebas reales.
- [ ] Capturar fotos del prototipo armado y guardarlas en `assets/fotos/`.
- [ ] Ejecutar pruebas de validación y registrar resultados en `test/integration/procedimiento-validacion.md`.
- [ ] Generar gráficos de respuesta con `graficar_respuesta.py` y guardarlos en `assets/graficos/`.
- [ ] Ajustar dirección I2C del LCD si es distinta a 0x27.

---

## Sesión 004 — Nota técnica: Corrección de escalado PI

- **Fecha:** Junio 2026
- **Estado:** Pendiente (cambios sin aplicar)

### Objetivo
Documentar el problema de escalado detectado durante la prueba del prototipo real y la solución propuesta para su implementación en el próximo montaje.

### Participantes
- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

### Problema detectado en prototipo real
Durante la prueba del prototipo, la bomba se activó mostrando:

| Variable | Valor | Comentario |
|---|---|---|
| SP | 0.6 L/min | SetPoint desde el potenciómetro |
| PV | ~0.04 L/min | Caudal real (0.2 con 1 decimal en LCD) |
| Error | 0.56 L/min | SP - PV |
| PWM | 150 | Salida al motor |

**Causa raíz:** El controlador PI opera en L/min pero su salida se usa directamente como PWM (0-255) sin escalar. Con KP=2.0, un error de 0.56 L/min produce solo ~1 PWM de corrección:

```
P term = KP × error = 2.0 × 0.56 ≈ 1.12 PWM
```

La bomba no se mueve con PWM 1 → el error persiste → la integral acumula durante minutos → cuando finalmente el PWM supera el umbral de arranque de la bomba, el integrador ya acumuló un valor desproporcionado (PWM 150 para un SP de 0.6 L/min).

### Solución propuesta: Escalado de salida del PI

El PI debe trabajar internamente en L/min (0 a SP_MAX) y escalar su salida a PWM al final.

| Archivo | Cambio |
|---|---|
| `src/firmware/control-caudal.ino:7` | `PIController pi(KP, KI, TS, 0.0, SP_MAX)` — límites en L/min |
| `src/firmware/control-caudal.ino:77` | `float pi_out = pi.compute(sp, pv); pwm_out = (int)((pi_out / SP_MAX) * PWM_MAX);` |
| `src/firmware/control-caudal.ino` | Añadir detección de cambio de setpoint → `pi.reset()` |
| `test/unit/test_pi_controller.cpp` | Actualizar límites de saturación de 0-255 a 0-SP_MAX |

### Verificación con Ziegler-Nichols

El script `scripts/matlab/sintonizar_ZN.m` modela la planta con ganancia K=0.02 (L/min)/PWM. Con el escalado (×42.5), la ganancia efectiva desde la perspectiva del PI es:

```
K_efectiva = 0.02 × (255/6) ≈ 0.85 (L/min) / (L/min)
```

Aplicando Ziegler-Nichols con K_efectiva:

| Parámetro | ZN teórico | Efectivo con KP=2.0, KI=0.5 |
|---|---|---|
| KP efectivo | ~2.26 | 2.0 |
| KI efectivo | ~0.50 | 0.5 |

Los valores actuales (KP=2.0, KI=0.5) están muy cerca del óptimo ZN una vez aplicado el escalado.

### Pendientes
- [x] Aplicar cambios en `src/firmware/control-caudal.ino` ← completado en Sesión 005
- [x] Actualizar `test/unit/test_pi_controller.cpp` ← completado en Sesión 005
- [ ] Probar en prototipo real y verificar respuesta
- [ ] Ajustar KP/KI finamente si es necesario

---

## Sesión 005 — Implementación de mejoras y corrección de bugs

- **Fecha:** Junio 2026
- **Estado:** Completada

### Objetivo
Implementar todas las mejoras documentadas en `sesiones/plan-mejoras.md` (Sesión 004), realizar una revisión completa del proyecto y corregir los bugs detectados durante la revisión.

### Participantes
- Neil Edickson Suarez Arevalo
- Jose Fabian Salas García

### Archivos modificados

#### Firmware

| Archivo | Cambios |
|---|---|
| `src/firmware/control-caudal.ino` | 4 correcciones (ver detalle abajo) |
| `ArduinoIDEcontrol-caudal/control-caudal.ino` | Mismas 4 correcciones (copia sincronizada) |
| `test/unit/test_pi_controller.cpp` | Límites de saturación actualizados de 255 a 6.0 |

#### Documentación

| Archivo | Cambios |
|---|---|
| `docs/manual-usuario.md` | Sección 4.1 reescrita con ciclo de 3 mensajes LCD; sección 5.2 con nota del reset de integral |
| `docs/technical-documentation.md` | Fórmula del sensor corregida (F/7.5→F/98); pseudocódigo actualizado con límites en L/min y paso de escalado; diagrama LCD actualizado |
| `assets/diagrama-bloques.md` | Flujo de control actualizado (paso de escalado, reset de SP, ciclo LCD); lazo de control actualizado |
| `docs/structure.md` | Árbol de directorios actualizado con carpetas `ArduinoIDEcontrol-caudal/` y `sesiones/`; conteo corregido a 36 archivos |
| `README.md` | Nueva sección "Comportamiento del LCD" con tabla de los 3 mensajes |

#### Documentación nueva

| Archivo | Descripción |
|---|---|
| `docs/nota-tecnica-ganancias-pi.md` | Nota técnica explicando KP y KI con valores reales del sistema |

---

### Cambios en el firmware — detalle

#### 1. Corrección de escalado del PI (de plan-mejoras.md)

**Problema:** El PI usaba límites 0–255 (PWM) en lugar de 0–SP_MAX (L/min), produciendo correcciones de ~1 PWM para errores pequeños. La integral acumulaba por minutos antes de que la bomba respondiera.

**Solución aplicada:**
```cpp
// Antes:
PIController pi(KP, KI, TS, PWM_MIN, PWM_MAX);
pwm_out = (int)pi.compute(setpoint, caudal);

// Después:
PIController pi(KP, KI, TS, 0.0, SP_MAX);
float pi_out = pi.compute(setpoint, caudal);
pwm_out = (int)((pi_out / SP_MAX) * (float)PWM_MAX);
```

#### 2. Reset del integrador por cambio de setpoint (de plan-mejoras.md)

Cuando el usuario gira el potenciómetro más de 0.05 L/min, el integrador se reinicia para evitar que la acción acumulada del setpoint anterior afecte la respuesta al nuevo valor.

```cpp
if (fabs(setpoint - setpoint_anterior) > 0.05) {
    pi.reset();
    setpoint_anterior = setpoint;
}
```

#### 3. Ciclo de 3 mensajes en el LCD (de plan-mejoras.md)

El LCD 16×2 rota su contenido entre 3 mensajes cada 10 segundos usando `millis()`, sin bloquear el lazo de control:

| Tiempo | Contenido |
|---|---|
| 0–6 s | SP, PV, Error, PWM |
| 6–8 s | ADC, voltaje del potenciómetro, SP |
| 8–10 s | PWM absoluto (0-255) y porcentaje |

#### 4. Corrección del cálculo de caudal (bug detectado en revisión)

**Problema:** `calcular_caudal()` se llama cada TS=0.1 s y resetea el contador de pulsos. La fórmula original `(p * 60) / PULSOS_POR_LITRO` asume `p` = pulsos por segundo, pero `p` son pulsos en 100 ms. El caudal se leía **10 veces menor al real**.

```cpp
// Antes:
return (p * 60.0) / PULSOS_POR_LITRO;

// Después:
return (p * 60.0) / (PULSOS_POR_LITRO * TS);
```

Este bug explica parcialmente la lectura PV ≈ 0.04 L/min observada en el prototipo (debería haber sido ~0.4 L/min).

---

### Decisiones técnicas

| Decisión | Justificación |
|---|---|
| No cambiar la clase `PIController` | El bug de caudal está en la capa de lectura del sensor, no en el controlador. El PI en sí es correcto. |
| Usar `PULSOS_POR_LITRO * TS` en la fórmula | Hace explícita la dependencia del período de muestreo, facilitando ajustes futuros de TS. |
| Redacción "3 mensajes" en lugar de "3 pantallas" | Corrección semántica: es una sola pantalla LCD física que rota su contenido. |

---

### Problemas encontrados

| Problema | Causa | Solución |
|---|---|---|
| PV leía 10× por debajo del valor real | `calcular_caudal()` no dividía por TS | Corregido: `(p * 60) / (PULSOS_POR_LITRO * TS)` |
| Fórmula `Q = F/7.5` en documentación técnica | Copiada de sensor YF-S201 (sensor diferente) | Corregida a `Q = F/98` (GFS401 real) |
| Pseudocódigo con `pwm_min/max = 0/255` | No actualizado tras corrección de escalado | Actualizado a `out_min=0.0`, `out_max=SP_MAX` |

---

### Pendientes

- [ ] Probar en prototipo real con todos los bugs corregidos
- [ ] Verificar que el caudal se lee correctamente (PV debe mostrar valores entre 0.3 y 6 L/min)
- [ ] Sintonizar KP y KI experimentalmente con Ziegler-Nichols tras la prueba de escalón
- [ ] Capturar datos con `scripts/python/log_datos.py` y graficar con `graficar_respuesta.py`
- [ ] Registrar resultados en `test/integration/procedimiento-validacion.md`

---

## Resumen del proyecto

### Total de archivos: 36

| Categoría | Archivos | Última sesión |
|---|---|---|
| **Código fuente** (`src/`) | 4 | 005 |
| **Arduino IDE** (`ArduinoIDEcontrol-caudal/`) | 4 | 005 |
| **Hardware** (`hardware/`) | 6 | 002 |
| **Scripts** (`scripts/`) | 5 | 003 |
| **Tests** (`test/`) | 2 | 005 |
| **Documentación** (`docs/`) | 6 | 005 |
| **Assets** (`assets/`) | 1 | 005 |
| **GitHub** (`.github/`) | 3 | 003 |
| **Raíz** | 3 | 003 |
| **Sesiones** | 2 | 005 |
| **Total** | **36** | |

### Estado del proyecto: LISTO PARA PRUEBA EN PROTOTIPO

El firmware tiene todos los bugs corregidos y las mejoras implementadas. La documentación está sincronizada con el código. El siguiente paso es la prueba física en el prototipo real para validar el comportamiento del sistema y realizar la sintonización final de KP y KI.
