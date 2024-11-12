import os
import sys
import json
from urllib.parse import unquote
import base64
impobcvcbcvrt time

body = ""
header = ""

env = os.environ

method = env['REQUEST_METHOD']

cookies = env.get('COOKIE')
session_cookie = ""

logged_in = False

if cookies and "session=" in cookies:
    if ';' in cookies:
        split_cookies = cookies.split(";")
        for c in split_cookies:
            if c.startswith("session="):
                session_cookie = c
                break
    else:
        session_cookie = cookies

    _, value = session_cookie.split("session=")

    session_valid = True
    # todo: check database if this is a valid session
    try:
        sessions = []
        last_match = 0
        with open("/tmp/.sessions.csv", 'r') as db:
            sessions = db.readlines()
        for s in sessions:
            if s.startswith(value):
                last_match = int(s.split("|")[1])
        current_time = int(time.time())
        if last_match > current_time:
            logged_in = True
        else:
            logged_in = False

    except Exception:
        logged_in = False
    
if not logged_in:
    if method == 'GET':
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
                    <form action="/Desktop/webserv/www/login.py" method="post">
                        <input type="text" name="username" placeholder="Username" required><br>
                        <input type="password" name="password" placeholder="Password" required><br>
                        <input type="submit" value="Log In">
                    </form>
                </div>

            </body>
        </html>
        """
        header = f"Content-Type: text/html\r\nContent-Length: {len(body)}\r\nStatus: 200\r\n\r\n"

    elif method == 'POST':
        for line in sys.stdin:
            body += line

        body = unquote(body)
        token = ""
        try:
            username, password = body.split("&")

            username = username.split("=")[1]
            password = password.split("=")[1]
            
            credentials = username + ":" + password
            token = base64.b64encode(credentials.encode('utf-8')).decode('utf-8')
            expires_at = int(time.time()) + 60
            
            session = token + "|" + str(expires_at)

            with open("/tmp/.sessions.csv", 'a') as db:
                db.write(session + "\n")

            body = "Logged in successfully"
        except Exception:
            body = "invalid credentials"
        
        header = f"Content-Length: {len(body)}\r\nStatus: 201\r\nSet-Cookie:session={token}; Max-Age=3600\r\n\r\n"

else:
    body = "Logged in"

print(header + body)