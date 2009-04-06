<?php

include_once '../config.php';
include_once '../util.php';
include_once '../db.php';

function randpass()
{
  $len = 7;

  $a = "abcdefghijklmnopqrstuvwxyz";
  $s = '';
  for($l=0; $l<$len; $l++) {
    $s = $s . $a{rand()%26};
  }
  return $s;
}

function listuser()
{
  echo '<b>these are old passwords</b>';
  echo '<table border=1>';
  echo '<tr><td width="10%"><b>username</b></td>';
  echo '<td width="40%"><b>name</b></td><td><b>password</b></td></tr>';
  connect_db();
//  $res = mysql_query("SELECT * FROM user_info WHERE NOT(type='A')");
  $res = mysql_query("SELECT * FROM user_info WHERE (type='C')");
  $row=mysql_num_rows($res);
  for($i=0; $i<$row; $i++) {
    echo '<tr><td>' . mysql_result($res,$i,'user_id') . '</td>';
    echo '<td>' . mysql_result($res,$i,'name') . '</td>';
    echo '<td>' . mysql_result($res,$i,'passwd') . "</td></tr>\n";
  }
  close_db();
  echo '</table>';
  $key = randpass();
  echo 'Do you <b><a href="random_user_password.php?genpassword=' . $key .
       '">want</a></b> to do change them?';
  $_SESSION['genkey'] = $key;
}

function setpassword($user_id, $passwd)
{
  mysql_query("UPDATE user_info SET passwd='$passwd' ". 
              "WHERE user_id='$user_id'");
}

function genpassword()
{
  echo "new password list...<br>\n";
  connect_db();
//  $res = mysql_query("SELECT * FROM user_info WHERE NOT(type='A')");
  $res = mysql_query("SELECT * FROM user_info WHERE (type='C')");
  $row=mysql_num_rows($res);
  for($i=0; $i<$row; $i++) {
    echo mysql_result($res,$i,'user_id') . ':';
    echo mysql_result($res,$i,'name') . ':';
    $pass = randpass();
    echo $pass . "<br>\n";
    setpassword(mysql_result($res,$i,'user_id'),$pass);
  }
  close_db();
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

<?php 
if(!isset($_GET['genpassword']))
  listuser();
else if((!isset($_SESSION['genkey'])) || 
        (!isset($_GET['genpassword'])) ||
        ($_SESSION['genkey']!=$_GET['genpassword'])) {
  echo 'cannot refress on this page<br>';
  listuser();
} else {
  unset($_SESSION['genkey']);
  genpassword();
}
?>

<br><br><b>Back to <a href="../main.php">main.</a></b>

</body>

</html>
