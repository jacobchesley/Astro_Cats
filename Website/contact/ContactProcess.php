<?php

session_start();
$name = $_POST['Name'];
$email = $_POST['Email'];
$subject = $_POST['Subject'];
$message = $_POST['Message'];
$comments = $_POST['comments'];

$mailheaders = "MIME-Version: 1.0\n";
$mailheaders .= "Content-type: text/plain; charset=iso-8859-1\n";
$mailheaders .= "From: $email\n";
$mailheaders .= "Reply-To:$email\n";
$mailheaders .= "Return-Path: $email\n";
$mailheaders .= "X-Mailer: PHP". phpversion() ."\n";

if($comments != ""){
	echo "thanks";
}

else if($name == "" || $email == "" ||  $subject == "" || $message == ""){
	$_SESSION["contactStatus"] = "blank";
	$_SESSION["name"] = $name;
	$_SESSION["email"] = $email;
	$_SESSION["subject"] = $subject;
	$_SESSION["message"] = $message;
	header("Location: http://www.ucrocketry.com/#Contact");
}

else{
	$to = "jacob.a.chesley@gmail.com";
	mail($to,"ucrocketry.com - " . $subject, "Name: " . $name . "\r\n" . "email address: " . $email . "\r\n" . "Subject: " . $subject . "\r\n" ."\r\n" . $message, $mailheaders);
	$_SESSION["contactStatus"] = "sent";
	header("Location: http://www.ucrocketry.com/#Contact");
}

?>