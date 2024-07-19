<?php
$target_file = "images/" . basename($_FILES["fileToUpload"]["name"]);
$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));
$status = 100;

if (isset($_POST["submit"])) {
    $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
    if ($check === false) {
        $status = 101;
    }
}

if (file_exists($target_file)) {
    $status = 102;
}

if ($_FILES["fileToUpload"]["size"] > 1000000) {
    $status = 103;
}

if ($imageFileType != "jpg" && $imageFileType != "png") {
    $status = 104;
}

if ($status == 100) {

    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        copy($target_file, "originals/" . basename($_FILES["fileToUpload"]["name"]));
        if ($imageFileType == "png")
            $image = imagecreatefrompng($target_file);
        else
            $image = imagecreatefromjpeg($target_file);
        $watermarkText = $_POST["watermarkText"];
        $textColor = imagecolorallocate($image, 255, 0, 0);
        $font = "fonts/Oswald-Medium.ttf";
        $fontSize = 20;
        $x = imagesx($image) - strlen($watermarkText) * $fontSize - 1;
        $y = imagesy($image) - $fontSize - 1;
        imagettftext($image, $fontSize, 0, $x, $y, $textColor, $font, $watermarkText);
        if ($imageFileType == "png")
            imagepng($image, $target_file);
        else
            imagejpeg($image, $target_file);
        imagedestroy($image);

        // Connect to MongoDB
        $mongoClient = new MongoDB\Client("mongodb://localhost:27017");

        // Select database and collection
        $database = $mongoClient->selectDatabase("your_database_name");
        $collection = $database->selectCollection("your_collection_name");

        // Prepare the document to be inserted
        $document = [
            "file_name" => basename($_FILES["fileToUpload"]["name"]),
            "status" => $status,
            // Add more fields as needed
        ];

        // Insert the document into the collection
        $collection->insertOne($document);


    } else
        $status = 105;
}

$newReferer = parse_url($_SERVER['HTTP_REFERER'])['path'];

header("Location: " . $newReferer . "?status=$status");
?>