
<?php
error_reporting(0);
session_start();
  if ($_SESSION['login']===true){ //ak je nastavena session, som prihlaseny
  ?>
<!DOCTYPE html>
<html>
<head>
<title>Systém FarmLab</title>
</head>
<body>
  <body>
  </body>
</html>
<?php }else{
	header('LOCATION: ../index.php'); //ak nie som prihlaseny, vrat ma na login
	
}
?>
