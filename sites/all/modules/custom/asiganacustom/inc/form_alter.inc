<?php
/**
 * Implements hook_form_FORM_ID_alter.
 */
function asiganacustom_form_galeria_node_form_alter(&$form, &$form_state, $form_id) {
  // Check if the user doesn't need to be limited
  if ( user_access('post-gallery-to-all-candidatos') ) {
    return;
  }
  
  global $user;
  if ( !in_array('editor candidato', $user->roles) ) {
    return;
  }
  
  // Hide the Candidato Drop Down and add a fixed value.
  $luser = user_load($user->uid);
  if ( !isset($luser->field_candidato[LANGUAGE_NONE][0]['tid']) ) {
    drupal_add_message('Lo sentimos, no tienes permisos suficientes para crear galerías.', 'error');
    drupal_goto('<front>');
    exit;
  }
  
  // Dar de alta Galeria
  $candidato = $luser->field_candidato[LANGUAGE_NONE][0]['tid'];
  $form['field_candidato'][LANGUAGE_NONE]['#value'] = $candidato;
  $form['field_candidato'][LANGUAGE_NONE]['#default_value'] = array($candidato);
  $form['field_candidato'][LANGUAGE_NONE]['#type'] = 'value';
}