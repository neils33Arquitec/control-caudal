% identificar_fopdt.m
% Identificación de planta FOPDT a partir de respuesta al escalón
% Sistema: bomba DC + tubería + sensor GFS401

clear; clc; close all;

%% Datos de entrada (cargar desde archivo o ingresar manualmente)
% Formato: tiempo (s), PWM (entrada), caudal L/min (salida)
% Ejemplo con datos simulados:
t = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20]';
u = [0, 0, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150]';
y = [0, 0, 0, 0.3, 0.9, 1.5, 2.0, 2.4, 2.7, 2.9, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0]';

%% Identificar parámetros FOPDT
u_ss1 = u(1);
u_ss2 = u(end);
y_ss1 = y(1);
y_ss2 = y(end);

K = (y_ss2 - y_ss1) / (u_ss2 - u_ss1);
delta_y = y_ss2 - y_ss1;

% Encontrar tiempo al 63.2% del cambio total
y_63 = y_ss1 + 0.632 * delta_y;
[~, idx_63] = min(abs(y - y_63));
t_63 = t(idx_63);

% Encontrar tiempo muerto (primer cambio significativo)
umbral = 0.05 * delta_y;
idx_inicio = find(abs(y - y_ss1) > umbral, 1, 'first');
theta = t(idx_inicio);

% Constante de tiempo
tau = t_63 - theta;

fprintf('=== Parámetros FOPDT identificados ===\n');
fprintf('Ganancia K:       %.4f (L/min)/PWM\n', K);
fprintf('Tiempo muerto θ:  %.2f s\n', theta);
fprintf('Constante tiempo τ: %.2f s\n', tau);
fprintf('Modelo: G(s) = %.4f * exp(-%.2f*s) / (%.2f*s + 1)\n', K, theta, tau);

%% Graficar
y_modelo = zeros(size(t));
for i = 1:length(t)
    if t(i) <= theta
        y_modelo(i) = y_ss1;
    else
        y_modelo(i) = y_ss1 + K * (u_ss2 - u_ss1) * (1 - exp(-(t(i)-theta)/tau));
    end
end

figure;
plot(t, y, 'bo-', 'LineWidth', 1.5, 'DisplayName', 'Datos reales');
hold on;
plot(t, y_modelo, 'r-', 'LineWidth', 2, 'DisplayName', 'Modelo FOPDT');
xline(theta, '--k', 'θ', 'LabelOrientation', 'horizontal');
xline(tau + theta, '--k', 'τ+θ');
yline(y_63, '--m', '63.2%');
xlabel('Tiempo (s)');
ylabel('Caudal (L/min)');
title('Identificación FOPDT — Respuesta al escalón');
legend;
grid on;
