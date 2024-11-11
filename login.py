import os

body = """
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
    hello there
"""

# body = input()




# generate cookies based of username/password
header = f"Content-Length: {len(body)}\r\nStatus: 201\r\n\r\n"


print(header + body)