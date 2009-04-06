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

function outputfile($content,$filename)
{
  header("Content-Type: application/force-download");
  header("Content-Type: text/plain");
  header("Content-Type: application/download");
  header("Content-Disposition: attachment; filename=".$filename.";");
  echo $content;
}

$fileid = $_GET['id'];
$pid = $_GET['pid'];
$num = $_GET['num'];

checkauthen();
connect_db();
if(checkpermission($fileid)) {
  $res = mysql_query("SELECT * FROM submission WHERE user_id=\"$fileid\" ".
                     "AND prob_id=\"$pid\" AND sub_num=$num");
  if(mysql_num_rows($res)!=1)
    echo "No such file";
  else {
    outputfile(mysql_result($res,0,'code'),$fileid . "-" . $pid . ".cpp");
  }
} else {
  echo "No permission!<br>";
  echo "This event has been logged:<br>";
  echo "<dd><tt> user " . $_SESSION['id'] . " is trying to access " . 
    $fileid . "-" . $pid . "</tt>";
}
close_db();

?>
