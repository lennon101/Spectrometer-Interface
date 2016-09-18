<?php
include '/home/pi/dev-kit/php/_getspectrum.php';

//get the date/time
if (isset($_GET['datetime'])){
	$datetime = 'datetime';
} else {
	$datetime = "could not be found";
}

echo ("todays datetime: $datetime \n");

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
$myfile = fopen("testfile2.txt", "w");

//write the date/time:
//fwrite($myfile, "$today[3] / $today[5] / $today[6] ; $today[2] : $today[1]");

for ($i = 0; $i < $lengthOfArray; $i++) {
    fwrite($myfile, "$array0[$i] "."$array1[$i]"."\n");
}

?>