<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * enfoque theme.
 */

/**
 * Check if a block is a menu block or not.
 *
 * @param $block
 *  A block object.
 * @return bool
 *  Given block is a menu block.
 */
function _enfoque_is_menu_block($block) {
  return in_array($block->module, array('menu', 'menu_block')) || ($block->module == 'system' && !in_array($block->delta, array('help', 'powered-by', 'main')));
}

/**
 * @file
 * This file is empty by default because the base theme chain (Alpha & Omega) provides
 * all the basic functionality. However, in case you wish to customize the output that Drupal
 * generates through Alpha & Omega this file is a good place to do so.
 * 
 * Alpha comes with a neat solution for keeping this file as clean as possible while the code
 * for your subtheme grows. Please read the README.txt in the /preprocess and /process subfolders
 * for more information on this topic.
 */
function enfoque_preprocess_html(&$vars) {
  // custom functionality here
  if ( drupal_is_front_page() ) {
    drupal_add_js(drupal_get_path('theme', 'enfoque') . '/js/bxslider/jquery.bxslider.js');
    drupal_add_js(drupal_get_path('theme', 'enfoque') . '/js/enfoque-home.js');
  }
  drupal_add_js(drupal_get_path('theme', 'enfoque') . '/js/enfoque-header.js');
}

/**
 * Overrides theme_menu_link
 */
function enfoque_menu_link(array $vars) {
  static $i;
  $i = !isset($i)?1:$i+1;
  enfoque_menu_link_term_color_set($vars);
  return theme_menu_link($vars);
}

/**
 * Gets taxonomy term color from Menu items.
 */
function enfoque_menu_link_term_color_set(&$vars) {
  $tid_pattern = '/taxonomy\/term\/(\d+)/';
  $menu_url = $vars['element']['#href'];
  $color = 'orange';
  if ( preg_match($tid_pattern, $menu_url, $match) > 0 ) {
    $tid = intval($match[1]);
    $term = taxonomy_term_load($tid);
    if ( isset($term->field_color[LANGUAGE_NONE][0]['value']) ) {
      $color = $term->field_color[LANGUAGE_NONE][0]['value'];
    }
    // Set Class
    $vars['element']['#attributes']['class'][] = $color;
  }
  
  return $color;
}

/**
 * Sets custom breadcrumb.
 */
function enfoque_breadcrumb($vars) {
  // Alter for type
  enfoque_breadcrumb_edicion_impresa($vars);
  
  return theme_breadcrumb($vars);
}

/**
 * Alter breadcrumb for Edicion Impresa Nodes.
 */
function enfoque_breadcrumb_edicion_impresa(&$vars) {
  if ( arg(0) == 'node' && intval(arg(1)) > 1 && is_null(arg(2)) ) {
    $node = node_load(arg(1));
    if ( $node->type == 'edicion_impresa' && isset($node->field_categoria[LANGUAGE_NONE][0]['tid']) ) {
      $term = taxonomy_term_load($node->field_categoria[LANGUAGE_NONE][0]['tid']);
      module_load_include('inc', 'pathauto');
      $name_safe = pathauto_cleanstring($term->name);
      $vars['breadcrumb'][] = l($term->name, 'edicion-impresa/' . $name_safe);
    }
  } else if ( arg(0) == 'edicion-impresa' && !is_null(arg(1)) && isset($vars['breadcrumb'][2]) ) {
    unset($vars['breadcrumb'][2]);
  }
}