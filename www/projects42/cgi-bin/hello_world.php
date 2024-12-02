
<?php

header("HTTP/1.1 200 OK");
header("Content-Type: text/html");


if (isset($_POST["name"])) {
    $hello = "Hello " . htmlspecialchars($_POST["name"]) . " !!!";
} else {
    $hello = "Hello World !!!";
}

$body = "<!DOCTYPE html>
<html lang=\"en\">
<head>
    <meta charset=\"UTF-8\">
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
    <title>Document</title>
</head>
<body>" . $hello . "
</body>
</html>";
header("Content-Length: " . strlen($body));
echo $body;
?>

?>