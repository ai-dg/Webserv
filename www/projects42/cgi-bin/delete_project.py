#!/usr/bin/python3
import cgitb
import os

cgitb.enable()


log_path = "./logs/debug_python.log"


def log_debug(message):
    with open(log_path, "a") as log_file:
        log_file.write(message + "\n")


request_method = os.getenv("REQUEST_METHOD", "")
query_string = os.getenv("QUERY_STRING", "")


project_id = None
if "project_id=" in query_string:
    project_id = query_string.split("project_id=")[-1].split("&")[0]


log_debug(f"REQUEST_METHOD: {request_method}")
log_debug(f"QUERY_STRING: {query_string}")
log_debug(f"Extracted project_id: {project_id}")

template_success_path = "./www/projects42/html/delete_success.html"
template_failure_path = "./www/projects42/html/delete_failure.html"

try:
    with open(template_success_path, 'r') as file:
        html_success_template = file.read()
    with open(template_failure_path, 'r') as file:
        html_failure_template = file.read()
except FileNotFoundError:
    log_debug("Template files not found.")
    print("HTTP/1.1 500 Internal Server Error")
    print("Content-Type: text/html")
    print()
    print("<html><body><h1>Erreur: Template HTML introuvable</h1></body></html>")
    exit(1)


if request_method == "DELETE":
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

            log_debug(f"Project with ID {project_id} successfully deleted.")
            content_length = len(html_success_template.encode('utf-8'))
            print("HTTP/1.1 200 OK")
            print(f"Content-Length: {content_length}")
            print("Content-Type: text/html")
            print()
            print(html_success_template)

        except FileNotFoundError:
            log_debug("Database file not found during deletion.")
            content_length = len(html_failure_template.encode('utf-8'))
            print("HTTP/1.1 500 Internal Server Error")
            print(f"Content-Length: {content_length}")
            print("Content-Type: text/html")
            print()
            print(html_failure_template)
    else:
        log_debug("project_id missing in QUERY_STRING.")
        content_length = len(html_failure_template.encode('utf-8'))
        print("HTTP/1.1 400 Bad Request")
        print(f"Content-Length: {content_length}")
        print("Content-Type: text/html")
        print()
        print(html_failure_template)
else:
    log_debug("Invalid method. Only DELETE is supported.")
    content_length = len(html_failure_template.encode('utf-8'))
    print("HTTP/1.1 405 Method Not Allowed")
    print(f"Content-Length: {content_length}")
    print("Content-Type: text/html")
    print()
    print(html_failure_template)
