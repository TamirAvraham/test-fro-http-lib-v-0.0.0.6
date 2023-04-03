// Get the dog ID from the URL
const urlParams = new URLSearchParams(window.location.search);
const dogId = urlParams.get('id');

// Get the dog data from the API
fetch(`/api/get_dog/${dogId}`)
    .then(response => response.json())
    .then(data => {
        // Update the dog detail page with the dog data
        const dogImage = document.querySelector('.dog-image');
        const dogName = document.querySelector('.dog-name');
        const dogBreed = document.querySelector('.dog-breed');
        const dogAge = document.querySelector('.dog-age');
        const dogGender = document.querySelector('.dog-gender');

        dogImage.src = data.path_to_picture;
        dogImage.alt = data.name;
        dogName.textContent = data.name;
        dogBreed.textContent = data.breed;
        dogAge.textContent = data.age;
        dogGender.textContent = data.gender === 'M' ? 'Male' : 'Female';
    })
    .catch(error => {
        console.error(error);
    });
