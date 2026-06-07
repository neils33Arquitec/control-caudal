#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// Configuración del Sistema de Control de Caudal
// Aquí se definen todos los pines, constantes y parámetros
// del controlador. Ajusta estos valores según tu hardware.
// ============================================================

// ---------- PINES DE CONEXIÓN ----------
// Pin 2:  Llega la señal del sensor de flujo GFS401 (cable amarillo)
//          Cada vez que el sensor detecta el paso de agua,
//          genera un pulso y el Arduino lo cuenta automáticamente
#define PIN_SENSOR       2

// Pin 9:  Salida PWM que controla la velocidad de la bomba
//         0 = bomba apagada, 255 = bomba al máximo
#define PIN_PWM          9

// Pines 7 y 8: Le dicen al L298N en qué dirección girar la bomba
// IN1 = HIGH, IN2 = LOW  → la bomba gira hacia adelante
// Si se invierten, la bomba gira al revés
#define PIN_IN1          7
#define PIN_IN2          8

// Pin A0: Entrada analógica del potenciómetro
// Giras el potenciómetro y cambias el caudal deseado (SetPoint)
// 0V   → 0 L/min
// 5V   → 6 L/min (SP_MAX)
#define PIN_POT          A0

// ---------- SENSOR GFS401 ----------
// El sensor genera 5880 pulsos por cada litro de agua que pasa
// Este valor viene en la hoja de datos del fabricante
#define PULSOS_POR_LITRO  5880.0

// ---------- CONTROLADOR PI ----------
// KP = 2.0:  Qué tan fuerte reacciona la bomba ante un error
//            Si el caudal real está lejos del deseado, la bomba
//            acelera más o menos según este valor
//            Muy alto → respuesta rápida pero puede oscilar
//            Muy bajo → respuesta lenta
#define KP                2.0

// KI = 0.5:  Corrige el error que se queda después de la parte
//            proporcional. Sirve para eliminar el error permanente
//            (por ejemplo, si falta siempre 0.2 L/min)
//            Un valor muy alto puede hacer que el sistema oscile
#define KI                0.5

// TS = 0.1:  Cada 100 milisegundos, el Arduino calcula el nuevo
//            PWM. No puede ser demasiado rápido porque el sensor
//            necesita tiempo para dar pulsos confiables
#define TS                0.1

// Límites del PWM
// 0:    bomba completamente detenida
// 255:  bomba a máxima velocidad
#define PWM_MIN           0
#define PWM_MAX           255

// ---------- LCD 16x2 (con módulo I2C) ----------
// Dirección I2C del LCD (la mayoría usa 0x27 o 0x3F)
#define LCD_DIR           0x27
#define LCD_COLS          16
#define LCD_FILAS         2

// ---------- POTENCIÓMETRO (SetPoint) ----------
// Valor máximo del SetPoint en litros por minuto
// Con el potenciómetro al máximo, pediremos 6 L/min
#define SP_MAX            6.0

// ---------- PUERTO SERIE (monitoreo en PC) ----------
// Velocidad de comunicación: 9600 baudios
#define SERIAL_BAUD       9600
// Cada 1000 ms se envía un reporte de datos al monitor serial
#define SERIAL_INTERVAL   1000

#endif
