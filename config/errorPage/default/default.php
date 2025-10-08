<?php
// Set default values in case arguments are not provided.
$statusCode = 400;
$statusText = 'Bad Request';

// Get status code and phrase from environment variables (as suggested in the previous solution).
if (isset($_SERVER['STATUS_CODE'])) {
    $statusCode = (int)$_SERVER['STATUS_CODE'];
}

if (isset($_SERVER['STATUS_PHRASE'])) {
    $statusText = $_SERVER['STATUS_PHRASE'];
}

// Prepare variables for safe HTML display.
$safeStatusCode = htmlspecialchars($statusCode, ENT_QUOTES, 'UTF-8');
$safeStatusText = htmlspecialchars($statusText, ENT_QUOTES, 'UTF-8');

// 1. Send the HTTP status header
// (It's good practice to ensure the correct status code is sent to the client)
http_response_code($statusCode);

// 2. Send HTML Body using a HEREDOC for readability.
echo <<<HTML
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Error {$safeStatusCode}</title>
    <style>
        body {
            font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
            background-color: #f8f9fa;
            color: #343a40;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
            text-align: center;
        }
        .container {
            max-width: 600px;
            padding: 2rem;
        }
        h1 {
            font-size: 6rem;
            font-weight: 700;
            margin: 0;
            color: #dc3545; /* A standard error red color */
        }
        p {
            font-size: 1.5rem;
            margin-top: 0.5rem;
            color: #6c757d;
        }
        hr {
            border: 0;
            border-top: 1px solid #dee2e6;
            margin: 1.5rem 0;
        }
        small {
            color: #adb5bd;
            display: block; /* Make small text take its own line */
            margin-bottom: 1rem; /* Space before the link */
        }
        .home-link {
            display: inline-block;
            padding: 0.5rem 1rem;
            margin-top: 1rem;
            background-color: #007bff; /* A standard blue color */
            color: white;
            text-decoration: none;
            border-radius: 0.3rem;
            transition: background-color 0.3s;
        }
        .home-link:hover {
            background-color: #0056b3;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>{$safeStatusCode}</h1>
        <p>{$safeStatusText}</p>
        <hr>
        <small>We apologize for the inconvenience. Please try again later.</small>
        
        <a href="/" class="home-link">Go to Homepage</a>
        
    </div>
</body>
</html>
HTML;

?>