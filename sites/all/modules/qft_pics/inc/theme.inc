<?php
/**
 * Implements hook_theme.
 */
function qft_pics_theme() {
  return array(
    'qft_pics-node_gallery' => array(
      'arguments' => array(
        'pictures' => NULL, 
        'navigation' => NULL, 
        'url' => NULL, 
      ), 
      'template' => 'themes/qft_pics-node_gallery', 
    ), 
    'qft_pics-node_gallery-pictures' => array(
      'arguments' => array(
        'pictures' => NULL, 
      ), 
      'template' => 'themes/qft_pics-node_gallery-pictures', 
    ), 
    'qft_pics-node_gallery-navigation' => array(
      'arguments' => array(
        'pictures' => NULL, 
      ), 
      'template' => 'themes/qft_pics-node_gallery-navigation', 
    ), 
  );
}