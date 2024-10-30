#!/usr/bin/python3
import cgi
import cgitb
import os
import shutil 
import sys
import mimetypes
from pathlib import Path

# Active le débogage CGI
cgitb.enable()

# Configuration pour les images
UPLOAD_DIR = "./www/html/images"
ALLOWED_EXTENSIONS = {'.jpg', '.jpeg', '.png', '.gif'}
MAX_FILE_SIZE = 8 * 1024 * 1024  # 5 MB

def is_valid_image(fileitem):
    """Vérifie si le fichier est une image valide"""
    if not fileitem.filename:
        return False
    
    # Vérifie l'extension
    file_ext = os.path.splitext(fileitem.filename)[1].lower()
    if file_ext not in ALLOWED_EXTENSIONS:
        return False
    
    # Vérifie le type MIME
    mime_type = mimetypes.guess_type(fileitem.filename)[0]
    if not mime_type or not mime_type.startswith('image/'):
        return False
    
    return True

def sanitize_filename(filename):
    """Nettoie le nom de fichier pour le rendre sûr"""
    filename = os.path.basename(filename)
    return ''.join(c for c in filename if c.isalnum() or c in '._-')

# Récupération des variables d'environnement
request_method = os.getenv("REQUEST_METHOD", "")
content_length = os.getenv("CONTENT_LENGTH", "")

# Initialisation du formulaire
form = None
if request_method == "POST" and content_length and int(content_length) > 0:
    form = cgi.FieldStorage()

# Chemins des templates
template_success_path = "./www/html/add_success.html"
template_failure_path = "./www/html/add_failure.html"

# Lecture des templates HTML
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

# Débogage - affichage des données du formulaire
for key in form.keys() if form else []:
    print(f"{key}: {form.getvalue(key)}", file=sys.stderr)

def get_next_project_id():
    """Récupère le prochain ID de projet disponible"""
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
    # Récupération des données du formulaire
    project_id = get_next_project_id()
    project_name = form.getvalue("projectName", "N/A")
    project_grade = form.getvalue("projectGrade", "N/A")
    expected_time = form.getvalue("expectedTime", "N/A")
    real_time = form.getvalue("realTime", "N/A")
    experience = form.getvalue("experience", "N/A")
    satisfaction = form.getvalue("satisfaction", "N/A")
    comments = form.getvalue("comments", "N/A")

    # Gestion de l'upload de fichier
    file_path = None
    upload_error = None
    if "image" in form:
        uploaded_file = form["image"]
        # Débogage modifié pour éviter l'erreur fileno
        print(f"Nom du fichier: {uploaded_file.filename}", file=sys.stderr)
        print(f"Type du fichier: {uploaded_file.type}", file=sys.stderr)
        
        if isinstance(uploaded_file, cgi.FieldStorage) and uploaded_file.file:
            try:
                # Vérifie si c'est une image valide
                if not is_valid_image(uploaded_file):
                    upload_error = "Format de fichier non valide"
                else:
                    # Crée le répertoire si nécessaire
                    os.makedirs(UPLOAD_DIR, exist_ok=True)
                    
                    # Nettoie et sécurise le nom de fichier
                    safe_filename = sanitize_filename(uploaded_file.filename)
                    
                    # Ajoute un timestamp si le fichier existe déjà
                    base, ext = os.path.splitext(safe_filename)
                    if os.path.exists(os.path.join(UPLOAD_DIR, safe_filename)):
                        import time
                        safe_filename = f"{base}_{int(time.time())}{ext}"
                    
                    file_path = os.path.join(UPLOAD_DIR, safe_filename)
                    
                    # Écriture du fichier avec gestion du buffer
                    with open(file_path, "wb") as output_file:
                        while True:
                            chunk = uploaded_file.file.read(8192)
                            if not chunk:
                                break
                            output_file.write(chunk)
                    
                    if not os.path.exists(file_path):
                        upload_error = "Erreur lors de l'écriture du fichier"
                        file_path = None
                    
            except Exception as e:
                upload_error = f"Erreur lors de l'upload: {str(e)}"
                file_path = None
                print(f"Erreur d'upload: {str(e)}", file=sys.stderr)

    # Remplacement des variables dans le template
    html_success_template = html_success_template.replace("{{project_name}}", project_name)
    html_success_template = html_success_template.replace("{{project_grade}}", project_grade)
    html_success_template = html_success_template.replace("{{expected_time}}", expected_time)
    html_success_template = html_success_template.replace("{{real_time}}", real_time)
    html_success_template = html_success_template.replace("{{experience}}", experience)
    html_success_template = html_success_template.replace("{{satisfaction}}", satisfaction)
    html_success_template = html_success_template.replace("{{comments}}", comments)

    # Écriture dans le fichier projects.txt
    with open("./sessions/projects.txt", "a") as f:
        f.write(f"ID: {project_id}\n")
        f.write(f"Nom du projet: {project_name}\n")
        f.write(f"Note: {project_grade}\n")
        f.write(f"Temps prévu: {expected_time} heures\n")
        f.write(f"Temps réel: {real_time} heures\n")
        f.write(f"Expérience: {experience}\n")
        f.write(f"Satisfaction: {satisfaction}/5\n")
        f.write(f"Commentaires: {comments}\n")
        if file_path:
            f.write(f"Fichier image: {file_path}\n")
        if upload_error:
            f.write(f"Erreur upload: {upload_error}\n")
        f.write("-" * 40 + "\n")

    # Envoi de la réponse HTTP
    print("HTTP/1.1 200 OK")
    print("Content-Type: text/html")
    print(f"Content-Length: {len(html_success_template)}")
    print()
    print(html_success_template)

else:
    # Gestion du cas d'erreur
    print("HTTP/1.1 400 Bad Request")
    print("Content-Type: text/html")
    print(f"Content-Length: {len(html_failure_template)}")
    print()
    print(html_failure_template)