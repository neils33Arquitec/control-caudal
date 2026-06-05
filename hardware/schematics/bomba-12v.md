# Bomba DC 12V

## Descripción

Bomba de agua de 12V DC utilizada como actuador del sistema de control de caudal. El flujo es regulado mediante PWM a través del módulo L298N.

## Especificaciones típicas

| Parámetro | Valor típico |
|---|---|
| **Voltaje nominal** | 12V DC |
| **Corriente en vacío** | ~100 – 200 mA |
| **Corriente a carga nominal** | ~300 – 800 mA |
| **Corriente de arranque** | ~1 – 2 A |
| **Caudal máximo** | 2 – 5 L/min (según modelo) |
| **Presión máxima** | 2 – 5 m de columna de agua |
| **Tipo** | Sumergible o de diafragma |
| **Diámetro de conexión** | 6 – 8 mm (manguera) |

## Conexión al L298N

```
L298N OUT1 ──── Bomba 12V (+)
L298N OUT2 ──── Bomba 12V (−)
```

## Protecciones

- **Diodo flyback:** El L298N incluye internamente diodos de protección. No obstante, agregar un 1N4007 en paralelo a la bomba (cátodo al +) como protección extra.
- **Capacitor:** 100 µF entre alimentación y GND del L298N para filtrar transitorios de arranque.
- **Fusible:** Opcional de 1A en serie con la fuente 12V.
