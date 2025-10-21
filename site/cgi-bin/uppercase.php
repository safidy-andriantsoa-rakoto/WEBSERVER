<?php
// Set the Content-Type header
header('Content-Type: text/plain');

// Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Read the raw POST data from the standard input stream (php://input)
    // This is where the server reconstructs the chunked body.
    $original_content = file_get_contents('php://input');
    
    // Convert the entire content to uppercase
    $uppercase_content = strtoupper($original_content);
    
    // Output the result
    echo $uppercase_content;
    
} else {
    // If not a POST request, return an error message
    http_response_code(405); // Method Not Allowed
    echo "Error: Only POST requests are accepted.";
}
?>