<?php

include_once 'config.php';

function connect_db()
{
  global $myserv;

  $myserv = mysql_connect("localhost",MYSQL_USER,MYSQL_PASSWD);
  mysql_select_db(MYSQL_DATABASE);
  mysql_set_charset('utf8');
}

function close_db()
{
  global $myserv;

  mysql_close($myserv);
}

?>
