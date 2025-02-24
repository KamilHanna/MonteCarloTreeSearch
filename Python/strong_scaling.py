import matplotlib.pyplot as plt
import numpy as np

# Example data (execution time in milliseconds)
num_processors = np.array([1, 2, 4, 8, 16])
execution_time_ms = np.array([1865995, 1088749, 594374, 359583, 86389])  # In milliseconds

# Convert milliseconds to seconds
execution_time_seconds = execution_time_ms / 1000

# Calculate speedup
speedup = execution_time_seconds[0] / execution_time_seconds

# Plot strong scaling
plt.figure(figsize=(10, 6))
plt.plot(num_processors, execution_time_seconds, 'bo-', label='Execution Time (s)')
plt.plot(num_processors, speedup, 'ro-', label='Speedup')
plt.xlabel('Number of Threads')
plt.ylabel('Execution Time (s) / Speedup')
plt.title('Strong Scaling')
plt.legend()
plt.grid(True)
plt.savefig('strong_scaling.png')
plt.show()