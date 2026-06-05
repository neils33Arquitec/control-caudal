# Bill of Materials (BOM) — Lista de Componentes

## Sistema de Control de Caudal PI

| # | Componente | Cantidad | Especificación | Referencia | Notas |
|---|---|---|---|---|---|
| 1 | **Arduino Uno R3** | 1 | ATmega328P | U1 | Microcontrolador |
| 2 | **Sensor de flujo GFS401** | 1 | 0.3–6 L/min, Hall, 5V | S1 | Equivalente YF-S401 |
| 3 | **Bomba DC 12V** | 1 | 12V, ~300–800 mA | M1 | Bomba sumergible o diafragma |
| 4 | **Módulo Puente H L298N** | 1 | L298N, 2A por canal | U2 | Control PWM de dirección y velocidad |
| 5 | **LCD 16×2 + I2C** | 1 | HD44780 + PCF8574 | LCD1 | Dirección I2C 0x27 |
| 6 | **Potenciómetro** | 1 | 10 kΩ, lineal | R1 | SetPoint de caudal |
| 7 | **Fuente de alimentación** | 1 | 12V DC, ≥1A | PS1 | Alimentación bomba y Arduino |
| 8 | **Protoboard** | 1 | 830 puntos | — | Para prototipado |
| 9 | **Cables jumper** | 20 | M-M y M-H | — | Conexiones varias |

### Componentes pasivos y misceláneos

| # | Componente | Cantidad | Especificación | Notas |
|---|---|---|---|---|
| 10 | Resistencia | 1 | 10 kΩ, 1/4W | Pull-down potenciómetro (si necesario) |
| 11 | Capacitor electrolítico | 1 | 100 µF, 25V | Filtro fuente driver |
| 12 | Diodo 1N4007 | 1 | 1A, 1000V | Flyback protección bomba |
| 13 | Cable USB A-B | 1 | — | Programación Arduino |
| 14 | Manguera 7 mm | 1 m | Silicona o PVC | Conexión hidráulica |
| 15 | Conectores rápidos | 4 | — | Uniones manguera |

### Herramientas

| Herramienta | Uso |
|---|---|
| Multímetro digital | Verificar continuidad y voltajes |
| Fuente de laboratorio (opcional) | Pruebas sin arriesgar la fuente principal |
| Cautín + estaño | Soldar conexiones críticas |
| Pelacables | Preparar cables |

## Costo estimado

| Componente | Precio estimado (USD) |
|---|---|
| Arduino Uno R3 | $8 – $12 |
| Sensor GFS401 | $3 – $6 |
| Bomba DC 12V | $5 – $15 |
| Driver L298N | $3 – $5 |
| LCD 16×2 + I2C | $5 – $8 |
| Potenciómetro | $0.50 |
| Fuente 12V | $5 – $10 |
| Protoboard + cables | $3 – $5 |
| Pasivos | $1 – $2 |
| **Total estimado** | **$35 – $65 USD** |
