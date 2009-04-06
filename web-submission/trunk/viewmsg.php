<?php

include_once 'db.php';
include_once 'util.php';

function checkpermission($id)
{
  if($_SESSION['type']==USERTYPE_ADMIN) {
    return TRUE;
  } else if($_SESSION['type']==USERTYPE_SUPERVISOR) {
    $res = mysql_query("SELECT * FROM user_info WHERE user_id=\"$id\"");
    if((mysql_num_rows($res)==1) && 
       (mysql_result($res,0,'grp')==$_SESSION['group']))
      return TRUE;
    else
      return FALSE;
  } else
    return ($id==$_SESSION['id']);
}

$fileid = $_GET['id'];
$pid = $_GET['pid'];

checkauthen();
connect_db();
if(checkpermission($fileid)) {
  $res = mysql_query("SELECT * FROM grd_status WHERE user_id=\"$fileid\" ".
                     "AND prob_id=\"$pid\"");
  if(mysql_num_rows($res)!=1)
    echo "Compiler message is not available.";
  else {
    echo "<html><body>Compiler message of $fileid/$pid<hr><tt>";
    echo nl2br(htmlspecialchars(mysql_result($res,0,'compiler_msg')));
    echo "</body></html>";
  }
} else {
  echo "No permission!<br>";
  echo "This event has been logged:<br>";
  echo "<dd><tt> user " . $_SESSION['id'] . " is trying to access " . 
    $fileid . "-" . $pid . "</tt>";
}
close_db();

?>
