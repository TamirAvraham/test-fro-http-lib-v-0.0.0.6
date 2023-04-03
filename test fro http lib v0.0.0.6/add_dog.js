// Submit the form data to the API
const submitForm = document.querySelector('form');
submitForm.addEventListener('submit', (event) => {
    event.preventDefault();

    const name = document.querySelector('#name').value;
    const breed = document.querySelector('#breed').value;
    const age = document.querySelector('#age').value;
    const gender = document.querySelector('#gender').value;

    const formData = new FormData();
    formData.append('name', name);
    formData.append('breed', breed);
    formData.append('age', age);
    formData.append('gender', gender);

    fetch('/api/create', {
        method: 'POST',
        body: formData
    })
        .then(response => response.json())
        .then(data => {
            // Redirect to the dog detail page for the new dog
            window.location.href = `/dog.html?id=${data.id}`;
        })
        .catch(error => {
            console.error(error);
        });
});
