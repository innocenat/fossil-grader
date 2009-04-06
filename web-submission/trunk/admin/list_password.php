<?php

include_once '../config.php';
include_once '../util.php';
include_once '../db.php';

function listuser()
{
  echo '<table border=1>';
  echo '<tr><td width="10%"><b>username</b></td>';
  echo '<td width="40%"><b>name</b></td><td><b>password</b></td></tr>';
  connect_db();
  $res = mysql_query("SELECT * FROM user_info WHERE NOT(type='A')");
  $row=mysql_num_rows($res);
  for($i=0; $i<$row; $i++) {
    echo '<tr><td>' . mysql_result($res,$i,'user_id') . '</td>';
    echo '<td>' . mysql_result($res,$i,'name') . '</td>';
    echo '<td>' . mysql_result($res,$i,'passwd') . "</td></tr>\n";
  }
  close_db();
  echo '</table>';
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

<?php listuser(); ?>

</body>

</html>
