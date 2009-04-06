<?php
session_start();
session_destroy();
?>
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=TIS-620"/>
</head>

<body>
<form method="post" action="authen.php">

<center><b>
<?php

if(isset($_GET['error'])) {
  $p_error = $_GET['error'];
  if($p_error==1) {
    echo 'wrong password, please login again<br><br>';
  }
}
?>
</b></center>

<table width="100%">
<tr><td align="right" width="50%">username</td>
<td><input type="text" name="id"></td></tr>
<tr><td align="right">password</td>
<td><input type="password" name="pass"></td></tr>
<tr><td colspan="2" align="center">
<input type="submit" name="okay" value="login">
</td></tr>
</table>

</form>
</body>
</html>
