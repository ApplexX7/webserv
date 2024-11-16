# Webserv

This is an Nginx like HTTP server built using c++98 from scratch

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
