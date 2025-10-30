<?php

$counter = 0;
$countMax = 40;

// echo "Starting infinite loop...\n";
while (true) {
    $counter++;

    // echo "Iteration: " . $counter . ". Still running...\n";
    sleep(1);
    if ($counter >= $countMax) {
        // echo "Safety break triggered. Exiting.\n";
        break;
    }
}
?>