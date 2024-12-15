console.log("working...")

let btn = document.getElementById("btn")
btn.addEventListener("click", (e) => {
    e.preventDefault();
    let name = document.getElementById("name").value;    
    fetch(`hello_world.sh?name=${name}`, {
        method: 'UNKNOWN',
    })
        .then(response => response.text())
        .then(body => {
            document.body.innerHTML = body;
        })
        .catch(error => {
            console.error("Erreur réseau :", error);
        });
})
