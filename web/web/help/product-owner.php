<?php
require_once ("../bootstrap/bootstrap.php");
page_access_level (GUEST_LEVEL);
Assets_Page::header (gettext ("Product Owner"));
$smarty = new Smarty_Bibledit (__FILE__);
$smarty->display("product-owner.tpl");
Assets_Page::footer ();
?>
