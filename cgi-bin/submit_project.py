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

template_success_path = "./www/html/add_success.html"
template_failure_path = "./www/html/add_failure.html"

try:
    with open(template_success_path, 'r') as file:
        html_success_template = file.read()
    with open(template_failure_path, 'r') as file:
        html_failure_template = file.read()
except FileNotFoundError:
    print("Content-Type: text/html")
    print()
    print("<html><body><h1>Erreur: Template HTML introuvable</h1></body></html>")
    exit(1)


def get_next_project_id():
    try:
        with open("./sessions/projects.txt", "r") as f:
            lines = f.readlines()
            
            id_lines = [line for line in lines if line.startswith("ID:")]
            if id_lines:
                
                last_id = int(id_lines[-1].split(":")[1].strip())
                return last_id + 1
            else:
                
                return 1
    except FileNotFoundError:
        
        return 1


if form:
    project_id = get_next_project_id()
    project_name = form.getvalue("projectName", "N/A")
    project_grade = form.getvalue("projectGrade", "N/A")
    expected_time = form.getvalue("expectedTime", "N/A")
    real_time = form.getvalue("realTime", "N/A")
    experience = form.getvalue("experience", "N/A")
    satisfaction = form.getvalue("satisfaction", "N/A")
    comments = form.getvalue("comments", "N/A")

    
    html_success_template = html_success_template.replace("{{project_name}}", project_name)
    html_success_template = html_success_template.replace("{{project_grade}}", project_grade)
    html_success_template = html_success_template.replace("{{expected_time}}", expected_time)
    html_success_template = html_success_template.replace("{{real_time}}", real_time)
    html_success_template = html_success_template.replace("{{experience}}", experience)
    html_success_template = html_success_template.replace("{{satisfaction}}", satisfaction)
    html_success_template = html_success_template.replace("{{comments}}", comments)

    
    with open("./sessions/projects.txt", "a") as f:
        f.write(f"ID: {project_id}\n")
        f.write(f"Nom du projet: {project_name}\n")
        f.write(f"Note: {project_grade}\n")
        f.write(f"Temps prévu: {expected_time} heures\n")
        f.write(f"Temps réel: {real_time} heures\n")
        f.write(f"Expérience: {experience}\n")
        f.write(f"Satisfaction: {satisfaction}/5\n")
        f.write(f"Commentaires: {comments}\n")
        f.write("-" * 40 + "\n")

    print("HTTP/1.1 200 OK")
    print("Content-Type: text/html")
    print()
    print(html_success_template)

else:
    print("HTTP/1.1 400 Bad Request")
    print("Content-Type: text/html")
    print()
    print(html_failure_template)
