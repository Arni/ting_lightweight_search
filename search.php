<?php

require_once './Twig/lib/Twig/Autoloader.php';
require_once './includes/openplatform.inc';
Twig_Autoloader::register();

$loader = new Twig_Loader_Filesystem('./templates');
$twig = new Twig_Environment($loader, array(
    'cache' => './cache',
    'auto_reload' => true,
));

//$token = open_platform_get_token();
//file_put_contents('/var/www/drupalvm/drupal/debug/opentoken.txt', $token, FILE_APPEND);
$startTime = explode(' ', microtime());
$results = open_platform_search('pluto');
        $stopTime = explode(' ', microtime());
        $time = floatval(($stopTime[1]+$stopTime[0]) - ($startTime[1]+$startTime[0]));
file_put_contents("/var/www/drupalvm/drupal/debug/opensearch2.txt", print_r($time  , TRUE), FILE_APPEND);

$template = $twig->load('search.html.twig');
echo $template->render();