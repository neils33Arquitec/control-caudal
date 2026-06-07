#ifndef PI_CONTROLLER_H
#define PI_CONTROLLER_H

// ============================================================
// Controlador PI (Proporcional + Integral)
// ============================================================
// Este controlador se encarga de que la bomba entregue
// exactamente el caudal que pedimos con el potenciómetro.
//
// ¿Cómo lo hace?
// 1. Recibe el caudal deseado (SetPoint) y el caudal real medido
// 2. Calcula la diferencia: error = SetPoint - medición
// 3. La parte proporcional (KP) reacciona al error actual
// 4. La parte integral (KI) corrige errores que se acumulan
// 5. Devuelve un valor de PWM entre 0 y 255
//
// Fórmula:  Salida = KP × error + KI × Ts × suma_de_errores
//
// KP: respuesta inmediata (qué tan rápido acelera la bomba)
// KI: corrige errores que persisten en el tiempo
// Ts: cada cuánto tiempo se hace el cálculo
// ============================================================

class PIController {
public:
    // Constructor: guarda los valores con los que vamos a trabajar
    // kp       → qué tan fuerte reacciona al error actual
    // ki       → qué tanto corrige errores acumulados
    // ts       → cada cuántos segundos se ejecuta el cálculo
    // out_min  → valor mínimo de salida (casi siempre 0)
    // out_max  → valor máximo de salida (casi siempre 255)
    PIController(float kp, float ki, float ts,
                 float out_min, float out_max);

    // Calcula el nuevo valor de PWM
    // setpoint:    el caudal que queremos lograr (viene del potenciómetro)
    // measurement: el caudal real que está midiendo el sensor
    // Devuelve:    un número entre out_min y out_max listo para analogWrite
    float compute(float setpoint, float measurement);

    // Reinicia el controlador: vuelve el error acumulado a cero
    // Útil cuando hacemos un cambio grande en el SetPoint
    void reset();

    // Permite cambiar KP y KI mientras el sistema está funcionando
    // sin tener que apagar y reprogramar el Arduino
    void setGains(float kp, float ki);

    // Muestra cuánto error se ha ido acumulando (para diagnóstico)
    float getIntegral() const;

private:
    float _kp, _ki;       // Ganancias del controlador
    float _ts;            // Tiempo entre cada cálculo (segundos)
    float _out_min;       // Límite inferior del PWM
    float _out_max;       // Límite superior del PWM
    float _integral;      // Error acumulado a lo largo del tiempo
    float _out_prev;      // Último valor de PWM que se envió

    // Evita que un número se salga de los límites permitidos
    // Por ejemplo: clamp(300, 0, 255) devolvería 255
    float clamp(float value, float min, float max) const;
};

#endif
