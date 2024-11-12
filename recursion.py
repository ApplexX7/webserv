import requests

response = requests.get("http://localhost:3000/Desktop/webserv/recursion.py")

print(response.text)