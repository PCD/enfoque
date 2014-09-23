<?php
/**
 * Implements hook_block_info.
 */
function gobsocial_block_info() {
  $blocks['gobsocial'] = array(
    'info' => 'Gobierno Social Bar', 
  );
  return $blocks;
}

/**
 * Implements hook_block_view.
 */
function gobsocial_block_view($delta = '') {
  $block = array();
  switch($delta) {
    case 'gobsocial':
      if ( arg(0) == 'node' && intval(arg(1)) > 0 && is_null(arg(2)) ) {
        $node_url = url('node/' . arg(1), array('absolute'=>TRUE));
        $block['subject'] = '';
        $block['content'] = gobsocial_block_view_social_bar($node_url);
        return $block;
      }
      break;
  }
}