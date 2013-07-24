<?php
require_once ("../bootstrap/bootstrap.php");
page_access_level (CONSULTANT_LEVEL);
$header = new Assets_Header (gettext ("Resources"));
$header->jQueryOn ();
$header->run ();
$view = new Assets_View (__FILE__);

$database_config_user = Database_Config_User::getInstance ();
$resources = $database_config_user->getActiveResources ();
$view->view->resources = $resources;

$resource_count = count ($resources);
$script = <<<EOD
var resourceCount = $resource_count;
EOD;
$view->view->script = $script;

$view->view->navigationHtml = Navigation_Logic::htmlNavigator ();
$view->view->navigationCode = Navigation_Logic::code ();

$view->render ("index.php");
Assets_Page::footer ();
?>
