<?php
define('DRUPAL_ROOT', $_GET['DRUPAL_ROOT']);
require_once (DRUPAL_ROOT . '/includes/bootstrap.inc');

require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
require_once DRUPAL_ROOT . '/includes/common.inc';
require_once DRUPAL_ROOT . '/includes/file.inc';
require_once DRUPAL_ROOT . '/includes/module.inc';
require_once DRUPAL_ROOT . '/includes/ajax.inc';

// We prepare only a minimal bootstrap. This includes the database and
// variables, however, so we have access to the class autoloader registry.
drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);

// This must go after drupal_bootstrap(), which unsets globals!
global $conf;

// We have to enable the user and system modules, even to check access and
// display errors via the maintenance theme.
$module_list['system']['filename'] = 'modules/system/system.module';
$module_list['user']['filename'] = 'modules/user/user.module';
module_list(TRUE, FALSE, FALSE, $module_list);
drupal_load('module', 'system');
drupal_load('module', 'user');

// We also want to have the language system available, but we do *NOT* want to
// actually call drupal_bootstrap(DRUPAL_BOOTSTRAP_LANGUAGE), since that would
// also force us through the DRUPAL_BOOTSTRAP_PAGE_HEADER phase, which loads
// all the modules, and that's exactly what we're trying to avoid.
drupal_language_initialize();

// Initialize the maintenance theme for this administrative script.
drupal_maintenance_theme();

$output = '';
$show_messages = TRUE;

require_once('lib/includes/includes.inc.php');
$sceaucontrol = new SceauControl();

/* * ***** <utilisateur> **************** */
$sceaucontrol->createCustomer('', 1, $_GET['lastname'], $_GET['firstname'],$_GET['email']);
/* * ****** <infocommande> ************** */
$sceaucontrol->createOrderDetails($_GET['refid'], $_GET['siteid'], $_GET['amount'], 'EUR', $_SERVER['REMOTE_ADDR'], date('Y-m-d H:i:s'));

 
/* * ****** <paiement> ************** */
$sceaucontrol->createPayment($_GET['methodfia']);

/** Sending to Fia-Net * */
$sceau = new Sceau();
$result = $sceau->sendSendrating($sceaucontrol);
if (isXMLstringSceau($result)) {
    $resxml = new SceauSendratingResponse($result);
	if ($resxml->isValid()) {
		//update fianetsceau_state 2:sent
		db_query("UPDATE {commerce_order} SET order_status_sceau = '1' WHERE order_id = '" . $_GET['refid'] . "'");
		//mise à jour des logs FIA-NET
		SceauLogger::insertLogSceau(__METHOD__ . " : " . __LINE__, 'Order sended');
	} 
	else {
		//update fianetsceau_state 3:error
		SceauLogger::insertLogSceau(__METHOD__ . " : " . __LINE__, 'Order '.$order->order_id.' XML send error : ' . $resxml->getDetail());
	}
} 
else {
	//update fianetsceau_state 3:error
	SceauLogger::insertLogSceau(__METHOD__ . " : " . __LINE__, 'Order '.$order->order_id.' XML send error : ' . $resxml->getDetail());
}

echo "<html><head></head>
	<body>
		Traitement en cours...
		<script type='text/javascript'>window.location='" . $_SERVER['HTTP_REFERER'] . "';</script>
	</body></html>";