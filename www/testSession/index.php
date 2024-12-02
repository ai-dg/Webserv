<?php
// Fonction pour écrire des messages de débogage
function debug_log($message) {
    $logFile = __DIR__ . '/logs/php.log';
    if (!file_exists(dirname($logFile))) {
        mkdir(dirname($logFile), 0777, true);
    }
    file_put_contents($logFile, "[" . date('Y-m-d H:i:s') . "] " . $message . PHP_EOL, FILE_APPEND);
}

// Activer le débogage des sessions
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);
error_reporting(E_ALL);

// Configurer les sessions
ini_set('session.use_strict_mode', 1);
ini_set('session.use_cookies', 1);
ini_set('session.use_only_cookies', 1);
ini_set('session.cookie_lifetime', 0);

// Vérifier si l'ID de session est transmis dans l'environnement
if (isset($_SERVER['PHPSESSID'])) {
    debug_log("PHPSESSID trouvé dans l'environnement : " . $_SERVER['PHPSESSID']);
    session_id($_SERVER['PHPSESSID']); // Utiliser l'ID transmis via l'environnement
} elseif (isset($_COOKIE['PHPSESSID'])) {
    debug_log("PHPSESSID reçu du cookie : " . $_COOKIE['PHPSESSID']);
    session_id($_COOKIE['PHPSESSID']); // Utiliser l'ID transmis via le cookie
} else {
    debug_log("Aucun PHPSESSID trouvé.");
}

// Debugger toutes les variables d'environnement
debug_log("Contenu de \$_SERVER : " . print_r($_SERVER, true));
debug_log("Variables d'environnement via getenv() : " . print_r(getenv(), true));

// Vérifier si le fichier de session existe
$session_save_path = ini_get('session.save_path');
$session_file = $session_save_path . '/sess_' . session_id();

if (file_exists($session_file)) {
    debug_log("Fichier de session trouvé : " . $session_file);
} else {
    debug_log("Fichier de session introuvable : " . $session_file);
}

// Démarrer la session
debug_log("Démarrage de session...");
session_start();
debug_log("ID de session actuel : " . session_id());

// Gestion de la connexion
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['login'])) {
        $username = trim($_POST['username']);
        debug_log("Tentative de connexion avec le nom d'utilisateur : " . $username);
        if (!empty($username)) {
            $_SESSION['user'] = $username;
            debug_log("Utilisateur connecté : " . $username);
        } else {
            debug_log("Nom d'utilisateur vide, connexion ignorée.");
        }
    } elseif (isset($_POST['logout'])) {
        debug_log("Déconnexion demandée.");
        session_unset();
        session_destroy();
        debug_log("Session détruite.");
    } else {
        debug_log("Requête POST inconnue.");
    }
}

if (isset($_SESSION['user'])) {
    $username = htmlspecialchars($_SESSION['user'], ENT_QUOTES, 'UTF-8');
    debug_log("Session active pour l'utilisateur : " . $username);
} else {
    debug_log("Aucune session utilisateur active.");
}

// En-têtes HTTP
debug_log("Envoi des en-têtes HTTP pour le cache.");
header("Expires: 0");
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Pragma: no-cache");

// Indiquer la fin du script dans les logs
debug_log("Fin du script PHP.");
?>





<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Test Sessions</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; }
        input, button { padding: 10px; margin: 5px; }
    </style>
</head>
<body>
    <h1>Test Sessions</h1>

    <?php
    if (isset($_SESSION['user'])): ?>
        <p>Bienvenue, <strong><?php echo htmlspecialchars($_SESSION['user']); ?></strong> !</p>
        <form method="POST">
            <button type="submit" name="logout">Se déconnecter</button>
        </form>
    <?php else: ?>
        <p>Vous n'êtes pas connecté.</p>
        <form method="POST">
            <input type="text" name="username" placeholder="Entrez votre nom" required>
            <button type="submit" name="login">Se connecter</button>
        </form>
    <?php endif; ?>
</body>
</html>