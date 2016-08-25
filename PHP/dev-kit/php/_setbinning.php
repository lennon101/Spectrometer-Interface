<?php
include_once 'common.php';
function setbinning($bin, $channel = 0) {
	global $ip_address, $port, $cmd_set_pixel_binning_factor;

	$socket = connect_or_abort($ip_address, $port);

    $arguments = $channel . ";" . $bin;
    $msg = $cmd_set_pixel_binning_factor . string_length_bytes($arguments);
    $msg = $msg . $arguments;

    socket_write_all($socket, $msg);

	$response_string = socket_read_all($socket);
        socket_close($socket);

	$result_code = extract_result_code_from_response($response_string);

	return $result_code;
}
?>
