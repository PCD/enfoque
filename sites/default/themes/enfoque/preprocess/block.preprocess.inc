<?php

/**
 * Implements hook_preprocess_TEMPLATE().
 */
function enfoque_preprocess_block(&$variables) {
  $block = $variables['block'];
  if ($block->region == 'navigation' && _enfoque_is_menu_block($block)) {
    $variables['attributes_array']['class'][] = 'block--nav-bar';
  }
}
