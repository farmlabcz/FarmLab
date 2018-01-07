<!DOCTYPE html>

<html><head><meta http-equiv="Content-Type" charset=UTF-8">
    <title>Smart Home</title>
    <meta name="robots" content="noindex, nofollow">
    <meta name="viewport" content="user-scalable=0, width=device-width, initial-scale=1.0, maximum-scale=1.0">
    <link rel="icon" type="image/png" href="icon.png" />
    <link rel="stylesheet" href="CSS/misc.css">
	<link id="pre-load-login-css" rel="stylesheet" href="CSS/loader.css">
    <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">
		
</head>
<body class="light-mode">
  <div class="login-container">
    <div class="login-container__login-bg">
    </div>
      <div class="login-container__logo-container">
      </div>
        <div class="login-container__login-window">
          <div class="login-window__icon-container">
            <img class="icon-container__img" src="icon.png">
          </div>
<?php    
      error_reporting(0);
session_start();
	
	if ($_SESSION['login']===true){?>
<form class="login-window__login-form">
  <center>
		<h2><font color="red">Používateľ stále prihlásený!</font></h2>
		<a href="system/#/route/dashboard/home" class="btn btn-success">Späť do administrácie</a>
		<br><br>
		<a href="system/logout.php" class="btn btn-danger">Odhlásiť sa z administrácie</a>
       
        </form>
		 
	
	  </center>
		<?php
		
		
	}else{
	error_reporting(0);
	session_start();
      if(isset($_POST['odoslat'])){
       $username = $_POST['username'];
    $username = trim( $username );
    $username = htmlspecialchars( $username, ENT_QUOTES );
    $password = $_POST['password']; 
    $password = trim( $password );
    $password = htmlspecialchars( $password, ENT_QUOTES );  
        if($username == "farmlab" && $password == "farmlab"){
          $_SESSION['login'] = true; 
		  header('LOCATION: /system'); die();
        } else{
           echo "Údaje nie sú správne! Váš pokus bol uložený do logu!";
   $ip = "";

if (!empty($_SERVER["HTTP_CLIENT_IP"]))
{
 //check for ip from share internet
 $ip = $_SERVER["HTTP_CLIENT_IP"];
}
elseif (!empty($_SERVER["HTTP_X_FORWARDED_FOR"]))
{
 // Check for the Proxy User                    
 $ip = $_SERVER["HTTP_X_FORWARDED_FOR"];
}
else
{
 $ip = $_SERVER["REMOTE_ADDR"];
}
$t = file_get_contents("logy/neuspesneprihlasenia.txt");
      $today = date("Y-m-d H:i:s");
     
       $t .= $today." Pouzivatel z ip ".$ip." sa neprihlasil, zadal zle prihlasovacie informacie, meno: ".$username." a heslo: ".$password."\r\n";
      file_put_contents("logy/neuspesneprihlasenia.txt",$t);
    }
        }

	
	?>
	<form class="login-window__login-form" method="post" action="<?php echo $_SERVER['PHP_SELF']; ?>">
        <div class="login-form__title">Webové rozhranie domácnosti</div>
        <br>
        <input id="username" name="username" placeholder="Užívateľské meno" type="text" class="login-form__username" autocorrect="off" autocapitalize="off" spellcheck="false" required>
        <input id="password" name="password" placeholder="Heslo" type="password" class="login-form__password" required>
        
        
        <br>
        <button type="submit" name="odoslat" class="login-form__connect-button">PRIHLÁSIŤ</button>
       
        </form>
	<?php  }  ?>
   <div class="login-window__loader-container hidden">
            <div class="loader-container__spinner"></div>
            <div class="loader-container__text-container">
                <div class="text-container__title"></div>
                <div class="text-container__subtitle"></div>
            </div>
        </div>
    </div>
</body></html>
