<?php
/**
 * Implements hook_permission.
 */
function asiganacustom_permission() {
  $permissions = array(
    'post-gallery-to-all-candidatos' => array(
      'title' => 'Subir Galerias a cualquier Candidato', 
      'description' => 'Subir Galerias a cualquier Candidato', 
    ), 
  );
  return $permissions;
}