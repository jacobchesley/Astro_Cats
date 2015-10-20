<?php
	session_start();
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#940000">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>UC Astro Cats - PIL</title>
    <link href="../ucrocketry.css" rel="stylesheet" type="text/css" />
    <link href="../rocketpiledu.css" rel="stylesheet" type="text/css" />
    <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.8.2/jquery.min.js"></script>
</head>
<body class="preload"> 
    <ul class="Nav">
        <div class="NavFloatLeft">
            <li><a class="TopLeftTitle" href="../#Astrocats">UC Astro Cats</a></li>
        </div>
        <div class="NavFloatRight">
            <li><a class="NavButton" href="#PIL">The PIL</a></li>
            <li><a class="NavButton" href="#Diagram">Diagram</a></li>
            <li><a class="NavButton" href="#Components">Components</a></li>
        </div>
    </ul>
    
    <a name="Rocket" id="Rocket" class="Anchor"></a>
    <div class="AboutBlock">
        <div class="AboutTitle">
            The Planetary Investigation Lander (PIL)
        </div>
    </div>
    
    <a name="Diagram" id="Diagram" class="Anchor"></a>
    <div class="DiagramBlock">
        <div class="ComponentsTitle">
            PIL Diagram
        </div>
    </div>
    
    <a name="Components" id="Components" class="Anchor"></a>
    <div class="ComponentsBlock">
        <div class="ComponentsTitle">
            PIL Components List
        </div>
    </div>
    
    <div class="FooterBlock">
        <a href="http://www.twitter.com/UCAstrocats">
            <img class="SocialMediaLogo" src="../images/icons/TwitterLogo.png"/>
        </a>
        <a href="http://www.facebook.com/UCAstrocats">
            <img class="SocialMediaLogo" src="../images/icons/FacebookLogo.png"/>
        </a>
        <div class="Copyright">
            Copyright 2015.  University of Cincinnati AstroCats.  All rights reserved.
        </div>
    </div>
</body>
</html>

<script type="text/javascript">
	$(".NavFloatRight").addClass("NavFloatRight").before('<img class="ThreeBar" src="../images/icons/menu.png"></img>');
	$(".ThreeBar").click(function(){
		$(".NavFloatRight").toggle();
	});
    $(window).resize(function(){
		if(window.innerWidth > 768) {
			$(".NavFloatRight").removeAttr("style");
		}
	});
</script>
<script>
$('a').click(function(){
    $('html, body').animate({
        scrollTop: $( $(this).attr('href') ).offset().top
    }, 1200);
    return false;
});
</script>
