#include "PIController.h"

// ============================================================
// Constructor: Guarda todos los parámetros y pone el
// controlador en cero (sin error acumulado, sin salida previa)
// ============================================================
PIController::PIController(float kp, float ki, float ts,
                           float out_min, float out_max)
    : _kp(kp), _ki(ki), _ts(ts),
      _out_min(out_min), _out_max(out_max),
      _integral(0.0), _out_prev(0.0) {}

// ============================================================
// reset: Vuelve el controlador a su estado inicial
// Sirve para empezar de cero cuando se cambia el SetPoint
// de forma brusca, evitando acumulaciones anteriores
// ============================================================
void PIController::reset() {
    _integral = 0.0;   // Olvidamos el error acumulado
    _out_prev = 0.0;    // Olvidamos la salida anterior
}

// ============================================================
// setGains: Permite cambiar KP y KI en pleno funcionamiento
// Útil para hacer ajustes finos sin reprogramar
// ============================================================
void PIController::setGains(float kp, float ki) {
    _kp = kp;
    _ki = ki;
}

// ============================================================
// compute: El corazón del controlador PI
// Aquí es donde se decide cuánta potencia darle a la bomba
//
// Paso a paso:
//  1. Calcula el error (lo que queremos - lo que tenemos)
//  2. Acumula ese error en el tiempo (parte integral)
//  3. Calcula la salida: KP × error + integral acumulada
//  4. Ajusta la salida al rango del PWM (0 a 255)
//  5. Anti-Windup: si la bomba ya está al máximo o mínimo,
//     el error acumulado no puede seguir creciendo sin control
//
// Anti-Windup:
//  Imagina que pides 6 L/min pero la bomba ya está al máximo
//  y solo da 4 L/min. El error siempre es positivo y el
//  integral acumula sin parar. Cuando bajas el SetPoint, el
//  integral acumulado haría que la bomba se quede acelerada
//  por varios segundos de más. El anti-windup evita esto
//  limitando cuánto puede crecer el integral.
// ============================================================
float PIController::compute(float setpoint, float measurement) {
    // Paso 1: ¿Cuánto falta o cuánto sobra?
    // Si setpoint = 3.0 y measurement = 2.0, error = +1.0 (falta caudal)
    // Si setpoint = 3.0 y measurement = 4.0, error = -1.0 (sobra caudal)
    float error = setpoint - measurement;

    // Paso 2: Acumulamos el error en el tiempo
    // Cuanto más tiempo pase con error, más crece este acumulador
    _integral += _ki * _ts * error;

    // Paso 3: Calculamos la salida total del controlador
    // Parte proporcional: reacciona al error de ahora
    // Parte integral: corrige errores que han persistido
    float out = _kp * error + _integral;

    // Paso 4: Nos aseguramos de que la salida esté entre 0 y 255
    // Si el cálculo diera 300, lo dejamos en 255
    // Si el cálculo diera -50, lo dejamos en 0
    out = clamp(out, _out_min, _out_max);

    // Paso 5: Anti-Windup
    // Ajustamos el integral para que no pueda crecer más allá
    // de lo que la bomba puede responder. Si la salida está
    // saturada en 255, el integral no puede seguir subiendo
    _integral = clamp(_integral, _out_min - _kp * error,
                      _out_max - _kp * error);

    // Guardamos la salida para referencia futura
    _out_prev = out;

    // Devolvemos el PWM listo para enviar al motor
    return out;
}

// ============================================================
// getIntegral: Muestra el error acumulado actual
// Sirve para diagnosticar si el integral está muy alto
// ============================================================
float PIController::getIntegral() const {
    return _integral;
}

// ============================================================
// clamp: Función auxiliar que mantiene un valor dentro de
// un rango. Si el valor se pasa del límite, lo recorta.
//
// Ejemplos:
//  clamp(150, 0, 255) → 150  (está dentro del rango)
//  clamp(300, 0, 255) → 255  (se pasó, lo recortamos)
//  clamp(-10, 0, 255) → 0    (está debajo, lo subimos)
// ============================================================
float PIController::clamp(float value, float min, float max) const {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}
