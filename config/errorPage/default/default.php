<?php
// Set default values in case arguments are not provided.
$statusCode = 500;
$statusText = 'Internal Server Error';

// $argv is an array containing the command-line arguments.
// $argv[0] is the script name.
// $argv[1] is the first argument (status code).
// $argv[2] is the second argument (status phrase).
if (isset($argv[1])) {
    // Ensure the status code is an integer.
    $statusCode = (int)$argv[1];
}
if (isset($argv[2])) {
    // Collect all arguments from the second one onwards to allow phrases with spaces.
    $statusText = implode(' ', array_slice($argv, 2));
}

// 1. Send HTTP Headers
// echo "Status: {$statusCode} {$statusText}\r\n";
// echo "Content-Type: text/html; charset=UTF-8\r\n";
// echo "\r\n"; // This blank line is crucial! It separates headers from the body.

// Prepare variables for safe HTML display.
$safeStatusCode = htmlspecialchars($statusCode, ENT_QUOTES, 'UTF-8');
$safeStatusText = htmlspecialchars($statusText, ENT_QUOTES, 'UTF-8');

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
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>{$safeStatusCode}</h1>
        <p>{$safeStatusText}</p>
        <hr>
        <small>We apologize for the inconvenience. Please try again later.</small>
    </div>
</body>
</html>
HTML;

?>