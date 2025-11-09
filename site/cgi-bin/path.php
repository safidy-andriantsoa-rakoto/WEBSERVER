<?php
header('Content-Type: text/plain');

echo "--- CGI PATH_INFO Test Results ---\n";
echo "1. SCRIPT_NAME: " . ($_SERVER['SCRIPT_NAME'] ?? 'Not Set') . "\n";
echo "2. PATH_INFO:   " . ($_SERVER['PATH_INFO'] ?? 'Not Set') . "\n";
echo "3. REQUEST_URI: " . ($_SERVER['REQUEST_URI'] ?? 'Not Set') . "\n";
echo "4. PATH_TRANSLATED: " . ($_SERVER['PATH_TRANSLATED'] ?? 'Not Set') . "\n";

if (isset($_SERVER['PATH_INFO']) && $_SERVER['PATH_INFO'] !== '') {
    echo "\n✅ PATH_INFO was successfully captured.";
} else {
    echo "\n❌ PATH_INFO was NOT captured. Check server configuration.";
}
?>