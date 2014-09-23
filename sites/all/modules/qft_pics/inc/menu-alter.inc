<?php
/**
 * Implements hook_menu_alter.
 */
function qft_pics_menu_alter(&$items) {
  $items['node/%node']['page callback'] = 'qft_pics_menu_galeria_page_view';
}

/**
 * Alters the Page View Render of the Node.
 */
function qft_pics_menu_galeria_page_view($node) {
  drupal_set_title($node->title);
  $uri = entity_uri('node', $node);
  // Set the node path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);
  
  // Default node_show
  if ( $node->type != 'event' ) {
    return node_show($node);
  }
  
  // Use theme
  $output = '';
  
  // Build Galeria
  drupal_add_js(drupal_get_path('module', 'qft_pics') . '/js/jquery.jcarousel.js');
  drupal_add_js(drupal_get_path('module', 'qft_pics') . '/js/node-gallery.js');
  drupal_add_css(drupal_get_path('module', 'qft_pics') . '/css/jcarousel.connected-carousels.css');
  //if ( isset($node->field_picture_evento_teaser[$node->language]) && isset($node->field_picture_large[$node->language]) ) {
  if ( isset($node->field_image_multiple[$node->language]) ) {
    /*$thumbnails = $node->field_picture_thumbnail[$node->language];
    $bigs = $node->field_picture_large[$node->language];*/
    $images = $node->field_image_multiple[$node->language];
    
    // Theme Gallery
    $navigation = $pictures = '';
    /*foreach($thumbnails as $key => $thumbnail) {
      $pictures .= "<li><img src=\"{$bigs[$key]['url']}\" width=\"691\" height=\"396\" /></li>\n";
      $navigation .= "<li><img src=\"{$thumbnail['url']}\" width=\"100\" height=\"100\" /></li>\n";
    }*/
    foreach($images as $key => $img) {
      $picture_path = image_style_url('qft_large', $img['uri']);
      $navigation_path = image_style_url('qft_thumb', $img['uri']);
      $pictures .= "<li><img src=\"{$picture_path}\" width=\"691\" height=\"396\" /></li>\n";
      $navigation .= "<li><img src=\"{$navigation_path}\" width=\"100\" height=\"100\" /></li>\n";
    }
    $pictures = theme('qft_pics-node_gallery-pictures', array('pictures'=>$pictures));
    $navigation = theme('qft_pics-node_gallery-navigation', array('pictures'=>$navigation));
    $output = theme('qft_pics-node_gallery', array(
      'pictures' => $pictures, 
      'navigation' => $navigation, 
      'url' => url('node/' . arg(1), array('absolute'=>TRUE)), 
    ));
    //drupal_add_js(drupal_get_path('module', 'colorbox') . '/js/colorbox.js');
  }
  
  return $output;
}