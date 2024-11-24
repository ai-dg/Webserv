<?php
if (session_status() == PHP_SESSION_NONE) {
    session_start();
}

// Gestion de la connexion
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_POST['login'])) {
        // Connexion
        $username = trim($_POST['username']);
        if (!empty($username)) {
            $_SESSION['user'] = $username;        
        }
    } elseif (isset($_POST['logout'])) {
        // Déconnexion
        session_unset(); // Vide la session
        session_destroy(); // Détruit la session
    }
    else {
        echo "username : ".$username;
    }
}

if (($_SERVER['REQUEST_METHOD'] ==='GET') && isset($_SESSION['user']))
{
    $username = htmlspecialchars($_SESSION['user'], ENT_QUOTES, 'UTF-8');
}

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