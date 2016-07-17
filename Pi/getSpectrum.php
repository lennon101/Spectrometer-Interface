<?php
include '_getspectrum.php';

//echo getspectrum(0);

$spectrum = getspectrum(0);

$array = explode(" ",$spectrum);

foreach ($array as $item){
	echo "$item"."\n"; 
}

$myfile = fopen("testfile.txt", "w");

foreach ($array as $item){
	fwrite($myfile, "$item"."\n");
}

?>