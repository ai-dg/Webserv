#!/usr/bin/python3
import cgi
import cgitb
import os
import logging

# 
cgitb.enable()


logging.basicConfig(
    filename='./logs/error_python.log',
    level=logging.ERROR,
    format='%(asctime)s - %(levelname)s - %(message)s'
)

logging.debug("Script started.")

image_upload_path = "./www/projects42/upload/"
default_image_url = "./www/projects42/images/Default.jpg"


request_method = os.getenv("REQUEST_METHOD", "")
content_length = os.getenv("CONTENT_LENGTH", "")

logging.debug(f"REQUEST_METHOD: {request_method}")
logging.debug(f"CONTENT_LENGTH: {content_length}")


form = None
if request_method == "POST" and content_length and int(content_length) > 0:
    form = cgi.FieldStorage()
    logging.debug("Form data loaded successfully.")
else:
    logging.warning("No POST request or content length is missing.")

image_url = default_image_url

if form and 'image' in form:
    image_field = form['image']
    if image_field.filename:
        try:
            image_filename = os.path.basename(image_field.filename)
            image_path = os.path.join(image_upload_path, image_filename)
            image_url = image_path  
            with open(image_path, "wb") as f:
                f.write(image_field.file.read())
            logging.debug(f"Image saved successfully at: {image_path}")
        except Exception as img_err:
            logging.error(f"Error saving image: {img_err}")

template_success_path = "./www/projects42/html/add_success.html"
template_failure_path = "./www/projects42/html/add_failure.html"

image_url = image_url.replace("./www/projects42", "")


try:
    with open(template_success_path, 'r') as file:
        html_success_template = file.read()
    with open(template_failure_path, 'r') as file:
        html_failure_template = file.read()
except FileNotFoundError as e:
    logging.error(f"Template file not found: {e}")
    print("Content-Type: text/html")
    print()
    print("<html><body><h1>Erreur: Template HTML introuvable</h1></body></html>")
    exit(1)


if form:
    for key in form.keys():
        logging.debug(f"Form field {key}: {form.getvalue(key)}")

def get_next_project_id():
    """Récupère le prochain ID de projet disponible"""
    try:
        with open("./www/projects42/database/projects.txt", "r") as f:
            lines = f.readlines()
            id_lines = [line for line in lines if line.startswith("ID:")]
            if id_lines:
                last_id = int(id_lines[-1].split(":")[1].strip())
                return last_id + 1
            else:
                return 1
    except FileNotFoundError:
        logging.info("projects.txt not found. Starting with ID 1.")
        return 1
    except Exception as e:
        logging.error(f"Error reading project ID: {e}")
        return 1

if form:
    
    try:
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
        html_success_template = html_success_template.replace("{{image_url}}", image_url)

        
        with open("./www/projects42/database/projects.txt", "a") as f:
            f.write(f"ID: {project_id}\n")
            f.write(f"Nom du projet: {project_name}\n")
            f.write(f"Note: {project_grade}\n")
            f.write(f"Temps prévu: {expected_time} heures\n")
            f.write(f"Temps réel: {real_time} heures\n")
            f.write(f"Expérience: {experience}\n")
            f.write(f"Satisfaction: {satisfaction}/5\n")
            f.write(f"Commentaires: {comments}\n")
            f.write(f"Image_URL: {image_url}\n")
            f.write("-" * 40 + "\n")

        logging.debug("Project details written to projects.txt.")

        
        print("HTTP/1.1 200 OK")
        print("Content-Type: text/html")
        print(f"Content-Length: {len(html_success_template)}")
        print()
        print(html_success_template)

    except Exception as e:
        logging.error(f"Error processing form data: {e}")
        print("HTTP/1.1 500 Internal Server Error")
        print("Content-Type: text/html")
        print()
        print("<html><body><h1>Erreur lors du traitement du formulaire</h1></body></html>")

else:
    
    logging.warning("No form data found.")
    print("HTTP/1.1 400 Bad Request")
    print("Content-Type: text/html")
    print(f"Content-Length: {len(html_failure_template)}")
    print()
    print(html_failure_template)
