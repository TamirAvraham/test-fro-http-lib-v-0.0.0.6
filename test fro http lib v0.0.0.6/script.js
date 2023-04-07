const urlParams = new URLSearchParams(window.location.search);
const dogId = window.location.href.substring(window.location.href.lastIndexOf('/') + 1);

fetch(`http://localhost:8080/api/get_dog/${dogId}`)
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(dog => {
        console.log(`dog is:${dog}`)
        // Display dog details on the page
        const dogNameElement = document.getElementById('dog-name');
        if (dogNameElement) {
            dogNameElement.textContent = dog.name;
            console.log(`got to name: ${dog.name}`);
        }

        const dogBreedElement = document.getElementById('dog-breed');
        if (dogBreedElement) {
            dogBreedElement.textContent = dog.bread;
            console.log(`got to bread:${dog.bread}`);
        }

        const dogAgeElement = document.getElementById('dog-age');
        if (dogAgeElement) {
            dogAgeElement.textContent = dog.age;
            console.log(`got to age :${dog.age}`);
        }

        const dogGenderElement = document.getElementById('dog-gender');
        if (dogGenderElement) {
            dogGenderElement.textContent = dog.gender;
            console.log(`got to gender :${dog.gender}`);
        }

        const dogImageElement = document.getElementById('dog-image');
        if (dogImageElement) {
            dogImageElement.setAttribute('src', dog.path_to_picture || 'default_dog_image.jpg');
            console.log(`got to image:`, dog.path_to_picture);
        }
    })
    .catch(error => {
        console.error('Error:', error);
        alert('An error occurred while fetching dog details');
    });
const submitButton = document.getElementById("adopt_button");
submitButton.onclick = (mouseEvent) => {
    fetch(`http://localhost:8080/api/adopt_dog/${dogId}`, {
        method:"DELETE"
    }).then(response => {
        const dogName = document.getElementById('dog-name').innerText;
        if (response.ok) {
            alert(`${dogName} adopted successfuly`);
            window.location.href('/');
        } else {
            alert(`error adopting dog`);
        }
    })
};