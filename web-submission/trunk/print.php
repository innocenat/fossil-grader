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

//  $cmd = "print /d:" . PRINTERNAME . " $tmpfname";
//  exec($cmd);
$filename = $tmpfname;
$handle = fopen($filename, "r");
$contents = fread($handle, filesize($filename));
fclose($handle);
$handle = printer_open(PRINTERNAME);

if($handle==FALSE)
	echo "Can't open printer<br/>";

printer_set_option($handle, PRINTER_MODE, "raw");

//if(printer_write($handle, "Hello!")==FALSE)
//	echo "Print error<br/>";

printer_set_option($handle, PRINTER_PAPER_FORMAT, PRINTER_FORMAT_A4);
printer_start_doc($handle,"source code");
if(printer_write($handle, $contents)==FALSE)
	echo "Can't print<br/>";
printer_end_doc($handle);

/*
printer_start_doc($handle,"Source Code");
printer_start_page($handle);
printer_draw_text($handle,"Hello world",10,10);
printer_end_page($handle);
printer_end_doc($handle);
*/

//if(printer_write($handle, $contents)==FALSE)
//	echo "Can't print<br/>";

/*


printer_start_doc($handle,"Source Code");
printer_start_page($handle);

printer_draw_line($handle,10,10,1000,1000);
printer_end_page($handle);
printer_end_doc($handle);
*/

printer_close($handle);


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
