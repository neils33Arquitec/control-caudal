# Estructura del Proyecto

Sistema Inteligente de Control de Caudal mediante Control PI en Lazo Cerrado

## Árbol de directorios

```
Proyecto TM/
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md          # Plantilla para reportar bugs
│   │   └── feature_request.md     # Plantilla para solicitar mejoras
│   └── workflows/
│       └── arduino-compile.yml    # CI: compila sketch con arduino-cli
├── ArduinoIDEcontrol-caudal/      # Copia plana para Arduino IDE
│   ├── config.h                   # (copia de src/firmware/config.h)
│   ├── control-caudal.ino         # (copia de src/firmware/control-caudal.ino)
│   ├── PIController.cpp           # (copia de src/firmware/PIController.cpp)
│   └── PIController.h             # (copia de src/firmware/PIController.h)
├── assets/
│   ├── fotos/                     # Fotos del prototipo armado
│   ├── graficos/                  # Gráficos de respuesta del sistema
│   └── diagrama-bloques.md        # Diagrama de bloques en Mermaid
├── docs/
│   ├── manual-montaje.md          # Guía de montaje paso a paso
│   ├── manual-usuario.md          # Manual de operación del sistema
│   ├── nota-tecnica-ganancias-pi.md  # Explicación de KP y KI en el sistema
│   ├── proyecto002.md             # Documento original del proyecto
│   ├── structure.md               # Este archivo
│   └── technical-documentation.md # Marco teórico e investigación
├── hardware/
│   ├── bom/
│   │   └── bill-of-materials.md   # Lista completa de componentes
│   └── schematics/
│       ├── bomba-12v.md           # Especificaciones de la bomba
│       ├── gfs401-datasheet.md    # Datasheet del sensor de flujo
│       ├── l298n-module.md        # Documentación del puente H
│       ├── lcd-i2c.md             # Documentación de la pantalla LCD
│       ├── potenciometro-setpoint.md  # SetPoint del sistema
│       └── wiring-diagram.md      # Diagrama de conexiones completo
├── scripts/
│   ├── matlab/
│   │   ├── identificar_fopdt.m    # Identificación de planta FOPDT
│   │   └── sintonizar_ZN.m        # Sintonización Ziegler-Nichols
│   ├── python/
│   │   ├── graficar_respuesta.py  # Visualización de datos
│   │   └── log_datos.py           # Captura de datos seriales
│   └── simulink/
│       └── modelo_lazo_control.md # Descripción del modelo Simulink
├── sesiones/
│   ├── plan-mejoras.md            # Plan de correcciones tras prueba del prototipo
│   └── sesiones.md                # Bitácora de desarrollo por sesión
├── src/
│   └── firmware/
│       ├── config.h               # Constantes y configuración global
│       ├── control-caudal.ino     # Sketch principal de Arduino
│       ├── PIController.cpp       # Implementación del PI discreto
│       └── PIController.h         # Cabecera de la clase PI
├── test/
│   ├── integration/
│   │   └── procedimiento-validacion.md  # Pruebas de integración
│   └── unit/
│       └── test_pi_controller.cpp       # Tests unitarios del PI
├── .gitignore
├── LICENSE                    # Licencia MIT
└── README.md                  # Descripción del proyecto
```

## Resumen de archivos

| Categoría | Archivos | Descripción |
|---|---|---|
| Código fuente (`src/`) | 4 | Firmware Arduino + librería PI |
| Arduino IDE (`ArduinoIDEcontrol-caudal/`) | 4 | Copia plana para compatibilidad con Arduino IDE |
| Hardware (`hardware/`) | 6 | Datasheets, esquemas, BOM |
| Scripts (`scripts/`) | 5 | MATLAB, Python, Simulink |
| Tests (`test/`) | 2 | Unitarios y validación |
| Documentación (`docs/`) | 6 | Técnica, manuales, notas, estructura, proyecto |
| Sesiones (`sesiones/`) | 2 | Bitácora y plan de mejoras |
| Assets (`assets/`) | 1 | Diagrama de bloques |
| GitHub (`.github/`) | 3 | CI + issue templates |
| Raíz | 3 | README, LICENSE, .gitignore |
| **Total** | **36** | |

## Convenciones

- **Idioma:** Código y documentación en español
- **Plataforma:** Arduino Uno (ATmega328P)
- **Controlador:** PI discreto con anti-windup por clamping
- **Sensor:** GFS401 (Hall, 0.3–6 L/min, 5880 pulsos/L)
- **Actuador:** Bomba DC 12V vía L298N (PWM)
