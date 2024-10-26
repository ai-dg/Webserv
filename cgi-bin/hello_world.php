<?php
// Envoyer les en-têtes HTTP
header("Content-Type: text/html");

// Vérifier si le nom est passé en POST
if (isset($_POST["name"])) {
    $hello = "Hello " . htmlspecialchars($_POST["name"]) . " !!!";
} else {
    $hello = "Hello World !!!";
}

// Construire le corps de la réponse
$body = "<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1.0'>
    <title>Document</title>
</head>
<body>$hello
</body>
</html>";

// En-têtes de réponse
$http_response_header = "HTTP/1.1 200 OK\r\n" .
        "Content-Type: text/html\r\n" .
        "Content-Length: " . strlen($body) . "\r\n\r\n" .
        $body;

// Écrire la réponse HTTP dans le flux de sortie
echo $http_response_header;
?>
