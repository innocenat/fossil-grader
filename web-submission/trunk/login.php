<?php
session_start();
session_destroy();
?>
<html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=TIS-620"/>
</head>

<body OnLoad="document.form.id.focus();">
<br><br><br><br><br>
<form method="post" action="authen.php" name="form">

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


<div style="float: left; text-align: right; padding-right: 5px; width: 45%">
<img src="picture/logoposn.jpg"><br/><img src="picture/logoposn3.jpg">
</div>
<div style="float: right; text-align: left; padding-left: 5px; width: 53%">
<br>
<br>
<b>username : </b><input type="text" name="id"/><br/>
<b>password : </b><input type="password" name="pass"/><br/>
<input type="submit" name="okay" value="login"/>
</div>

<!---
<br><br><br>
<center>
<table width=500>
<tr>
  <td align="center" width="168"><img src="picture/logoposn.jpg"><br><img src="picture/logoposn3.jpg"></td>	
  <td><table width=100%><tr>
  <td align="right" width="30%">username :</td><td align="left"><input type="text" name="id"></td></tr><tr>
  <td align="right" width="30%">password :</td><td align="left"><input type="password" name="pass"></td></tr>
  </table></td>
</tr>

<tr><td colspan="2" align="center">
<input type="submit" name="okay" value="login">
</td></tr>
</table>
-->
</form>

<h1><a href="download">Download software</a></h1>
</body>
</html>
