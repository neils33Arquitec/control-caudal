% sintonizar_ZN.m
% Cálculo de ganancias Kp y Ki por Ziegler-Nichols (lazo abierto)
% Basado en parámetros FOPDT identificados

clear; clc;

%% Parámetros FOPDT (ingresar desde identificar_fopdt.m)
K = 0.02;     % Ganancia de la planta
theta = 1.5;  % Tiempo muerto (s)
tau = 3.2;    % Constante de tiempo (s)

%% Ziegler-Nichols — Método curva de reacción (lazo abierto)
a = K * theta / tau;

fprintf('=== Sintonización Ziegler-Nichols (lazo abierto) ===\n');
fprintf('Parámetro a = K * θ / τ = %.4f\n\n', a);

% Tabla ZN para PI
Kp_zn = 0.9 / a;
Ti_zn = 3 * theta;
Ki_zn = Kp_zn / Ti_zn;

fprintf('--- Controlador PI ---\n');
fprintf('Kp = 0.9 / a = %.4f\n', Kp_zn);
fprintf('Ti = 3 * θ  = %.2f s\n', Ti_zn);
fprintf('Ki = Kp / Ti = %.4f\n\n', Ki_zn);

%% Sugerencia de ajuste fino
fprintf('=== Recomendaciones ===\n');
fprintf('1. Probar Kp = %.2f como punto de partida\n', Kp_zn * 0.7);
fprintf('2. Si hay sobreimpulso excesivo, reducir Kp\n');
fprintf('3. Si el error estacionario persiste, aumentar Ki\n');
fprintf('4. Ts recomendado: %.2f s (θ/10)\n', theta / 10);

%% Simulación de lazo cerrado
s = tf('s');
Gp = K * exp(-theta * s) / (tau * s + 1);
Gc = Kp_zn * (1 + 1 / (Ti_zn * s));
Gla = Gc * Gp;
Glc = feedback(Gla, 1);

figure;
step(Glc);
title('Respuesta al escalón — PI sintonizado ZN');
grid on;
