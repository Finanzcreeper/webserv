import matplotlib.pyplot as plt
import sys
import sympy as sp
import numpy as np
import io

expression_str = sys.stdin.read()

x = sp.Symbol('x')
try:
    expr = sp.sympify(expression_str)
except sp.SympifyError:
    raise ValueError(f"Invalid expression: {expression_str}")

# Convert the symbolic expression to a lambda function
f = sp.lambdify(x, expr, modules='numpy')

# Create an array of x values
x_values = np.linspace(-1, 1, 50)
# Calculate y values using the lambda function
y_values = f(x_values)

buf = io.BytesIO()

# Plot the function
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label=f'${expression_str}$')
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Plot of the function')
plt.grid(True)
plt.legend()
plt.show()

plt.savefig(buf, format='jpeg')
plt.close()

buf.seek(0)
sys.stdout.buffer.write(buf.read())
