# Webserv

This is an Nginx like HTTP server, built using `C++98` from scratch by me [Qirall79](https://github.com/Qirall79) and [ApplexX7](https://github.com/ApplexX7)

<img width="1301" alt="Screenshot 2024-11-16 at 20 56 06" src="https://github.com/user-attachments/assets/313ad8ba-20a0-4d77-9643-e8b05b829571">

## Features

- Configuration similar to Nginx
- Handle multiple concurrent clients using IO Multiplexing
- Serving static files and directories
- Directory listing in case of autoindex
- Handle GET, POST and DELETE
- File upload and deletion
- Video range handling
- CGI execution using any extension (should be specified in config file like so `cgi_path .py /usr/bin/python3`)

## How to run

```bash
  make
  ./webserv /path/to/config_file
```
