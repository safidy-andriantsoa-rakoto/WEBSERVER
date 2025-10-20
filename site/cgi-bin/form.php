<?php
// Set the content type to application/json so the response is clearly readable
header('Content-Type: application/json');

// Define a response array
$response = [
    'status' => 'error',
    'message' => 'No POST data received.',
    'data_received' => []
];

// 1. Check if the request method is POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    
    // Update status and message
    $response['status'] = 'success';
    $response['message'] = 'POST data successfully processed.';

    // 2. Access all POST data
    // The $_POST superglobal array contains all data sent with a POST request
    $all_post_data = $_POST;
    $response['data_received'] = $all_post_data;

    // 3. Process specific arguments (example)
    $processed_data = [];

    // Check for 'username'
    if (isset($all_post_data['username']) && !empty($all_post_data['username'])) {
        // Sanitize the input for security before use (important!)
        $username = htmlspecialchars(trim($all_post_data['username']));
        $processed_data['username'] = "Hello, $username! Your username has been recorded.";
    } else {
        $processed_data['username'] = "Username argument not provided or is empty.";
    }

    // Check for 'email'
    if (isset($all_post_data['email']) && filter_var($all_post_data['email'], FILTER_VALIDATE_EMAIL)) {
        // Sanitize the input
        $email = htmlspecialchars(trim($all_post_data['email']));
        $processed_data['email'] = "Your email ($email) is a valid format.";
    } else {
        $processed_data['email'] = "Email argument not provided or is invalid.";
    }

    // Add processed data to the response
    $response['processed_results'] = $processed_data;

    // Optional: Check for any other arguments
    if (count($all_post_data) > 0) {
        $response['count_of_arguments'] = count($all_post_data);
    }

} else {
    // If it's not a POST request
    $response['message'] = 'This endpoint only accepts POST requests.';
}

// 4. Output the final JSON response
echo json_encode($response, JSON_PRETTY_PRINT);
?>