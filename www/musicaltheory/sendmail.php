<?php
// Paramètres de l'email
$to = 'chrisap@live.fr'; // Remplace par ton adresse email personnelle
$subject = 'Test d\'envoi d\'email';
$message = 'Bonjour, ceci est un test d\'envoi d\'email depuis PHP!';
$headers = 'From: admin@musicaltheory.42.fr' . "\r\n" .
           'Reply-To: admin@musicaltheory.42.fr' . "\r\n" .
           'X-Mailer: PHP/' . phpversion();

// Envoi de l'email
if(mail($to, $subject, $message, $headers)) {
    echo 'L\'email a été envoyé avec succès!';
} else {
    echo 'Échec de l\'envoi de l\'email.';
}
?>
