<?php

$counter = 0;

echo "Starting infinite loop...\n";
while (true) {
    $counter++;

    echo "Iteration: " . $counter . ". Still running...\n";
    sleep(1);
    if ($counter >= 20) {
        echo "Safety break triggered. Exiting.\n";
        break;
    }
}
?>