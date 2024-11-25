#!/usr/bin/perl
use strict;
use warnings;
use CGI;

my $cgi = CGI->new;

# Envoi des en-têtes HTTP avec Content-Length
my $content = <<HTML;
<!DOCTYPE html>
<html lang="fr">
<head>
    <meta charset="UTF-8">
    <title>Hello World en CGI</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 40px;
            text-align: center;
        }
        h1 {
            color: #2c3e50;
        }
    </style>
</head>
<body>
    <h1>Bonjour le monde !</h1>
    <p>Ceci est ma première page générée en Perl CGI</p>
    <p>Date et heure actuelles : @{[scalar localtime]}</p>
</body>
</html>
HTML

# Calculer la longueur du contenu
use bytes;
my $length = length($content);
no bytes;

# Envoyer les en-têtes avec la longueur du contenu
print $cgi->header(
    -type    => 'text/html',
    -charset => 'utf-8',
    -Content_length => $length
);

# Envoyer le contenu
print $content;

# Forcer la vidange du buffer
$| = 1;
exit 0;