<?php
/**
 * Implements hook_menu.
 */
function fakecounter_menu() {
  $items = array();
  $items['admin/reports/pcdcounter'] = array(
    'title' => 'Contador de Visitas', 
    'page callback' => 'fakecounter_menu_report', 
    'access arguments' => array('ver contador de pcd'), 
    'description' => 'Despliega un reporte de visitas.',
    'type' => MENU_NORMAL_ITEM, 
  );
  return $items;
}

/**
 * Displays the ammount of Visits.
 */
function fakecounter_menu_report() {
  $visits = number_format(variable_get('fakecounter-visits', 0));
  return theme('fakecounter-report', array('visits' => $visits));
}
