<?php

if (isset($_COOKIE['visitCount'])) {
    $visitCount = $_COOKIE['visitCount'] + 1;
} else {
    $visitCount = 1;
}

setcookie('visitCount', $visitCount, time() + 31536000, '/');


$content = "
<!DOCTYPE html>
<html lang=\"fr\">
<head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <link rel=\"stylesheet\" href=\"style.css\">
    <title>Test de Cookies</title>
</head>
<body>
    <header>
        <div class=\"title\">Testing Webserv ...</div>
        <div class=\"nav\">
            <a class=\"nav_link\" href=\"index.html\">Home</a>
            <a class=\"nav_link\" href=\"bash.html\">Bash.sh script</a>
            <a class=\"nav_link\" href=\"unkown.html\">unkown method</a>
            <a class=\"nav_link\" href=\"bashCgi.html\">Bash.cgi script</a>
            <a class=\"nav_link\" href=\"php.html\">Php script</a>
            <a class=\"nav_link\" href=\"putfiles.html\">Test PUT</a>
            <a class=\"nav_link\" href=\"p.html\">A non existent file...</a>
            <a class=\"nav_link\" target=\"_blank\" href=\"/google/\">External redirections</a>
            <a class=\"nav_link\" target=\"_blank\" href=\"/sessions/index.php\">Internal redirections</a>
            <a class=\"nav_link\" href=\"/files/\">Display Directory</a>
            <a class=\"nav_link\" href=\"counter.php\">Cookies</a>
        </div>
    </header>
    <div class=\"subtitle\">Testing Cookies</div>
    <div class=\"container\">
        Cette page a été appelée :
        <div id=\"cookieValue\">" . $visitCount . "</div>
        fois
    </div>
</body>
</html>";

$content_length = strlen($content);
header("Content-Type: text/html; charset=UTF-8");
header("Content-Length: " . $content_length);
header("Connection: close");

echo $content;
?>