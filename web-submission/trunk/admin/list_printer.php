<?php

include_once '../config.php';
include_once '../util.php';
include_once '../db.php';

function listprinter()
{
	$PrintDests = printer_list(PRINTER_ENUM_LOCAL);
	echo 'This is a list of printers that connect to the server.<br>';
	echo '<UL>';
	foreach ($PrintDests as $PrintDest)
      echo '<LI>'.$PrintDest["NAME"].'</LI>'; 
	echo '</UL>';  
	echo 'Use one of this list to set PRINTERNAME in config file.<br>';  
	echo 'Please install php_printer.dll to extension_dir and enable extension=php_printer.dll in php.ini before using print.';
}

checkauthen();
if($_SESSION['type']!=USERTYPE_ADMIN) {
  echo 'You do not have the permission to access this script.';
  exit;
}
?>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</head>

<body>
<?php listprinter(); ?><br>
Back to <a href="../main.php">main page</a>
</body>

</html>
