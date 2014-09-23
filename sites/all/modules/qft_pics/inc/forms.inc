<?php
/**
 * Returns a drupal form that imports an external galery into a story node.
 */
function qft_pics_import_external_form($form, &$form_state) {
  $form = array();
  $external_site = variable_get('qft_pics-external_site_url', '');
  $form['url'] = array(
    '#type' => 'value', 
    '#value' => $external_site, 
  );
  $form['path'] = array(
    '#type' => 'textfield', 
    '#title' => t('PATH of the External Gallery'), 
    '#required' => true, 
  );
  if ( !empty($external_site) ) {
    $form['path']['#description'] = "{$external_site}/<strong>node/?</strong>";
  }
  $form['actions'] = array(
    '#type' => 'actions', 
    'submit' => array(
      '#type' => 'submit', 
      '#value' => t('Import'), 
      '#weight' => 1, 
    ), 
  );
  return $form;
}

/**
 * Process the submission of the form qft_pics_import_external_form.
 */
function qft_pics_import_external_form_submit($form, &$form_state) {
  $url = "{$form_state['values']['url']}/{$form_state['values']['path']}";
  print $url;
  exit;
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
  }
}

/**
 * Creates a drupal form for creating new remote gallery.
 */
function qft_pics_create_remote_gallery_form($form, &$form_state) {
  $external_site = variable_get('qft_pics-external_site_url', '');
  $action = "{$external_site}/admin/gallery/new/post";
  $form['#action'] = $action;
  $form['key'] = array(
    '#type' => 'hidden', 
    '#value' => variable_get('qft_pics-access_key', md5('qft_pics-access_key')), 
  );
  $form['place'] = array(
    '#type' => 'select', 
    '#title' => t('Place'), 
    '#options' => array(), 
    '#required' => true, 
  );
  $places = taxonomy_get_tree(2);
  foreach($places as $place) {
    $form['place']['#options'][$place->tid] = check_plain($place->name);
  }
  $form['title'] = array(
    '#type' => 'textfield', 
    '#title' => t('Title'), 
    '#required' => true, 
  );
  $form['date'] = array(
    '#type' => 'date', 
    '#title' => t('Date'), 
    '#required' => true, 
  );
  $form['body'] = array(
    '#type' => 'textarea', 
    '#title' => t('Description'), 
    '#resizable' => false, 
    '#required' => true, 
  );
  $form['pictures_zip'] = array(
    '#type' => 'file', 
    '#title' => t('Zipped Pictures'), 
    '#required' => true, 
    '#upload_validators' => array(
      'file_validate_extensions' => array('zip'), 
    ), 
  );
  $form['actions'] = array(
    '#type' => 'actions', 
    'submit' => array(
      '#type' => 'submit', 
      '#value' => t('Save'), 
      '#weight' => 1, 
    ), 
  );
  return $form;
}

/**
 * Returns a drupal form for configuring its features.
 */
function qft_pics_admin_settings_form($form, &$form_state) {
  $form['external_site_url'] = array(
    '#type' => 'textfield', 
    '#title' => t('External Storage URL'), 
    '#required' => true, 
    '#default_value' => variable_get('qft_pics-external_site_url', ''), 
  );
  $form['access_key'] = array(
    '#type' => 'password_confirm', 
    '#required' => true, 
  );
  $form['actions'] = array(
    '#type' => 'actions', 
    'submit' => array(
      '#type' => 'submit', 
      '#value' => t('Save'), 
      '#weight' => 1, 
    ), 
  );
  return $form;
}

/**
 * Process the submission of the form qft_pics_admin_settings.
 */
function qft_pics_admin_settings_form_submit($form, &$form_state) {
  variable_set('qft_pics-external_site_url', trim($form_state['values']['external_site_url'], ' /'));
  variable_set('qft_pics-access_key', md5($form_state['values']['access_key']));
  drupal_set_message(t('The settings has been saved.'));
}