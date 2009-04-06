<?php

define('SOURCE_DOWNLOAD',1);

define('SHOW_COMPILER_MSG',1);

//uncomment to add priting support
//define('PRINTERNAME','\\\\192.168.0.1\\hpLaserJ');

//uncomment to change to analysis mode
//define('ANALYSIS_MODE',1);

define("OUTPUT_DIR","output_dir");

//this is for giving out students' outputs
function getoutputfname($user_id,$prob_id)
{
  return OUTPUT_DIR."/".$user_id."-".$prob_id."-out.zip";
}

//for MySQL
define("MYSQL_USER","your_user_name_here");
define("MYSQL_PASSWD","your_password_herer");
define("MYSQL_DATABASE","your_database_name_here");

//submission status
define("SUBSTATUS_UNDEFINED",0);
define("SUBSTATUS_INQUEUE",1);
define("SUBSTATUS_GRADING",2);
define("SUBSTATUS_ACCEPTED",3);
define("SUBSTATUS_REJECTED",4);

//user types
define("USERTYPE_ADMIN",'A');
define("USERTYPE_SUPERVISOR",'S');
define("USERTYPE_CONTESTANT",'C');


?>