import matplotlib.pyplot as plt
import sys
import sympy as sp
import numpy as np
import io
from urllib.parse import parse_qs

# Parse query string input
query_string = sys.stdin.read().strip()
parsed_query = dict(parse_qs(query_string))
expression_string = parsed_query['expression'][0]
x_min = int(parsed_query['x_min'][0])
x_max = int(parsed_query['x_max'][0])

# Create mathematical expression
x = sp.Symbol('x')
try:
    expr = sp.sympify(parsed_query['expression'][0])
except sp.SympifyError:
    raise ValueError(f"Invalid expression: {expression_string}")

# Convert the symbolic expression to a lambda function
f = sp.lambdify(x, expr, modules='numpy')

# Create an array of x values
x_values = np.linspace(x_min, x_max, 50)
# Calculate y values using the lambda function
y_values = f(x_values)

buf = io.BytesIO()

# Plot the function
plt.figure(figsize=(8, 6))
plt.plot(x_values, y_values, label=f"${expression_string}$")
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Plot of the function')
plt.grid(True)
plt.legend()
plt.show()

plt.savefig(buf, format='jpeg')
plt.close()

buf.seek(0)
print('HTTP/1.1 200 OK\r', flush=True)
print('Content-Type: image/jpeg\r', flush=True)
print(f'Content-length: {buf.getbuffer().nbytes}\r\n\r', flush=True)
sys.stdout.buffer.write(buf.read())
