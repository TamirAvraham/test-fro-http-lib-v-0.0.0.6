function submitDog(event) {
    event.preventDefault();

    const nameInput = document.getElementById("name");
    const breedInput = document.getElementById("breed");
    const ageInput = document.getElementById("age");
    const genderSelect = document.getElementById("gender");

    const dog = {
        name: nameInput.value,
        breed: breedInput.value,
        age: parseInt(ageInput.value),
        gender: genderSelect.value,
        
    };

    fetch("/api/create", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(dog)
    })
        .then(response => {
            if (response.ok) {
                alert("Dog submitted successfully!");
                nameInput.value = "";
                breedInput.value = "";
                ageInput.value = "";
                genderSelect.value = "";
            } else {
                alert("Failed to submit dog.");
            }
        })
        .catch(error => {
            console.error(error);
            alert("Failed to submit dog.");
        });
}

const submitForm = document.querySelector("form");
submitForm.addEventListener("submit", submitDog);
