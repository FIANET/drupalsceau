<?php

/**
 * @file
 * Definition of views_handler_field_url.
 */

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class commerce_Sceau_handler_field_url_fianet extends views_handler_field {

    function option_definition() {
        $options = parent::option_definition();
        $options['display_as_link'] = array('default' => TRUE, 'bool' => TRUE);
        return $options;
    }

    /**
     * Provide link to the page being visited.
     */
    function options_form(&$form, &$form_state) {
        $form['display_as_link'] = array(
             '#title' => t('Display as link'),
             '#type' => 'checkbox',
             '#default_value' => !empty($this->options['display_as_link']),
        );
        parent::options_form($form, $form_state);
    }

	function render($values) {
		if (variable_get('commerce_sceau_activation', '') == "1") {
			$order = commerce_order_load($values->order_id);
			$order_total = db_query("SELECT commerce_order_total_amount FROM {field_data_commerce_order_total} as f, {commerce_order} as c WHERE f.revision_id = c.revision_id and f.revision_id = '" .$order->revision_id. "'")->fetchall();


			/*******  <infos utilisateur>   ***************** */         
			$address = db_query("SELECT commerce_customer_address_country as country, commerce_customer_address_locality as city, commerce_customer_address_postal_code as code_postal, commerce_customer_address_thoroughfare as street, commerce_customer_address_first_name as first_name, commerce_customer_address_last_name as last_name, commerce_customer_address_name_line as firstlast_name 
                    FROM {field_data_commerce_customer_address} 
                    WHERE entity_id = '".$values->users_commerce_order_uid."' ")->fetchAll();

			$first_last = explode(' ',$address[0]->firstlast_name);
			$last_name = $first_last[0];
			$first_name = $first_last[1];


			/*********** récupération des informations sur l'envoi de la commande  ***** */
			$valeur = $this->get_value($values);
			if ($valeur == 1){
				$img = "Envoyé à FIA-NET <img width=20 src=" . dirname($_SERVER["PHP_SELF"]) . '/' . drupal_get_path('module', 'commerce_sceau') . "/images/ok.png />";
			} else {
				if ($order->status == "completed"){
					$img = "</a><form action='" . dirname($_SERVER["PHP_SELF"]) . '/' . drupal_get_path('module', 'commerce_sceau') . "/send.php?id=" . $order->order_id . "' method='GET'>";
					$img.= "<input type=hidden name=DRUPAL_ROOT value='" . getcwd() . "'/>";
					$img.= "<input type=hidden name=refid value='" . $order->order_id . "'/>";
					$img.= "<input type=hidden name='lastname' value='" . $last_name . "'/>";
					$img.= "<input type=hidden name='firstname' value='" . $first_name . "'/>";
					$img.= "<input type=hidden name='email' value='" . $order->mail . "'/>";
					$img.= "<input type=hidden name='amount' value='" . $order_total[0]->commerce_order_total_amount/100 . "'/>";
					$img.= "<input type=hidden name=id value='" . $order->order_id . "'/>";
                    if (isset ($order->data['payment_method'])){
						$payment_method = explode('|',$order->data['payment_method']);
						$img.= "<input type=hidden name=method value='" . $payment_method[0] . "'/>";
						$img.= "<input type=hidden name=methodfia value='" . variable_get('commerce_sceau_payment_types' . $payment_method[0], '') . "' />";
						$img.= "<input type=hidden name=siteid value='" . variable_get('commerce_sceau_siteid', '') . "'/>";
					}
					$img.= "<input type=submit value='Envoyer à FIA-NET'>";
					$img.= "</form>";
				} else {  
					$img = "Flux non envoyé à FIA-NET <img width=20 src=" . dirname($_SERVER["PHP_SELF"]) . '/' . drupal_get_path('module', 'commerce_sceau') . "/images/ko.png />";
				}
			}
		} else
			$img = "Module Sceau de Confiance désactivé <img width=20 src=" . dirname($_SERVER["PHP_SELF"]) . '/' . drupal_get_path('module', 'commerce_sceau') . "/images/warning.gif />";
		return $img;
	}
}