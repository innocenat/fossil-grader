<?php

include_once '../config.php';
include_once '../util.php';
include_once '../db.php';

function ljust($st, $w)
{
  $ost = $st;
  for($i=0; $i<$w-strlen($st); $i++)
    $ost = $ost . "&nbsp;";
  return $ost;
}

function rjust($st, $w)
{
  $ost = "";
  for($i=0; $i<$w-strlen($st); $i++)
    $ost = $ost . "&nbsp;";
  $ost = $ost . $st;
  return $ost;
}

function getuserlist()
{
  global $user_list;

  $res = mysql_query("SELECT user_id FROM user_info WHERE (type='C') or (type='A') ORDER BY grp, user_id");
  $row=mysql_num_rows($res);
  for($i=0; $i<$row; $i++) {
    $user_list[$i] = mysql_result($res,$i,'user_id');
  }
}

function getproblist()
{
  global $prob_list;

  $res = mysql_query("SELECT prob_id FROM prob_info WHERE (avail='Y')");
  $row=mysql_num_rows($res);
  for($i=0; $i<$row; $i++) {
    $prob_list[$i] = mysql_result($res,$i,'prob_id');
  }
}

function gengrd_result($uid, $pid, &$score)
{
  $res = mysql_query("SELECT score, grading_msg FROM grd_status " .
                     "WHERE (user_id='$uid') and (prob_id='$pid')");
  $row=mysql_num_rows($res);
  if($row!=0) {
    $score = mysql_result($res,0,'score');
    return 
      rjust(mysql_result($res,0,'score'),4) . "&nbsp;&nbsp;" . 
      mysql_result($res,0,'grading_msg') . "";
  } else {
    $score = 0;
    return "&nbsp;&nbsp;&nbsp;0&nbsp;&nbsp;N";
  }
}

function genreport_user($uid)
{
  global $prob_list;

  $totalscore = 0;

  echo "-------------------------------------------------------<br>\n";
  $res = mysql_query("SELECT user_id, name, grp FROM user_info " .
                     "WHERE (user_id='$uid')");
  $row=mysql_num_rows($res);
  echo "user: " . $uid . " : " . mysql_result($res,0,'name') . "<br>\n";
  echo "center: " . mysql_result($res,0,'grp') . "<br>\n";
//  echo "task&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;score&nbsp;&nbsp;grading result<br>\n";
//  echo "-------------------------------------------------------<br>\n";
  for($i=0; $i<count($prob_list); $i++) {
    $msg = gengrd_result($uid, $prob_list[$i], $score);
    $totalscore += $score;
    echo 
      ljust($prob_list[$i],10) . "  " . $msg . "<br>\n";
  }
//  echo "<br>\n";
  echo "TOTAL : &nbsp;&nbsp;&nbsp;" . rjust($totalscore,4) . "<br>\n";
//  echo "***<br>\n";
//  echo "============<br>\n";
}

function genreport()
{
  global $user_list, $prob_list;

  for($i=0; $i<count($user_list); $i++)
    genreport_user($user_list[$i]);
}

checkauthen();
if($_SESSION['type']!=USERTYPE_ADMIN) {
  echo 'You do not have the permission to access this script.';
  exit;
}
connect_db();
getuserlist();
getproblist();
?>

<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
</head>

<body>
<tt>
<?php genreport(); ?>
</tt>

</body>

</html>

<?php
close_db();
?>
