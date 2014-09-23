<?php
/**
 * Implements hook_permission.
 */
function fakecounter_permission() {
  return array(
    'ver contador de pcd' => array(
      'title' => 'Ver Contador de PCD', 
      'description' => 'Dar acceso a la página administrativa que muestra el contador de visitas de PCD.', 
    ), 
  );
}