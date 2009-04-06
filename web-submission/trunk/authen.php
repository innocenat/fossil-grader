<?php
include_once 'db.php';

function checkpassword($id, $pass, &$user_type, &$user_group)
{
  $res = mysql_query("select * from user_info where user_id=\"$id\"");
  if(mysql_num_rows($res)==1) {
    if($pass==mysql_result($res,0,"passwd")) {
      $user_type = mysql_result($res,0,"type");
      $user_group = mysql_result($res,0,"grp");
      return TRUE;
    } else
      return FALSE;    
  } else {
    return FALSE;
  }
}

session_start();
$p_id = $_POST['id'];
$p_pass = $_POST['pass'];

connect_db();
$check = checkpassword($p_id,$p_pass,$user_type, $user_group);
close_db();

if($check) {
  $_SESSION['id']=$p_id;
  $_SESSION['type']=$user_type;
  $_SESSION['group']=$user_group;
  echo '<html>';
  echo '<META HTTP-EQUIV="Refresh" CONTENT="0; URL=main.php">';
  echo '</html>'; 
} else {
  session_destroy();
  echo '<html>';
  echo '<META HTTP-EQUIV="Refresh" CONTENT="0; URL=login.php?error=1">';
  echo '</html>';
}
?>

