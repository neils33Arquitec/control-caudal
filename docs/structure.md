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
├── assets/
│   ├── fotos/                     # Fotos del prototipo armado
│   ├── graficos/                  # Gráficos de respuesta del sistema
│   └── diagrama-bloques.md        # Diagrama de bloques en Mermaid
├── docs/
│   ├── images/                    # Imágenes y diagramas
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
├── control-caudal.ino     # Sketch principal de Arduino (raíz)
├── src/
│   ├── config.h               # Constantes y configuración global
│   ├── PIController.cpp       # Implementación del PI discreto
│   └── PIController.h         # Cabecera de la clase PI
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
| Código fuente | 4 | Firmware Arduino + librería PI |
| Hardware | 6 | Datasheets, esquemas, BOM |
| Scripts | 5 | MATLAB, Python, Simulink |
| Tests | 2 | Unitarios y validación |
| Documentación | 4 | Técnica, estructura, proyecto |
| Assets | 1 | Diagrama de bloques |
| GitHub | 3 | CI + issue templates |
| Raíz | 3 | README, LICENSE, .gitignore |
| **Total** | **28** | |

## Convenciones

- **Idioma:** Código y documentación en español
- **Plataforma:** Arduino Uno (ATmega328P)
- **Controlador:** PI discreto con anti-windup por clamping
- **Sensor:** GFS401 (Hall, 0.3–6 L/min, 5880 pulsos/L)
- **Actuador:** Bomba DC 12V vía L298N (PWM)
