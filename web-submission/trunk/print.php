<?php
include_once 'config.php';
include_once 'db.php';
include_once 'util.php';

/*
$cmd = "print c:\\home\\temp\\test3.c";
echo $cmd;
if(exec($cmd)==FALSE) {
  echo "dead";
}
*/

function printfile($id,$fname)
{
  $tmpfname = tempnam("", "");

  //  echo $tmpfname;

  $fout = fopen($tmpfname, "w");

  fputs($fout,"***************************************************\r\n");
  fputs($fout,"*    USER: $id\r\n");
  fputs($fout,"***************************************************\r\n");
  fputs($fout,"\r\n");

  $line = 0;
  $fp = fopen($fname,'r');
  while (!feof($fp)) {
    $buffer = fgets($fp, 1000);
    $line++;
    $printstr = sprintf("%3d: %s\r",$line,$buffer);
    fputs($fout,$printstr);
  }
  fclose($fp);
  fclose($fout);

  $cmd = "print /d:" . PRINTERNAME . " $tmpfname";
  exec($cmd);

  unlink($tmpfname);

//  echo $cmd;
//  exec($cmd);
}

checkauthen();
$id=$_SESSION['id'];
$fsize = $_FILES['code']['size'];
if(($fsize>0) && ($fsize<10000)) {
  $fname = $_FILES['code']['tmp_name'];
  printfile($id,$fname);
  echo "your program has been printed<br>";
} else
  echo "file too big or too small<br>";

echo "<br><br>return to <a href=\"main.php\">main page</a>";
?>
