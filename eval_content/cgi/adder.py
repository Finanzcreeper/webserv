import sys
import io
from urllib.parse import parse_qs

# Parse query string input
query_string = sys.stdin.read().strip()
parsed_query = dict(parse_qs(query_string))
num_1 = int(parsed_query['num1'][0])
num_2 = int(parsed_query['num2'][0])

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