<?php
$target_path = "uploads/";

$target_path = $target_path . basename( $_FILES['fileToUpload']['name']); 

if(move_uploaded_file($_FILES['fileToUpload']['tmp_name'], $target_path)) {
    echo "The file ".  basename( $_FILES['fileToUpload']['name']). 
    " has been uploaded";
    ob_start();
    passthru('exefiletest');
    /*  assthru('exefiletest ' . escapeshellarg(['tmp_name'])); */
    $out = ob_get_contents();
    ob_end_clean();
    $arr = explode(PHP_EOL, $out);
    var_dump($arr);
    $out1 = $arr[0];
    $out2 = $arr[1];
    $out3 = $arr[2];
    $out4 = $arr[3];
    var_dump($out1, $out2, $out3, $out4);

} else{
    echo "There was an error uploading the file, please try again!";
}
?>
