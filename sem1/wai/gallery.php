<!DOCTYPE html>
<html>

<head>
    <title>Image Gallery</title>
    <style>
        .gallery {
            display: flex;
            flex-wrap: wrap;
            gap: 15px;
            justify-content: center;
        }

        #success {
            color: green;
        }

        #error {
            color: red;
        }
    </style>
</head>

<body>
    <form action="upload.php" method="post" enctype="multipart/form-data">
        Select image to upload: <br />
        <input type="file" name="fileToUpload" id="fileToUpload" required><br />
        <input type="text" name="watermarkText" placeholder="Watermark text" required><br />
        <input type="submit" value="Upload Image" name="submit" disabled>
        <script>
            document.getElementById('fileToUpload').addEventListener('change', function () {
                document.getElementsByName('submit')[0].disabled = false;
            });
        </script>
    </form>

    <div class="status">
        <?php
        if (isset($_GET['status'])) {
            $status = $_GET['status'];

            if ($status == 100) {
                echo '<span id="success">Success upload</span>';
            } elseif ($status == 101) {
                echo '<span id="error">File is not an image.</span>';
            } elseif ($status == 102) {
                echo '<span id="error">Sorry, file already exists.</span>';
            } elseif ($status == 103) {
                echo '<span id="error">Sorry, your file is too large.</span>';
            } elseif ($status == 104) {
                echo '<span id="error">Sorry, only JPG, PNG files are allowed.</span>';
            } elseif ($status == 105) {
                echo '<span id="error">Sorry, there was an error uploading your file.</span>';
            }
        }
        ?>
    </div>

    <div class="gallery">
        <?php
        $images = glob('images/*');
        $imageCount = count($images);
        foreach ($images as $image) {
            $thumbnail = 'thumbnails/' . basename($image);
            if (!file_exists($thumbnail)) {
                $thumbnailImage = imagecreatetruecolor(200, 125);
                if (exif_imagetype($image) == IMAGETYPE_PNG)
                    $sourceImage = imagecreatefrompng($image);
                else
                    $sourceImage = imagecreatefromjpeg($image);
                $sourceWidth = imagesx($sourceImage);
                $sourceHeight = imagesy($sourceImage);
                imagecopyresized($thumbnailImage, $sourceImage, 0, 0, 0, 0, 200, 125, $sourceWidth, $sourceHeight);
                if (exif_imagetype($image) == IMAGETYPE_PNG)
                    imagepng($thumbnailImage, $thumbnail);
                else
                    imagejpeg($thumbnailImage, $thumbnail);
                imagedestroy($thumbnailImage);
                imagedestroy($sourceImage);
            }
            echo '<a href="' . $image . '"><img src="' . $thumbnail . '" alt="Image"></a>';
        } ?>
    </div>
    <div class="image-count">
        <?php
        echo 'Total images: ' . $imageCount;
        ?>
</body>

</html>