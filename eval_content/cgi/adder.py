import sys
import io

# Parse query string input
query_string = sys.stdin.read().strip()
pos1 = query_string.find("num1")
pos_and = query_string.find("&")
pos2 = query_string.find("num2")
num_1 = int(query_string[pos1 + len("num1="): pos_and])
num_2 = int(query_string[pos2 + len("num2="):])

result = num_1 + num_2
webpage = f"""
	<html>
	<head><title>Result</title></head>
	<body>
		<h1>Result: {num_1} + {num_2} = {result}</h1>
		<a href="/">Go Back</a>
	</body>
	</html>
	"""
print('HTTP/1.1 200 OK\r', flush=True)
print('Content-Type: text/html\r', flush=True)
print(f'Content-length: {len(webpage)}\r\n\r', flush=True)
print(webpage)