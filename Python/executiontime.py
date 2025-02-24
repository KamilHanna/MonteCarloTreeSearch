import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize

# Example data (execution time in milliseconds)
num_simulations = np.array([10, 100, 1000, 10000])
execution_time_ms = np.array([903, 10290, 86389, 928900])  # In milliseconds

# Convert milliseconds to seconds
execution_time_seconds = execution_time_ms / 1000

# Plot execution time vs. number of simulations
plt.figure(figsize=(10, 6))
plt.plot(num_simulations, execution_time_seconds, 'mo-', label='Execution Time (s)')
plt.xlabel('Number of Simulations')
plt.ylabel('Execution Time (s)')
plt.title('Execution Time vs. Number of Simulations')
plt.legend()
plt.grid(True)
plt.savefig('execution_time.png')
plt.show()