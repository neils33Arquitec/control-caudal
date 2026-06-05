# Protocolo de Sesiones

## Sistema de Control de Caudal PI — Memoria de Recuperación

---

## Índice de sesiones

| Sesión | Fecha | Descripción |
|---|---|---|
| [Sesión 001](#sesión-001) | Junio 2026 | Creación del proyecto, estructura, marco teórico |
| [Sesión 002](#sesión-002) | Junio 2026 | Documentación de hardware, BOM, wiring |
| [Sesión 003](#sesión-003) | Junio 2026 | Código fuente, scripts, tests, archivos raíz, GitHub, assets, manuales |

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
| `control-caudal.ino` | Sketch principal de Arduino (raíz del proyecto). `setup()`: init LCD, pines, interrupción. `loop()`: lectura SP/PV, PI → PWM, LCD, serial cada 1s. |
| `src/PIController.h` | Clase `PIController` con métodos: `compute()`, `reset()`, `setGains()`, `getIntegral()`. |
| `src/PIController.cpp` | Implementación: PI discreto por Backward Euler, anti-windup por clamping con saturación condicional del integrador. |
| `src/config.h` | Constantes del sistema: pines (2, 7, 8, 9, A0), Kp=2.0, Ki=0.5, Ts=0.1s, 5880 pulsos/L, LCD 0x27. |

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
| `src/config.h` | Eliminadas macros no utilizadas (`CAUDAL_MAXIMO`, `PIN_LCD_SDA`, `PIN_LCD_SCL`). |
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

## Resumen del proyecto

### Total de archivos: 30

| Categoría | Archivos | Última sesión |
|---|---|---|
| **Código fuente** (`src/`) | 4 | 003 |
| **Hardware** (`hardware/`) | 6 | 002 |
| **Scripts** (`scripts/`) | 5 | 003 |
| **Tests** (`test/`) | 2 | 003 |
| **Documentación** (`docs/`) | 6 | 003 |
| **Assets** (`assets/`) | 1 | 003 |
| **GitHub** (`.github/`) | 3 | 003 |
| **Raíz** | 3 | 003 |
| **Sesiones** | 1 | 003 |
| **Total** | **31** | |

### Estado del proyecto: COMPLETADO

El proyecto cuenta con firmware funcional para Arduino, marco teórico documentado, scripts de simulación, pruebas unitarias, procedimiento de validación, CI/CD, licencia, manual de montaje y manual de usuario.
