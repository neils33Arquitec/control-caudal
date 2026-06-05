// ============================================================
// Sistema de Control de Caudal
// ============================================================
// Este programa controla una bomba de agua para que entregue
// el caudal que se ajusta con un potenciómetro.
//
// Componentes:
//   - Arduino Uno
//   - Sensor de flujo GFS401 (cuenta pulsos)
//   - Bomba DC 12V
//   - Puente H L298N (controla la velocidad de la bomba)
//   - LCD 16x2 con I2C (muestra los valores en pantalla)
//   - Potenciómetro (ajusta el caudal deseado)
//
// Funcionamiento:
//   El Arduino lee el potenciómetro para saber cuánto caudal
//   se desea (SetPoint). Con el sensor GFS401 mide cuánto
//   caudal está pasando realmente. Si hay diferencia, el
//   controlador PI ajusta la velocidad de la bomba hasta
//   que el caudal real sea igual al deseado.
// ============================================================

#include <Wire.h>              // Librería para comunicación I2C
#include <LiquidCrystal_I2C.h> // Librería para manejar el LCD con I2C
#include "config.h"            // Nuestro archivo de configuración
#include "PIController.h"      // Nuestro controlador PI

// ---------- CREACIÓN DE OBJETOS ----------

// Inicializamos el LCD indicando:
// - Dirección I2C (0x27)
// - Cantidad de columnas (16)
// - Cantidad de filas (2)
LiquidCrystal_I2C lcd(LCD_DIR, LCD_COLS, LCD_FILAS);

// Creamos el controlador PI con los valores de config.h
// KP=2.0, KI=0.5, TS=0.1s, salida entre 0 y 255
PIController pi(KP, KI, TS, PWM_MIN, PWM_MAX);

// ---------- VARIABLES GLOBALES ----------

// Contador de pulsos del sensor de flujo
// Es "volatile" porque esta variable cambia dentro de una
// interrupción y el Arduino necesita saber que puede cambiar
// en cualquier momento
volatile unsigned long pulsos = 0;

// Variables para controlar el tiempo entre acciones
// millis() devuelve los milisegundos desde que se encendió
unsigned long tiempo_anterior = 0;  // Cuándo fue el último control
unsigned long serial_anterior = 0;  // Cuándo fue el último envío serial

// Variables que se muestran en el LCD y se envían por serial
float caudal = 0.0;       // Caudal real medido por el sensor (L/min)
float setpoint = 0.0;     // Caudal que queremos lograr (L/min)
float error = 0.0;        // Diferencia: lo que queremos - lo que tenemos
int pwm_out = 0;          // Valor de PWM que se envía a la bomba (0-255)

// ============================================================
// contar_pulso
// ============================================================
// Esta función se ejecuta automáticamente cada vez que el
// sensor GFS401 detecta el paso de agua. El sensor manda un
// pulso eléctrico (de 0V a 5V) y el Arduino lo cuenta.
//
// No podemos hacer cálculos aquí porque las interrupciones
// deben ser lo más rápidas posible. Solo contamos.
// ============================================================
void contar_pulso() {
    pulsos++;
}

// ============================================================
// leer_setpoint
// ============================================================
// Lee el valor del potenciómetro conectado al pin A0 y lo
// convierte a litros por minuto.
//
// analogRead devuelve un número entre 0 y 1023
//   (0V → 0,  5V → 1023)
//
// Dividimos entre 1023 para tener un valor entre 0.0 y 1.0
// Multiplicamos por SP_MAX (6.0) para tener L/min
//
// Ejemplo:
//   Potenciómetro a la mitad → analogRead = 511
//   511 / 1023 = 0.5
//   0.5 × 6.0 = 3.0 L/min
// ============================================================
float leer_setpoint() {
    int adc = analogRead(PIN_POT);
    return (adc / 1023.0) * SP_MAX;
}

// ============================================================
// calcular_caudal
// ============================================================
// Toma los pulsos que acumuló el sensor y los convierte a
// litros por minuto.
//
// El sensor GFS401 genera 5880 pulsos por cada litro de agua.
//
// Fórmula: caudal = pulsos × 60 / 5880
//
// Se multiplica por 60 porque los pulsos se contaron en 100 ms
// y necesitamos la tasa por minuto.
//
// Se desactivan las interrupciones momentáneamente para leer
// el contador de forma segura (evitar que llegue un pulso
// justo en medio de la lectura).
// ============================================================
float calcular_caudal() {
    // Desactivamos interrupciones para lectura segura
    noInterrupts();
    unsigned long p = pulsos;   // Copiamos el valor actual
    pulsos = 0;                  // Reiniciamos el contador
    interrupts();                // Reactivamos interrupciones

    // Convertimos pulsos a L/min
    return (p * 60.0) / PULSOS_POR_LITRO;
}

// ============================================================
// setup
// ============================================================
// Se ejecuta UNA SOLA VEZ cuando el Arduino se enciende o se
// reinicia. Prepara todo para empezar a funcionar.
// ============================================================
void setup() {
    // ---------- Configurar pines del L298N ----------
    // El L298N necesita saber:
    //   - ENA (pin 9):  qué tanta velocidad (PWM)
    //   - IN1 (pin 7):  dirección de giro
    //   - IN2 (pin 8):  dirección de giro

    // El pin del sensor tiene una resistencia pull-up interna
    // para evitar lecturas falsas cuando no hay señal
    pinMode(PIN_SENSOR, INPUT_PULLUP);

    // El pin del PWM se configura como salida
    pinMode(PIN_PWM, OUTPUT);

    // Los pines de dirección se configuran como salida
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);

    // Configuramos la dirección de la bomba:
    // IN1 = HIGH (5V), IN2 = LOW (0V)
    // Esto hace que el motor gire hacia adelante
    // Si la bomba gira al revés, invertir estos valores
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);

    // ---------- Iniciar comunicación serial ----------
    // Abrimos el puerto serie para enviar datos a la PC
    // Se usa el Monitor Serial del Arduino IDE
    Serial.begin(SERIAL_BAUD);

    // ---------- Iniciar el LCD ----------
    // Preparamos el LCD para mostrar información
    lcd.init();                     // Inicia la comunicación con el LCD
    lcd.backlight();                // Enciende la luz de fondo

    // Mostramos mensaje de bienvenida
    lcd.setCursor(0, 0);            // Primera línea, primera columna
    lcd.print("Sistema Caudal");
    lcd.setCursor(0, 1);            // Segunda línea, primera columna
    lcd.print("Iniciando...");

    // Esperamos 1.5 segundos para que el usuario vea el mensaje
    delay(1500);

    // Limpiamos la pantalla para empezar a mostrar datos
    lcd.clear();

    // ---------- Configurar interrupción del sensor ----------
    // Cada vez que el pin 2 pase de 0V a 5V (RISING),
    // el Arduino ejecutará automáticamente la función
    // "contar_pulso", incluso si está en medio del loop
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), contar_pulso, RISING);

    // ---------- Inicializar tiempos ----------
    // Guardamos el momento actual como referencia para
    // saber cuándo ejecutar el próximo control
    tiempo_anterior = millis();
    serial_anterior = millis();
}

// ============================================================
// loop
// ============================================================
// Se ejecuta UNA Y OTRA VEZ mientras el Arduino esté encendido.
//
// Hace tres cosas principales:
//   1. Lee el potenciómetro (SetPoint) — siempre
//   2. Cada 100 ms: mide caudal, calcula PI, actualiza PWM y LCD
//   3. Cada 1000 ms: envía datos al monitor serial
// ============================================================
void loop() {
    // Tomamos el tiempo actual en milisegundos
    unsigned long ahora = millis();

    // Calculamos cuánto tiempo pasó desde el último control
    unsigned long dt = ahora - tiempo_anterior;

    // Leemos el potenciómetro para saber cuánto caudal se pide
    // Esto se hace en cada ciclo del loop para responder
    // rápidamente a los cambios del potenciómetro
    setpoint = leer_setpoint();

    // ========================================================
    // BLOQUE DE CONTROL: se ejecuta cada 100 ms (TS × 1000)
    // ========================================================
    // Si ya pasó el tiempo suficiente desde el último control...
    if (dt >= (unsigned long)(TS * 1000)) {
        // --- Medir el caudal real ---
        caudal = calcular_caudal();

        // --- Calcular el error ---
        // Si el error es positivo: falta caudal (acelerar bomba)
        // Si el error es negativo: sobra caudal (frenar bomba)
        // Si el error es cero: estamos en el valor deseado
        error = setpoint - caudal;

        // --- Calcular el PWM con el controlador PI ---
        // El PI recibe el setpoint y la medición, y devuelve
        // un valor entre 0 y 255 para el PWM
        pwm_out = (int)pi.compute(setpoint, caudal);

        // --- Enviar el PWM al L298N ---
        // analogWrite envía una señal PWM al pin 9
        // El L298N usa esa señal para encender y apagar la
        // bomba muy rápido, controlando así su velocidad
        analogWrite(PIN_PWM, pwm_out);

        // ====================================================
        // Mostrar información en el LCD
        // ====================================================

        // --- Primera línea: SetPoint y Caudal Real ---
        // Ejemplo: "SP:3.0 PV:2.8"
        lcd.setCursor(0, 0);
        lcd.print("SP:");            // "SP:"
        lcd.print(setpoint, 1);      // "3.0" (un decimal)
        lcd.print(" PV:");           // " PV:"
        lcd.print(caudal, 1);        // "2.8"
        // Espacios al final para borrar caracteres de la
        // actualización anterior si el número se achica
        lcd.print("      ");

        // --- Segunda línea: Error y PWM ---
        // Ejemplo: "E:0.2 PWM:128"
        lcd.setCursor(0, 1);
        lcd.print("E:");             // "E:"
        lcd.print(error, 1);         // "0.2"
        lcd.print(" PWM:");          // " PWM:"
        lcd.print(pwm_out);          // "128"
        // Espacios al final para borrar sobrantes
        lcd.print("   ");

        // --- Actualizar la referencia de tiempo ---
        tiempo_anterior = ahora;
    }

    // ========================================================
    // BLOQUE SERIAL: se ejecuta cada 1000 ms (SERIAL_INTERVAL)
    // ========================================================
    // Enviamos los datos al monitor serial para registrar o
    // graficar el comportamiento del sistema
    if (ahora - serial_anterior >= SERIAL_INTERVAL) {
        // Formato: "SP:3.00,PV:2.84,E:0.16,PWM:128"
        // Fácil de copiar a Excel o MATLAB
        Serial.print("SP:");
        Serial.print(setpoint, 2);
        Serial.print(",PV:");
        Serial.print(caudal, 2);
        Serial.print(",E:");
        Serial.print(error, 2);
        Serial.print(",PWM:");
        Serial.println(pwm_out);

        // Actualizamos la referencia de tiempo serial
        serial_anterior = ahora;
    }
}
