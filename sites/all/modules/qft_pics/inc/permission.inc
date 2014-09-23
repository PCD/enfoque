<?php
/**
 * Implements hook_permission.
 */
function qft_pics_permission() {
  return array(
    'Access Pics Remotely' => array(
      'title' => 'Access Pics Remotely', 
      'description' => 'Access Pics Remotely', 
    ), 
    'Post Pics Remotely' => array(
      'title' => 'Post Pics Remotely', 
      'description' => 'Post Pics Remotely', 
    ), 
    'Create A Remote Gallery' => array(
      'title' => 'Create A Remote Gallery', 
      'description' => 'Create A Remote Gallery', 
    ), 
  );
}

/**
 * Gives or declined access to Node Pics export.
 */
function qft_pics_access_pics($node, $access) {
  if ( !isset($_REQUEST['key']) ) {
    return false;
  }
  
  if ( $node->type != 'galeria' ) {
    return false;
  }
  
  if ( $node->status != 1 ) {
    return false;
  }
  
  if ( !user_access($access) ) {
    return false;
  }
  
  //e9c26c4fb01dce9959b96197ce7a853d
  $ikey = variable_get('qft_pics-access_key', md5('qft_pics-access_key'));
  if ( $_REQUEST['key'] != $ikey ) {
    return false;
  }
  
  return true;
}