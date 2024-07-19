<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Retrieve the login form data
    $username = $_POST['username'];
    $password = $_POST['password'];

    if ($username === 'admin') {
        if ($password === 'password') {
            header('Location: dashboard.php');
            exit;
        } else {
            header('Location: login.php?error=passw');
            exit;
        }
    } else {
        header('Location: login.php?error=uname');
        exit;
    }
} elseif ($_GET['error'] === 'uname') {
    echo "Invalid username. Please try again.";
} elseif ($_GET['error'] === 'passw') {
    echo "Invalid password. Please try again.";
}
?>