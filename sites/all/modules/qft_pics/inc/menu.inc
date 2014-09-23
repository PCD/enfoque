<?php
/**
 * Implements hook_menu.
 */
function qft_pics_menu() {
  $items = array();
  $items['node/%node/pics'] = array(
    'page callback' => 'qft_pics_menu_pics', 
    'page arguments' => array(1), 
    'access callback' => 'qft_pics_access_pics', 
    'access arguments' => array(1, 'Access Pics Remotely'), 
    'type' => MENU_CALLBACK, 
  );
  
  $items['admin/gallery/import'] = array(
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('qft_pics_import_external_form'), 
    'access arguments' => array('Access Pics Remotely'), 
    'type' => MENU_CALLBACK, 
  );
  $items['admin/gallery/import/%'] = array(
    'page callback' => 'qft_pics_import_external_gallery_node', 
    'page arguments' => array(3), 
    'access arguments' => array('Access Pics Remotely'), 
    'type' => MENU_CALLBACK, 
  );
  $items['admin/gallery/new'] = array(
    'page callback' => 'drupal_get_form', 
    'page arguments' => array('qft_pics_create_remote_gallery_form'), 
    'access arguments' => array('Create A Remote Gallery'), 
    'type' => MENU_CALLBACK, 
  );
  $items['admin/gallery/new/post'] = array(
    'page callback' => 'qft_pics_menu_new_gallery_post', 
    'access arguments' => array('Post Pics Remotely'), 
    'type' => MENU_CALLBACK, 
  );
  $items['admin/config/media/qft_pics'] = array(
    'title' => 'Quien Falto Pictures',
    'description' => 'Adjust Quien Falto Pictures settings.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('qft_pics_admin_settings_form'),
    'access arguments' => array('administer site configuration'),
  );
  return $items;
}

/**
 * Returns a json export of all the pics of a gallery.
 */
function qft_pics_menu_pics($node) {
  $response = array(
    'title' => trim($node->title), 
    'description' => '', 
    'lugar' => null, 
    'pics' => array(), 
  );
  if ( isset($node->field_lugar[$node->language][0]['tid']) ) {
    $response['lugar'] = $node->field_lugar[$node->language][0]['tid'];
  }
  if ( isset($node->body[$node->language][0]['safe_value']) ) {
    $response['description'] = trim($node->body[$node->language][0]['safe_value']);
  }
  $pics = array(
    'src' => array(), 
  );
  $styles = qft_pics_image_styles();
  foreach($node->field_images[$node->language] as $pic) {
    $pics['src'][] = file_create_url($pic['uri']);
    foreach($styles as $style_machine => $style) {
      $pics[$style_machine][] = image_style_url($style, $pic['uri']);
    }
  }
  $response['pics'] = $pics;
  header('Cache-Control: no-cache, must-revalidate');
  header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
  header('Content-type: application/json');
  print json_encode($response);
  exit;
}

/**
 * Receives a pics post and process it.
 */
function qft_pics_menu_new_gallery_post() {
  // Validate Access
  if ( !isset($_POST['key']) ) {
    drupal_set_message('Access Key not found.', 'error');
    drupal_goto('<front>');
    exit;
  }
  $access_key = variable_get('qft_pics-access_key', md5('qft_pics-access_key'));
  if ( $_POST['key'] != $access_key ) {
    drupal_set_message('Access Key Invalid.', 'error');
    drupal_goto('<front>');
    exit;
  }
  
  // Create node
  if ( !($node = qft_pics_create_gallery_from_post($_POST)) ) {
    drupal_set_message("Couldn't create the gallery.", 'error');
    drupal_goto('<front>');
    exit;
  }
  
  // Return an Export of the node.
  $url_back = "{$_SERVER['HTTP_ORIGIN']}/admin/gallery/import/{$node->nid}";
  drupal_goto($url_back);
  exit;
}

/**
 * Import
 */
function qft_pics_import_external_gallery_node($nid) {
  $external_site = variable_get('qft_pics-external_site_url', '');
  $key = variable_get('qft_pics-access_key', md5('qft_pics-access_key'));
  $url = "{$external_site}/node/{$nid}/pics?key={$key}";
  $get = json_decode(qft_pics_curl_get($url));
  
  // Build $Params.
  global $user;
  $params = array(
    'title' => $get->title, 
    'uid' => $user->uid, 
    'status' => 1, 
    'description' => $get->description, 
    'pics' => array(), 
  );
  // Taxonomy
  if ( isset($get->lugar) && !is_null($get->lugar) ) {
    $params['lugar'] = $get->lugar;
  }
  // Original
  if ( isset($get->pics->src) && !empty($get->pics->src) ) {
    foreach($get->pics->src as $pic) {
      $params['pics']['field_picture_src'][] = $pic;
    }
  }
  // Thumbnail
  if ( isset($get->pics->thumbnail) && !empty($get->pics->thumbnail) ) {
    foreach($get->pics->thumbnail as $pic) {
      $params['pics']['field_picture_thumbnail'][] = $pic;
    }
  }
  // Medium
  if ( isset($get->pics->medium) && !empty($get->pics->medium) ) {
    foreach($get->pics->medium as $pic) {
      $params['pics']['field_picture_medium'][] = $pic;
    }
  }
  // Large
  if ( isset($get->pics->large) && !empty($get->pics->large) ) {
    foreach($get->pics->large as $pic) {
      $params['pics']['field_picture_large'][] = $pic;
    }
  }
  // Evento Teaser
  if ( isset($get->pics->evento_teaser) && !empty($get->pics->evento_teaser) ) {
    foreach($get->pics->evento_teaser as $pic) {
      $params['pics']['field_picture_evento_teaser'][] = $pic;
    }
  }
  // Slider Home
  if ( isset($get->pics->slider_home) && !empty($get->pics->slider_home) ) {
    foreach($get->pics->slider_home as $pic) {
      $params['pics']['field_picture_slider_home'][] = $pic;
    }
  }

  if ( ($node = qft_pics_import_event($params)) ) {
    // Notify
    $link = l($node->title, 'node/' . $node->nid);
    $message = t('El evento !event_link ha sido importado exitosamente.', array('!event_link' => $link));
    drupal_set_message($message);
    drupal_goto('node/' . $node->nid);
  }
}