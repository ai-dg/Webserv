function handleDelete(event, projectId) {
    event.preventDefault(); // Empêche la soumission par défaut

    fetch(`delete_project.py?project_id=${projectId}`, {
        method: 'DELETE',
    })
        .then(response => response.text())
        .then(body => {
            document.body.innerHTML = body;
        })
        .catch(error => {
            console.error("Erreur réseau :", error);
        });
}
