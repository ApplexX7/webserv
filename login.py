import os
import sys
import json

body = ""

env = os.environ

method = env['REQUEST_METHOD']

cookie = env.get('COOKIE')


if method == 'GET':
    # todo: handle get
    body = """
    <!DOCTYPE html>
    <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Login Form</title>
            <style>

                * {
                    box-sizing: border-box;
                }

                body {
                    background-color: black;
                    color: white;
                    font-family: Arial, sans-serif;
                    display: flex;
                    justify-content: center;
                    align-items: center;
                    height: 100vh;
                    margin: 0;
                }
                .form-container {
                    width: 300px;
                    display: flex;
                    flex-direction: column;
                }
                h2 {
                    margin-bottom: 20px;
                }
                input[type="text"], input[type="password"] {
                    width: 100%;
                    padding: 10px;
                    margin-bottom: 12px;
                    border: none;
                    border-radius: 4px;
                    color: white;
                    font-size: 16px;
                }

                input[type="submit"] {
                    width: 100%;
                    padding: 10px;
                    background-color: #007bff;
                    color: white;
                    border: none;
                    border-radius: 4px;
                    cursor: pointer;
                    font-size: 16px;
                }
                input[type="submit"]:hover {
                    background-color: #024c9c;
                }
            </style>
        </head>
        <body>
            <div class="form-container">
                <h2>Login</h2>
                <form action="/Desktop/webserv/login.py" method="post">
                    <input type="text" name="username" placeholder="Username" required><br>
                    <input type="password" name="password" placeholder="Password" required><br>
                    <input type="submit" value="Log In">
                </form>
            </div>

        </body>
    </html>
    """
    header = f"Content-Type: text/html\r\nContent-Length: {len(body)}\r\nStatus: 201\r\n\r\n"

else:
    # todo: handle post
    # for line in sys.stdin:
    #     body += line

    body = str(cookie)
    
    header = f"Content-Length: {len(body)}\r\nStatus: 200\r\nSet-Cookie: session=hello\r\n\r\n"

print(header + body)


#Get HTTp    1.1