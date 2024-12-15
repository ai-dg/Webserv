#!/bin/bash

export $(cat | xargs)


env > env.txt


# Envoi des en-têtes HTTP
echo "Content-type: text/html"
echo "Content-length: $(wc -c << 'EOF'
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Hello World en Bash CGI</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            text-align: center;
            background-color: #f0f0f0;
        }
        h1 {
            color: #2c3e50;
        }
        .info {
            background-color: white;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            margin: 20px auto;
            max-width: 600px;
        }
    </style>
</head>
<body>
    <h1>Bonjour $(name) !</h1>
    <div class="info">
        <p>Ceci est ma première page générée en Bash CGI</p>
        <p>Date et heure : $(date)</p>
        <p>Nom du serveur : $(hostname)</p>
    </div>
</body>
</html>
EOF
)"
echo ""

# Envoi du contenu HTML
cat << EOF
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Hello World en Bash CGI</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            text-align: center;
            background-color: #f0f0f0;
        }
        h1 {
            color: #2c3e50;
        }
        .info {
            background-color: white;
            padding: 20px;
            border-radius: 5px;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
            margin: 20px auto;
            max-width: 600px;
        }
    </style>
</head>
<body>
    <h1>Bonjour $(echo $name) !</h1>
    <div class="info">
        <p>Ceci est ma première page générée en Bash CGI</p>
        <p>Date et heure : $(date)</p>
        <p>Nom du serveur : $(hostname)</p>
    </div>
</body>
</html>
EOF
