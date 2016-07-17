<?php
include '/home/pi/dev-kit/php/_getspectrum.php';

//echo getspectrum(0);

//get string of spectrum returned from the supplied php file
$spectrum0 = getspectrum(0);
$spectrum1 = getspectrum(1);

//separate the string based on the white space 
$array0 = explode(" ",$spectrum0);
$array1 = explode(" ",$spectrum1);

//place a newline at end of each array item
//foreach ($array as $item){
//	echo "$item"."\n"; 
//}

$lengthOfArray = sizeof($array0);

for ($i = 0; $i < $lengthOfArray; $i++) {
    echo "$array0[$i] "."$array1[$i]"."\n";
}

//write the spectrum into a text file
$myfile = fopen("testfile.txt", "w");

for ($i = 0; $i < $lengthOfArray; $i++) {
    fwrite($myfile, "$array0[$i] "."$array1[$i]"."\n");
}

?>