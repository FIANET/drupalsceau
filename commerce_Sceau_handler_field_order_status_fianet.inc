<?php

/**
 * @file
 * Views handler: Order status field.
 */

/**
 * Returns a human readable text for order status to display in the View.
 */
class commerce_sceau_handler_field_order_status_fianet extends views_handler_field {

  /**
   * Overrides views_handler_field::render().
   */
  function render($values) {
    return check_plain(commerce_order_status_fianet_data($this->get_value($values), 'title'));
  }

}
