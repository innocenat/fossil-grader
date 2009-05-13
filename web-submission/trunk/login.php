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
<img src="images/fossil-logo.png"/>
</div>
<div style="float: right; text-align: left; padding-left: 5px; width: 53%">
<br>
<br>
<b>username : </b><input type="text" name="id"/><br/>
<b>password : </b><input type="password" name="pass"/><br/>
<input type="submit" name="okay" value="login"/>
</div>
</form>

</body>
</html>
