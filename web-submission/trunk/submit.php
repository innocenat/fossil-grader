<?php

include_once 'config.php';
include_once 'db.php';
include_once 'util.php';

function getsubstatus($id, $probid)
{
  $res = mysql_query("SELECT * FROM grd_status WHERE user_id=\"$id\" AND ".
                     "prob_id=\"$probid\"");
  if(mysql_num_rows($res)==1) {
    $status = mysql_result($res,0,'res_id');
    settype($status,'integer');
    return $status;
  } else
    return SUBSTATUS_UNDEFINED;
}

function setsubstatus($id, $probid, $status)
{
  $res = mysql_query("SELECT * FROM grd_status WHERE user_id=\"$id\" AND ".
                     "prob_id=\"$probid\"");
  //  echo "HELLO";
  if(mysql_num_rows($res)==1) {
    $q = "UPDATE grd_status SET res_id=$status WHERE user_id=\"$id\" AND ".
      "prob_id=\"$probid\"";
  } else {
    $q = "INSERT INTO grd_status (user_id, prob_id, res_id) VALUES ".
      "(\"$id\",\"$probid\",$status)";
  }
  $res = mysql_query($q);
  if($res!=TRUE)
    echo "ERROR: " . mysql_error() . "<br>";
  return $res;
}

function putinqueue($id, $probid, $sub_num)
{
  $res = mysql_query("SELECT q_id FROM grd_queue WHERE user_id=\"$id\" AND ".
                     "prob_id=\"$probid\"");
  if(mysql_num_rows($res)==1) {
    $res = mysql_query("UPDATE grd_queue SET sub_num=$sub_num WHERE user_id=\"$id\" AND ".
                       "prob_id=\"$probid\"");
  } else {
    $res = mysql_query("INSERT INTO grd_queue (user_id, prob_id, sub_num) VALUES ".
                       "(\"$id\",\"$probid\",$sub_num)");
  }
  return $res;
}

function getsubcount($id,$probid)
{
  $query = mysql_query("select * from submission where user_id=\"$id\" and prob_id=\"$probid\"");
  return mysql_num_rows($query);
}

function builddate()
{
  return date("Y-m-d H:i:s");
}

function savesubmission($id, $probid, $filename)
{
  $content = file_get_contents($filename);
  $msg = NULL;
  
  mysql_query("LOCK TABLE submission WRITE, grd_queue WRITE, " .
              "grd_status WRITE");
  
  // savesubmission: savefile, set status, add submission to queue
  
  $status = getsubstatus($id, $probid);
  //  echo "status = " . $status . "<br>";
  if($status!=SUBSTATUS_GRADING) {
    // savefile
    $subcount = getsubcount($id, $probid);
    $timestamp = builddate();
    $query = "insert into submission (user_id,prob_id,sub_num,time,code) values " .
      "(\"$id\",\"$probid\"," . ($subcount+1) . ",\"$timestamp\", ".
      "\"" . mysql_real_escape_string($content) . "\");";
    //    echo htmlspecialchars($query);
    $res = mysql_query($query);
    if($res!=TRUE)
      $msg = "ERROR: Database problem (insertion error)";
    else {
      if(setsubstatus($id, $probid, SUBSTATUS_INQUEUE)!=TRUE)
        $msg = "ERROR: Database problem (grd_status)";
      else {
        if(putinqueue($id, $probid, $subcount+1)!=TRUE)
          $msg = "ERROR: Database problem (grd_queue)";
      }
    }
  } else {
    $msg = "ERROR: Grading old submission, please wait.";
  }
  mysql_query("UNLOCK TABLES");

  return $msg;
}

function processsubmission()
{
  $id = $_POST['id'];
  $fsize = $_FILES['code']['size'];
  //  echo 'id = ' . $id;
  if(($fsize>0) && ($fsize<=100000)) {
    connect_db();
    $res = savesubmission($_POST['id'], $_POST['probid'], 
                          $_FILES['code']['tmp_name']);
    close_db();
    if($res != NULL) {
      $_SESSION['msg']=$res;
    }
  } else {
    $_SESSION['msg']='ERROR: File too large';
  }
}

checkauthen();
processsubmission();

?>

<html>
<META HTTP-EQUIV="Refresh" CONTENT="0; URL=main.php">
</html>
