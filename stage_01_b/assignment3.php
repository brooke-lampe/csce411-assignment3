<?php
	$time_start = microtime(true); 
    $username = "mytestuser"; 
    $password = "123";   
    $host = "localhost";
    $database= "assignment3";
    
    $server = mysqli_connect($host, $username, $password);
    $connection = mysqli_select_db($server, $database);
    $myquery = $_GET["name"];
    $query = mysqli_query($server, $myquery);
    if ( ! $query ) {
        echo mysqli_error();
        die;
    }
  

    
    for ($x = 0; $x < mysqli_num_rows($query); $x++) {
        $data[] = mysqli_fetch_assoc($query);
    }
    /*for ($x = 0; $x < mysqli_num_rows($query); $x++) {
        echo json_encode($data[$x]);  
		echo nl2br("\n");
    }*/
     
     
    mysqli_close($server);
	
	$time_end = microtime(true);
	$execution_time = ($time_end - $time_start);
	echo '<b>Total Execution Time:</b> '.$execution_time.'seconds';
?>
