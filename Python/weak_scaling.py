import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import minimize
# Example data (execution time in milliseconds)
num_processors = np.array([1, 2, 4, 8, 16])
problem_size = num_processors * 1000  # Problem size scales with processors
execution_time_ms = np.array([1865995, 1088749, 594374, 359583, 86389])  # In milliseconds

# Convert milliseconds to seconds
execution_time_seconds = execution_time_ms / 1000

# Plot weak scaling
plt.figure(figsize=(10, 6))
plt.plot(num_processors, execution_time_seconds, 'go-', label='Execution Time (s)')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (s)')
plt.title('Weak Scaling')
plt.legend()
plt.grid(True)
plt.savefig('weak_scaling.png')
plt.show()