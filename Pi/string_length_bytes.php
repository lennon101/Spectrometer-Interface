<?php 
function string_length_bytes($s) {

	//get the length in integer value
	$l = strlen($s);
	echo "length is: $l\n";

	//l is a 16 bit integer
	$hi = $l & 0xFF00; //1111111100000000
	echo "high value is: $hi\n";

	$hi = $hi >> 8; //shift the bits in hi 8 to the right
	$lo = $l & 0xFF;
	echo "low value is: $lo\n";

	//convert hi and lo to ASCII equivalent and concatenate
	$result = chr($hi) . chr($lo);

	return $result;
}

$s = "500000" . ";" . 0;
print "string is: $s\n";
$num = string_length_bytes($s);
echo $num;

?> 