<?php
function checkauthen()
{
  session_start();
  if(!isset($_SESSION['id'])) {
    // no session
    session_destroy();
    echo '<html>You have not login, please ';
    echo '<a href="login.php">login</a><br>';
    exit();
  }
}

function getname($id)
{
  $res = mysql_query("SELECT name FROM user_info WHERE user_id=\"$id\"");
  if(mysql_num_rows($res)==1)
    return mysql_result($res,0,'name');
  else
    return '(none)';
}

?>