#!/usr/bin/python3
import cgi
import cgitb
import os

cgitb.enable()

request_method = os.getenv("REQUEST_METHOD", "")
content_length = os.getenv("CONTENT_LENGTH", "")

form = None
if request_method == "POST" and content_length and int(content_length) > 0:
    form = cgi.FieldStorage()

template_success_path = "./www/projects42/html/delete_success.html"
template_failure_path = "./www/projects42/html/delete_failure.html"

try:
    with open(template_success_path, 'r') as file:
        html_success_template = file.read()
    with open(template_failure_path, 'r') as file:
        html_failure_template = file.read()
except FileNotFoundError:
    print("HTTP/1.1 500 Internal Server Error")
    print("Content-Type: text/html")
    print()
    print("<html><body><h1>Erreur: Template HTML introuvable</h1></body></html>")
    exit(1)

if form:
    project_id = form.getvalue("project_id", None)

    if project_id:
        try:
            with open("./www/projects42/database/projects.txt", "r") as f:
                lines = f.readlines()

            with open("./www/projects42/database/projects.txt", "w") as f:
                skip = False
                for line in lines:
                    if line.startswith(f"ID: {project_id}"):
                        skip = True
                    elif line.startswith("ID:") and skip:
                        skip = False
                    if not skip:
                        f.write(line)

            content_length = len(html_success_template.encode('utf-8'))
            print("HTTP/1.1 200 OK")
            print(f"Content-Length: {content_length}")
            print("Content-Type: text/html")
            print()
            print(html_success_template)

        except FileNotFoundError:
            content_length = len(html_failure_template.encode('utf-8'))
            print("HTTP/1.1 500 Internal Server Error")
            print(f"Content-Length: {content_length}")
            print("Content-Type: text/html")
            print()
            print(html_failure_template)
    else:
        content_length = len(html_failure_template.encode('utf-8'))
        print("HTTP/1.1 400 Bad Request")
        print(f"Content-Length: {content_length}")
        print("Content-Type: text/html")
        print()
        print(html_failure_template)

else:
    content_length = len(html_failure_template.encode('utf-8'))
    print("HTTP/1.1 400 Bad Request")
    print(f"Content-Length: {content_length}")
    print("Content-Type: text/html")
    print()
    print(html_failure_template)
