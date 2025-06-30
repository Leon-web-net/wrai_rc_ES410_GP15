import numpy as np
import matplotlib.pyplot as plt

def convert_steering_angle(delta_A, L_A, L_B):
    return np.arctan((L_B / L_A) * np.tan(delta_A))

def simulate_vehicle(x0, y0, theta0, delta_func, L, v, T, dt):
    n_steps = int(T / dt)
    x = np.zeros(n_steps)
    y = np.zeros(n_steps)
    theta = np.zeros(n_steps)
    x[0], y[0], theta[0] = x0, y0, theta0

    for t in range(1, n_steps):
        delta = delta_func(t * dt)
        print( delta )
        x[t] = x[t-1] + v * np.cos(theta[t-1]) * dt
        y[t] = y[t-1] + v * np.sin(theta[t-1]) * dt
        theta[t] = theta[t-1] + (v / L) * np.tan(delta) * dt

    return x, y, theta

# Parameters
imeche_wheelbase = 1.545
rc_wheelbase = 0.625
v = 5.0  # m/s
dt = 0.1
T = 50.0

# Define Vehicle A steering function (radians)
def delta_A_func(t):

    ideal = np.radians(30 * np.sin(0.1 * t))  # example: oscillating steering
    constrained = np.clip(ideal, -0.418879, 0.418879) # vehicle limits
    return constrained

# Convert to Vehicle B's steering
def delta_B_func(t):
    ideal = convert_steering_angle(delta_A_func(t), imeche_wheelbase, rc_wheelbase)
    contrained = np.clip(ideal, -0.2460914245312, 0.2460914245312) # vehicle limits
    return contrained

# Simulate both vehicles
xA, yA, _ = simulate_vehicle(0, 0, 0, delta_A_func, imeche_wheelbase, v, T, dt)
xB, yB, _ = simulate_vehicle(0, 0, 0, delta_B_func, rc_wheelbase, v, T, dt)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(xA, yA, label="Vehicle A (IMECHE)", linewidth=2)
plt.plot(xB, yB, label="Vehicle B (RC)", linestyle='--')
plt.title("Trajectory Comparison")
plt.xlabel("X Position (m)")
plt.ylabel("Y Position (m)")
plt.legend()
plt.axis('equal')
plt.grid(True)
plt.show()
