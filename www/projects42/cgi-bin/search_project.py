#!/usr/bin/python3
import cgi
import cgitb
import os

cgitb.enable()

projects_file_path = "./www/projects42/database/projects.txt"
template_path = "./www/projects42/html/find_results.html"

request_method = os.getenv("REQUEST_METHOD", "")
content_length = os.getenv("CONTENT_LENGTH", "")

form = None
if request_method == "POST" and content_length and int(content_length) > 0:
    form = cgi.FieldStorage()

search_results = []

if form:
    project_name_search = form.getvalue("projectName", "").strip().lower()

    try:
        with open(projects_file_path, "r") as file:
            project = {}
            for line in file:
                line = line.strip()
                if line == "----------------------------------------":
                    if project.get("Nom du projet", "").lower() == project_name_search:
                        search_results.append(project)
                    project = {}
                elif ": " in line:
                    key, value = line.split(": ", 1)
                    project[key] = value
    except FileNotFoundError:
        search_results.append({"error": "Fichier projects.txt introuvable."})

results_html = ""
if search_results:
    for project in search_results:
        results_html += "<div class='project-result'>"
        if "error" in project:
            results_html += f"<p class='text-danger'>{project['error']}</p>"
        else:
            image_url = project.get("Image_URL", "./www/html/images/Default.jpg")  # Chemin par défaut si aucune image n'est fournie
            results_html += f"""
            <div class="project-image">
                <img src="{image_url}" alt="Image du projet" style="max-width: 400px; height: 300px;">
            </div>
            """
            results_html += f"<p><strong>Nom du projet</strong>: {project.get('Nom du projet', 'N/A')}</p>"
            results_html += f"<p><strong>Note</strong>: {project.get('Note', 'N/A')}</p>"
            results_html += f"<p><strong>Temps prévu</strong>: {project.get('Temps prévu', 'N/A')}</p>"
            results_html += f"<p><strong>Temps réel</strong>: {project.get('Temps réel', 'N/A')}</p>"
            results_html += f"<p><strong>Expérience</strong>: {project.get('Expérience', 'N/A')}</p>"
            results_html += f"<p><strong>Satisfaction</strong>: {project.get('Satisfaction', 'N/A')}</p>"
            results_html += f"<p><strong>Commentaires</strong>: {project.get('Commentaires', 'N/A')}</p>"
        results_html += "</div><hr>"
else:
    results_html = "<p>Aucun projet trouvé avec ce nom.</p>"

try:
    with open(template_path, "r") as template_file:
        template_content = template_file.read()
except FileNotFoundError:
    template_content = """
    <!DOCTYPE html>
    <html lang="fr">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
        <title>Résultats de la recherche</title>
    </head>
    <body>
        <div class="container mt-4">
            <h2>Erreur</h2>
            <p>Template introuvable.</p>
        </div>
    </body>
    </html>
    """

html_output = template_content.replace("{{search_results}}", results_html)
content_length = len(html_output.encode('utf-8'))

print("HTTP/1.1 200 OK")
print(f"Content-Length: {content_length}")
print("Content-Type: text/html")
print()
print(html_output)
