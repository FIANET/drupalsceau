<?php
/**
 * @file
 * Sceau de Confiance menu items.
 */

//insertion des données dans l'api
$newparams['siteid'] = variable_get('commerce_sceau_siteid');
$newparams['login'] = variable_get('commerce_sceau_login');
$newparams['authkey'] = variable_get('commerce_sceau_authkey');
$newparams['status'] = variable_get('commerce_sceau_status');

$newurl['sendrating']['test'] = variable_get('commerce_sceau_url_test');
$newurl['sendrating']['prod'] = variable_get('commerce_sceau_url_prod');

//creates a yml string from the currect object's params
$yaml_string = SpycSceau::YAMLDump($newparams);
$yaml_string2 = SpycSceau::YAMLDump($newurl);
//opens the YAML file and puts the cursor at the beginning
$handle = fopen(SCEAU_ROOT_DIR . '/lib/sceau/const/site_params.yml', 'w');
$handle2 = fopen(SCEAU_ROOT_DIR . '/lib/sceau/const/url.yml', 'w');
//writes new params into the opened file
$written = @fwrite($handle, $yaml_string);
@fwrite($handle2, $yaml_string2);
fclose($handle);
fclose($handle2);

function commerce_Sceau_settings_form() {   
   $form['fianet_sceau'] = array(
   '#type' => 'vertical_tabs',
  );
        
   $form['commerce_sceau_info'] = array(
          '#type' => 'fieldset',
          '#title' => t('Configuration Générale'),
       '#group' => 'fianet_sceau',
   );
   $form['commerce_sceau_info']['commerce_sceau_activation'] = array(
        '#type' => 'select',
        '#title' => t('Activation du service'),
        '#default_value' => variable_get('commerce_sceau_activation', ''),
        '#options' => array(
            '1' => t('Activé'),
            '0' => t('Désactivé'),
        ),
   );
   $form['commerce_sceau_info']['commerce_sceau_status'] = array(
        '#type' => 'select',
        '#title' => t("Environnement"),
        '#description' => t("Environnement d'utilisation du service (Test ou Production)."),
        '#default_value' => variable_get('commerce_sceau_status', ''),
        '#options' => array(
            'test' => t('Mode Test'),
            'prod' => t('Mode Production'),
        ),
   );
   $form['commerce_sceau_info']['commerce_sceau_siteid'] = array(
        '#type' => 'textfield',
        '#title' => t('SiteID'),
        '#description' => t('Votre identifiant Sceau de Confiance.'),
        '#default_value' => variable_get('commerce_sceau_siteid', ''),
        '#size' => 16,
   );
    
   $form['commerce_sceau_info']['commerce_sceau_login'] = array(
        '#type' => 'textfield',
        '#title' => t('Login'),
        '#description' => t('Votre login Sceau de Confiance.'),
        '#default_value' => variable_get('commerce_sceau_login', ''),
        '#size' => 16,
   );
    
   $form['commerce_sceau_info']['commerce_sceau_authkey'] = array(
        '#type' => 'textfield',
        '#title' => t('Clé privée'),
        '#description' => t('Votre clé privée Sceau de Confiance.'),
        '#default_value' => variable_get('commerce_sceau_authkey', ''),
        '#size' => 30,
   );

   $payment_methods = &drupal_static(__FUNCTION__);
   $payment_methods = array();

   foreach (module_implements('commerce_payment_method_info') as $module) {
     foreach (module_invoke($module, 'commerce_payment_method_info') as $method_id => $payment_method) {
       $payment_method['method_id'] = $method_id;
       $payment_method['module'] = $module;
       $payment_methods[$method_id] = $payment_method;
     }
    }
    
   $form['commerce_sceau_method'] = array(
         '#type' => 'fieldset',
         '#title' => t('Configuration des méthodes de paiement'),
         '#group' => 'fianet_sceau',
   );
     
   drupal_alter('commerce_payment_method_info', $payment_methods);
   foreach ($payment_methods as $methodes) {
		$form['commerce_sceau_method']['commerce_sceau_payment_types' . $methodes['method_id']] = array(
            '#type' => 'select',
            '#title' => t('Type de Paiement - ' . $methodes['method_id']),
            '#options' => array(
                '1' => t('Carte bancaire'),
                '2' => t('Chèque'),
                '3' => t('Contre Remboursement'),
                '4' => t('Virement'),
                '5' => t('Carte bancaire en n fois'),
                '6' => t('Paypal'),
                '7' => t('1euro.com'),
                '8' => t('Kwixo'),
            ),
            '#default_value' => variable_get('commerce_sceau_payment_types'. $methodes['method_id'], '1'),
        );
    }
    
    $form['commerce_sceau_urls'] = array(
         '#type' => 'fieldset',
         '#title' => t('URLs'),
        '#group' => 'fianet_sceau',
    );
    
    $form['commerce_sceau_urls']['commerce_sceau_url_test'] = array(
        '#type' => 'textfield',
        '#title' => t('URL Test'),
        '#default_value' => variable_get('commerce_sceau_url_test', 'https://www.fia-net.com/engine/preprod/sendrating.cgi'),
        '#size' => 30,
    );
    
    $form['commerce_sceau_urls']['commerce_sceau_url_prod'] = array(
        '#type' => 'textfield',
        '#title' => t('URL Prod'),
        '#default_value' => variable_get('commerce_sceau_url_prod', 'https://www.fia-net.com/engine/sendrating.cgi'),
        '#size' => 30,
    );

   $form['commerce_sceau_log'] = array(
        '#type' => 'fieldset',
        '#title' => '<a target="_blank" href="'.dirname($_SERVER["PHP_SELF"]).'/'.drupal_get_path('module', 'commerce_Sceau').'/logs/fianet_log.txt">Accéder aux logs FIA-NET</a>',
   );
  return system_settings_form($form);
}