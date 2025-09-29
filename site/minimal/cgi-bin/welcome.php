<?php
// Simple GET test
if (isset($_GET['username']) && $_GET['username'] !== '') {
    $name = htmlspecialchars($_GET['username'], ENT_QUOTES, 'UTF-8');
    echo "<!DOCTYPE html>
<html lang='en'>
<head>
  <meta charset='UTF-8'>
  <title>Welcome</title>
</head>
<body>
  <h1>Welcome, $name!</h1>
  <p><a href='index.html'>Back to home</a></p>
</body>
</html>";
} else {
    echo "No name provided. <a href='index.html'>Try again</a>.";
}
?>