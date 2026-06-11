#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "PIController.h"

LiquidCrystal_I2C lcd(LCD_DIR, LCD_COLS, LCD_FILAS);
PIController pi(KP, KI, TS, 0.0, SP_MAX);

volatile unsigned long pulsos = 0;
unsigned long tiempo_anterior = 0;
unsigned long serial_anterior = 0;
unsigned long lcd_cycle_start = 0;

float caudal = 0.0;
float setpoint = 0.0;
float setpoint_anterior = -1.0;
float error = 0.0;
int pwm_out = 0;

void contar_pulso()
{
    pulsos++;
}

float leer_setpoint()
{
    int adc = analogRead(PIN_POT);
    return (adc / 1023.0) * SP_MAX;
}

float calcular_caudal()
{
    noInterrupts();
    unsigned long p = pulsos;
    pulsos = 0;
    interrupts();
    // p = pulsos contados en TS segundos → normalizar a pulsos/segundo antes de convertir
    return (p * 60.0) / (PULSOS_POR_LITRO * TS);
}

void setup()
{
    pinMode(PIN_SENSOR, INPUT_PULLUP);
    pinMode(PIN_PWM, OUTPUT);
    pinMode(PIN_IN1, OUTPUT);
    pinMode(PIN_IN2, OUTPUT);

    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);

    Serial.begin(SERIAL_BAUD);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sistema Caudal");
    lcd.setCursor(0, 1);
    lcd.print("Iniciando...");
    delay(1500);
    lcd.clear();

    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), contar_pulso, RISING);

    tiempo_anterior = millis();
    serial_anterior = millis();
    lcd_cycle_start = millis();
}

void loop()
{
    unsigned long ahora = millis();
    unsigned long dt = ahora - tiempo_anterior;

    setpoint = leer_setpoint();

    if (dt >= (unsigned long)(TS * 1000))
    {
        caudal = calcular_caudal();

        if (fabs(setpoint - setpoint_anterior) > 0.05) {
            pi.reset();
            setpoint_anterior = setpoint;
        }

        error = setpoint - caudal;
        float pi_out = pi.compute(setpoint, caudal);
        pwm_out = (int)((pi_out / SP_MAX) * (float)PWM_MAX);
        analogWrite(PIN_PWM, pwm_out);

        // --- Ciclo de pantallas LCD (10 s) ---
        unsigned long ciclo_ms = ahora - lcd_cycle_start;

        if (ciclo_ms < 6000) {
            // Pantalla 1: Normal (SP, PV, Error, PWM)
            lcd.setCursor(0, 0);
            lcd.print("SP:");
            lcd.print(setpoint, 1);
            lcd.print(" PV:");
            lcd.print(caudal, 1);
            lcd.print("      ");

            lcd.setCursor(0, 1);
            lcd.print("E:");
            lcd.print(error, 1);
            lcd.print(" PWM:");
            lcd.print(pwm_out);
            lcd.print("   ");
        }
        else if (ciclo_ms < 8000) {
            // Pantalla 2: Potenciometro (ADC, voltaje, SP)
            int adc = analogRead(PIN_POT);
            float volt = (adc / 1023.0) * 5.0;

            lcd.setCursor(0, 0);
            lcd.print("ADC:");
            lcd.print(adc);
            lcd.print(" ");
            lcd.print(volt, 1);
            lcd.print("V   ");

            lcd.setCursor(0, 1);
            lcd.print("SP:");
            lcd.print(setpoint, 1);
            lcd.print(" L/min   ");
        }
        else {
            // Pantalla 3: PWM (valor y porcentaje)
            float pct = (pwm_out * 100.0) / 255.0;

            lcd.setCursor(0, 0);
            lcd.print("PWM: ");
            lcd.print(pwm_out);
            lcd.print("/255 ");

            lcd.setCursor(0, 1);
            lcd.print(pct, 1);
            lcd.print("%         ");
        }

        if (ciclo_ms >= 10000)
            lcd_cycle_start = ahora;

        tiempo_anterior = ahora;
    }

    if (ahora - serial_anterior >= SERIAL_INTERVAL)
    {
        Serial.print("SP:");
        Serial.print(setpoint, 2);
        Serial.print(",PV:");
        Serial.print(caudal, 2);
        Serial.print(",E:");
        Serial.print(error, 2);
        Serial.print(",PWM:");
        Serial.println(pwm_out);
        serial_anterior = ahora;
    }
}