#!/usr/bin/python3
import os

projects_file = "./www/projects42/database/projects.txt"

html_template = """
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link href="styles.css" rel="stylesheet">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css">
    <script src="delete.js"></script> <!-- Inclusion du script -->
    <title>Supprimer un projet</title>
</head>
<body>
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark">
        <a class="navbar-brand" href="index.html">Home</a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarNav"
            aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarNav">
            <ul class="navbar-nav">
                <li class="nav-item">
                    <a class="nav-link blue-button" href="introduce.html">Introduction</a>
                </li>
                <li class="nav-item dropdown">
                    <a class="nav-link dropdown-toggle white-button" id="projectDropdown" data-bs-toggle="dropdown"
                        aria-expanded="false" href="#">
                        Projets
                    </a>
                    <ul class="dropdown-menu" aria-labelledby="projectDropdown">
                        <li><a class="dropdown-item font-size-button" href="add.html">Ajouter un projet</a></li>
                        <li><a class="dropdown-item font-size-button" href="find.html">Chercher un projet</a></li>
                        <li><a class="dropdown-item font-size-button" href="delete.html">Supprimer un projet</a></li>
                    </ul>
                </li>
            </ul>
        </div>
    </nav>
    <div class="custom-margin-top">
        <h2>Liste des projets</h2>
        <table class="table table-hover table-striped">
            <thead>
                <tr>
                    <th>Nom du projet</th>
                    <th>Note</th>
                    <th>Temps prévu (heures)</th>
                    <th>Temps réel (heures)</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
"""


try:
    with open(projects_file, "r") as f:
        projects = f.read().strip().split("----------------------------------------\n")

    for project in projects:
        if project.strip():
            lines = project.strip().split("\n")
            project_data = {}
            for line in lines:
                if ": " in line:
                    key, value = line.split(": ", 1)
                    project_data[key.strip()] = value.strip()

            html_template += f"""
                <tr>
                    <td>{project_data.get('Nom du projet', 'N/A')}</td>
                    <td>{project_data.get('Note', 'N/A')}</td>
                    <td>{project_data.get('Temps prévu', 'N/A')}</td>
                    <td>{project_data.get('Temps réel', 'N/A')}</td>
                    <td>
                        <button 
                            class="btn btn-primary" 
                            onclick="handleDelete(event, '{project_data.get('ID', '')}')">
                            Supprimer
                        </button>
                    </td>
                </tr>
            """
except FileNotFoundError:
    html_template += "<tr><td colspan='6'>Le fichier des projets n'a pas été trouvé.</td></tr>"

html_template += """
            </tbody>
        </table>
    </div>
    <script src="script.js"></script>
    <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js"></script>
</body>
</html>
"""

content_length = len(html_template.encode('utf-8'))  


print("HTTP/1.1 200 OK")
print(f"Content-Length: {content_length}")
print("Content-Type: text/html")
print()  


print(html_template)

