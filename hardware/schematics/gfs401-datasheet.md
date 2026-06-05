# Sensor de Flujo GFS401

## Descripción

El GFS401 es un sensor de flujo de agua basado en el **efecto Hall** para caudales pequeños (0.3 – 6 L/min). Internamente contiene un rotor con un imán que gira al paso del agua. El sensor Hall detecta el campo magnético y genera pulsos digitales proporcionales al caudal.

**Nota:** El GFS401 es equivalente/compatible con el modelo **YF-S401**.

## Especificaciones técnicas

| Parámetro | Valor |
|---|---|
| **Modelo** | GFS401 (equivalente YF-S401) |
| **Principio** | Efecto Hall |
| **Voltaje de operación** | 5 – 24 V DC |
| **Corriente máxima** | 10 mA (a 5V) |
| **Rango de caudal** | 0.3 – 6 L/min |
| **Presión máxima** | 0.8 MPa |
| **Temperatura de operación** | -25 °C a +80 °C |
| **Humedad de operación** | 35% – 90% RH |
| **Conexión hidráulica** | Barba de 7 mm (1/4″) |
| **Pulsos por litro** | ~5880 pulsos/L (~0.17 mL/pulso) |
| **Salida** | Digital (onda cuadrada, 0-5V TTL) |

## Asignación de pines

| Color | Tipo | Conexión |
|---|---|---|
| **Rojo** | VCC | 5V (Arduino) |
| **Negro** | GND | GND (Arduino) |
| **Amarillo** | Signal | Pin digital (Arduino, con interrupción) |

## Relación frecuencia-caudal

El sensor genera **~5880 pulsos por litro**.

Para un período de muestreo de 1 segundo:

```
caudal (L/min) = (pulsos_por_segundo × 60) / 5880
caudal (L/h)   = (pulsos_por_segundo × 3600) / 5880
```

**Ecuación simplificada:**

```
Q(L/min) = F(Hz) / 98
```

Donde `F = pulsos_por_segundo`.

## Instalación

- **Orientación:** Vertical (inclinación máxima recomendada ≤ 5°)
- **Flujo:** La flecha en el cuerpo indica la dirección del flujo
- **Conexiones:** Manguera de 7 mm de diámetro interno
- **Rosca:** 1/4″ (GFS401) o 1/2″ (según variante)

## Consideraciones

- Rango de medición limitado a **0.3 – 6 L/min**. Caudales fuera de este rango dan lecturas no lineales.
- Precisión nominal: ±10% del valor leído.
- La salida es TTL compatible con pines de Arduino (5V).
- Usar interrupción hardware (`attachInterrupt`) para lectura precisa de pulsos.
- El sensor NO es apto para agua caliente (>80°C) ni fluidos corrosivos.
