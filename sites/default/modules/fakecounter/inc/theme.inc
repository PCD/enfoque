<?php
/**
 * Implements hook_theme.
 */
function fakecounter_theme() {
  return array(
    'fakecounter-report' => array(
      'arguments' => array(
        'visits' => NULL, 
      ), 
      'template' => 'themes/fakecounter-report', 
    ), 
  );
}