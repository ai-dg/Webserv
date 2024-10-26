#!/bin/bash

# Configuration
URL="http://localhost:8080"  # URL de votre serveur
NUM_REQUESTS=100             # Nombre total de requêtes à envoyer
CONCURRENCY=10               # Nombre de requêtes simultanées

# Lancer les requêtes en parallèle
for ((i=1; i<=$NUM_REQUESTS; i++)); do
    (curl -s -o /dev/null -w "%{http_code}" $URL &)   # Envoie la requête et ignore la réponse
    if (( $i % $CONCURRENCY == 0 )); then
        wait  # Attend que les requêtes concurrentes se terminent
    fi
done
wait
