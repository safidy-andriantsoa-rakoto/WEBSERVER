<?php
header('Content-Type: text/plain');
echo "=== PHP-CGI started ===\n";
echo "Request body:\n";
$body = file_get_contents('php://input');
echo $body;
echo "\n=== End ===\n";
?>
