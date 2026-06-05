/*
 * test_pi_controller.cpp
 * Pruebas unitarias del controlador PI (compilar y ejecutar en PC)
 * Compilar: g++ -o test_pi test_pi_controller.cpp ../src/lib/PIController.cpp -I../src/lib
 */

#include <iostream>
#include <cmath>
#include "../src/lib/PIController.h"

int total = 0;
int pasaron = 0;

void verificar(const char* nombre, bool condicion) {
    total++;
    if (condicion) {
        pasaron++;
        std::cout << "  [PASS] " << nombre << std::endl;
    } else {
        std::cout << "  [FAIL] " << nombre << std::endl;
    }
}

int main() {
    std::cout << "=== Pruebas unitarias: Controlador PI ===\n\n";

    // Test 1: Creación y valores iniciales
    std::cout << "--- Test 1: Inicialización ---\n";
    PIController pi(2.0, 0.5, 0.1, 0, 255);
    verificar("Integral inicial en cero", pi.getIntegral() == 0.0f);

    // Test 2: SetPoint = PV → salida 0
    std::cout << "\n--- Test 2: Error cero ---\n";
    pi.reset();
    float out = pi.compute(3.0, 3.0);
    verificar("Salida 0 cuando SP = PV", std::abs(out) < 0.001f);

    // Test 3: SP > PV → salida positiva
    std::cout << "\n--- Test 3: Error positivo ---\n";
    pi.reset();
    out = pi.compute(3.0, 1.0);
    verificar("Salida positiva cuando SP > PV", out > 0);

    // Test 4: Saturación superior
    std::cout << "\n--- Test 4: Saturación ---\n";
    pi.reset();
    for (int i = 0; i < 100; i++) {
        out = pi.compute(6.0, 0.0);
    }
    verificar("Saturación a 255", out <= 255.0f);

    // Test 5: Saturación inferior
    std::cout << "\n--- Test 5: Saturación inferior ---\n";
    pi.reset();
    for (int i = 0; i < 100; i++) {
        out = pi.compute(0.0, 6.0);
    }
    verificar("Saturación a 0", out >= 0.0f);

    // Test 6: Anti-Windup — integral no crece infinitamente
    std::cout << "\n--- Test 6: Anti-Windup ---\n";
    pi.reset();
    float integral_prev = 0;
    bool windup_controlado = true;
    for (int i = 0; i < 50; i++) {
        out = pi.compute(6.0, 0.0);
        if (i > 0 && pi.getIntegral() < integral_prev - 1.0f) {
            windup_controlado = false;
            break;
        }
        integral_prev = pi.getIntegral();
    }
    verificar("Integral no decrece abruptamente (anti-windup)", windup_controlado);

    // Test 7: Cambio de ganancias
    std::cout << "\n--- Test 7: Actualización de ganancias ---\n";
    pi.setGains(5.0, 1.0);
    pi.reset();
    out = pi.compute(3.0, 1.0);
    verificar("Nuevas ganancias aplicadas", out > 0);

    // Test 8: Reset
    std::cout << "\n--- Test 8: Reset del controlador ---\n";
    pi.compute(3.0, 1.0);
    pi.reset();
    verificar("Integral en cero tras reset", pi.getIntegral() == 0.0f);

    // Resultados
    std::cout << "\n=== Resultados: " << pasaron << "/" << total << " pruebas pasaron ===\n";

    return (pasaron == total) ? 0 : 1;
}
