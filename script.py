import time

body = input()

header = f"Content-Length: {len(body)}\r\n\r\n"

print(body)