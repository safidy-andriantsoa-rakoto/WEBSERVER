<?php
// Define constants
const LOG_FILE = 'registered_names.txt';
const UPLOAD_DIR = 'uploads/'; // Directory where files will be stored

// NOTE: All header() and http_response_code() calls have been removed per your request.
// This script now focuses ONLY on generating the response body (HTML or JSON).

// Check the request method
$method = $_SERVER['REQUEST_METHOD'];

switch ($method) {
    case 'GET':
        // ---------------------------------------------------------------------
        // 1. HANDLE GET REQUEST (Welcome Page Body - HTML)
        // ---------------------------------------------------------------------

        $userName = filter_input(INPUT_GET, 'user_name', FILTER_SANITIZE_STRING);
        if (!$userName) {
            $userName = "Guest";
        }
        
        // Output the HTML body. (The Content-Type: text/html header must be set externally.)
        echo "<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset='UTF-8'>
    <title>Welcome</title>
    <style>
        body { font-family: 'Inter', sans-serif; background-color: #f0f4f8; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }
        .card { background: white; padding: 40px; border-radius: 12px; box-shadow: 0 10px 30px rgba(0, 0, 0, 0.1); text-align: center; }
        h1 { color: #2980b9; margin-bottom: 10px; }
        p { color: #333; }
        a { color: #2980b9; text-decoration: none; font-weight: bold; }
        a:hover { text-decoration: underline; }
    </style>
</head>
<body>
    <div class='card'>
        <h1>Welcome Back, " . htmlspecialchars($userName) . "!</h1>
        <p>This is the HTML body for a successful **GET** request.</p>
        <p>Headers (like Content-Type: text/html) must be set externally.</p>
        <p><a href='javascript:history.back()'>Go back to the test page</a></p>
    </div>
</body>
</html>";
        break;

    case 'POST':
        // ---------------------------------------------------------------------
        // 2. HANDLE POST REQUEST (Name Register OR File Upload Body - JSON)
        // ---------------------------------------------------------------------

        $response = ['status' => 'error', 'message' => 'Invalid POST request.', 'received_data' => []];
        // HTTP Status and Content-Type: application/json must be set externally.

        // --- File Upload Handling ---
        if (!empty($_FILES['upload_file'])) {
            // Ensure the upload directory exists
            if (!is_dir(UPLOAD_DIR) && !mkdir(UPLOAD_DIR, 0777, true)) {
                $response['message'] = "Error: Failed to create upload directory.";
            } else {
                $file = $_FILES['upload_file'];
                $fileName = basename($file['name']);
                $targetFile = UPLOAD_DIR . $fileName;
                
                if ($file['error'] === UPLOAD_ERR_OK) {
                    if (move_uploaded_file($file['tmp_name'], $targetFile)) {
                        $response['status'] = 'success';
                        $response['message'] = "File uploaded successfully.";
                        $response['received_data'] = ['filename' => $fileName, 'size' => $file['size']];
                        // Suggested external status: 200/201
                    } else {
                        $response['message'] = "Error: Failed to move uploaded file.";
                    }
                } else {
                    $response['message'] = "Error during file upload: " . $file['error'];
                }
            }

        // --- Name Registration Handling ---
        } elseif (!empty($_POST['user_name'])) {
            $userName = filter_var($_POST['user_name'], FILTER_SANITIZE_STRING);

            if (!is_writable(dirname(LOG_FILE)) || (file_exists(LOG_FILE) && !is_writable(LOG_FILE))) {
                 $response['message'] = "Error: Log file is not writable. Check server permissions.";
            } else {
                $names = file_exists(LOG_FILE) ? array_map('trim', file(LOG_FILE)) : [];
                
                if (in_array($userName, $names)) {
                    $response['status'] = 'info';
                    $response['message'] = "User '{$userName}' is already registered.";
                    // Suggested external status: 200
                } else {
                    if (file_put_contents(LOG_FILE, $userName . PHP_EOL, FILE_APPEND | LOCK_EX) !== false) {
                        $response['status'] = 'success';
                        $response['message'] = "User '{$userName}' registered successfully.";
                        // Suggested external status: 201
                    } else {
                        $response['message'] = "Error: Failed to write to log file.";
                    }
                }
            }
        }
        
        // Output the JSON body
        echo json_encode($response, JSON_PRETTY_PRINT);
        break;

    case 'DELETE':
        // ---------------------------------------------------------------------
        // 3. HANDLE DELETE REQUEST (Name Unregister OR File Delete Body - JSON)
        // ---------------------------------------------------------------------

        $response = ['status' => 'error', 'message' => 'No resource identifier provided for deletion.'];
        // HTTP Status and Content-Type: application/json must be set externally.
        
        // DELETE requests send parameters via the query string
        $userName = filter_input(INPUT_GET, 'user_name', FILTER_SANITIZE_STRING);
        $fileName = filter_input(INPUT_GET, 'file_name', FILTER_SANITIZE_STRING);

        // --- File Deletion Handling ---
        if ($fileName) {
            $targetFile = UPLOAD_DIR . basename($fileName); // Use basename for security
            
            if (file_exists($targetFile)) {
                if (unlink($targetFile)) {
                    $response['status'] = 'success';
                    $response['message'] = "File '{$fileName}' deleted successfully.";
                    // Suggested external status: 200
                } else {
                    $response['message'] = "Error: Failed to delete file '{$fileName}'. Check permissions.";
                }
            } else {
                $response['message'] = "Error: File '{$fileName}' not found.";
                // Suggested external status: 404
            }

        // --- Name Unregistration Handling ---
        } elseif ($userName) {
             if (!is_writable(LOG_FILE) && file_exists(LOG_FILE)) {
                 $response['message'] = "Error: Log file is not writable. Cannot unregister user.";
             } else {
                $names = file_exists(LOG_FILE) ? array_map('trim', file(LOG_FILE)) : [];
                $originalCount = count($names);
                
                // Filter out the user name
                $names = array_filter($names, function($name) use ($userName) {
                    return $name !== $userName;
                });
                
                if (count($names) < $originalCount) {
                    // User was found and removed, overwrite the log file
                    file_put_contents(LOG_FILE, implode(PHP_EOL, $names) . (empty($names) ? '' : PHP_EOL), LOCK_EX);
                    $response['status'] = 'success';
                    $response['message'] = "User '{$userName}' unregistered successfully.";
                    // Suggested external status: 200
                } else {
                    $response['message'] = "User '{$userName}' was not found in the log.";
                    // Suggested external status: 404
                }
             }
        }

        // Output the JSON body
        echo json_encode($response, JSON_PRETTY_PRINT);
        break;
}