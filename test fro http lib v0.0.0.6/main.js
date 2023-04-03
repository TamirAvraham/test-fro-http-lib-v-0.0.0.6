// Fetch the dog data from the API
fetch('/api/get_all_dogs')
    .then(response => response.json())
    .then(data => {
        // Loop through each dog and create a card to display it
        const dogsContainer = document.getElementById('dogs-container');
        data.dogs.forEach(dog => {
            const dogCard = createDogCard(dog);
            dogsContainer.appendChild(dogCard);
        });
    })
    .catch(error => {
        console.error('Error fetching dog data:', error);
    });

// Helper function to create a dog card
function createDogCard(dog) {
    const dogCard = document.createElement('div');
    dogCard.className = 'dog-card';

    const dogImage = document.createElement('img');
    dogImage.src = dog.path_to_picture || 'placeholder.png'; // Use a placeholder image if no picture available
    dogCard.appendChild(dogImage);

    const dogName = document.createElement('h3');
    dogName.textContent = dog.name;
    dogCard.appendChild(dogName);

    const dogInfo = document.createElement('p');
    dogInfo.textContent = `Breed: ${dog.bread}, Age: ${dog.age}, Gender: ${dog.gender}`;
    dogCard.appendChild(dogInfo);

    // Add an event listener to navigate to the dog details page when the card is clicked
    dogCard.addEventListener('click', () => {
        window.location.href = `/dog_details/${dog.id}`;
    });

    return dogCard;
}
