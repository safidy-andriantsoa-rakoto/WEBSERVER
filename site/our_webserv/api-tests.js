// api-tests.js
// This file contains the asynchronous functions for handling DELETE requests
// that update the result boxes in index.html without navigating away.

// The endpoint for all requests
const endpoint = 'script.php'; 

// --- Helper function to handle fetch response and update result box ---
async function handleResponse(response, resultBox) {
    const contentType = response.headers.get("content-type");
    
    if (response.status === 204) {
         // No Content status
         resultBox.textContent = 'Status 204: No Content. Operation successful.';
    } else if (contentType && contentType.includes("application/json")) {
        // Handle JSON response body
        const data = await response.json();
        resultBox.textContent = JSON.stringify(data, null, 2);
    } else {
        // Handle non-JSON (e.g., plain text or HTML error) response
        const text = await response.text();
        resultBox.textContent = `Non-JSON Response (${response.status}):\n${text}`;
    }
}

// --- JavaScript for NAME DELETE request ---
async function sendDeleteNameRequest() {
    const userName = document.getElementById('deleteName').value;
    const resultBox = document.getElementById('deleteNameResult');

    resultBox.textContent = 'Sending DELETE request for ' + userName + '...';

    try {
        // Sending name in the URL query string for DELETE
        const response = await fetch(endpoint + '?user_name=' + encodeURIComponent(userName), {
            method: 'DELETE',
        });
        await handleResponse(response, resultBox);

    } catch (error) {
        resultBox.textContent = 'Request failed: ' + error.message;
    }
}


// --- JavaScript for FILE DELETE request ---
async function sendDeleteFileRequest() {
    const fileName = document.getElementById('deleteFilename').value;
    const resultBox = document.getElementById('deleteFileResult');

    resultBox.textContent = 'Sending DELETE request for file: ' + fileName + '...';

    try {
        // Sending filename in the URL query string for DELETE. PHP script should look for $_GET['file_name']
        const response = await fetch(endpoint + '?file_name=' + encodeURIComponent(fileName), {
            method: 'DELETE',
        });
        await handleResponse(response, resultBox);

    } catch (error) {
        resultBox.textContent = 'Request failed: ' + error.message;
    }
}
