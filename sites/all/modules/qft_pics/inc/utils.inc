<?php
/** 
 * Send a POST requst using cURL 
 * @param string $url to request 
 * @param array $post values to send 
 * @param array $options for cURL 
 * @return string 
 */ 
function qft_pics_curl_post($url, array $post = NULL, array $options = array()) { 
  $defaults = array( 
      CURLOPT_POST => 1, 
      CURLOPT_HEADER => 0, 
      CURLOPT_URL => $url, 
      CURLOPT_FRESH_CONNECT => 1, 
      CURLOPT_RETURNTRANSFER => 1, 
      CURLOPT_FORBID_REUSE => 1, 
      CURLOPT_TIMEOUT => 4, 
      CURLOPT_POSTFIELDS => http_build_query($post) 
  ); 

  $ch = curl_init(); 
  curl_setopt_array($ch, ($options + $defaults)); 
  if( ! $result = curl_exec($ch)) 
  { 
      trigger_error(curl_error($ch)); 
  } 
  curl_close($ch); 
  return $result; 
}

/** 
 * Send a GET requst using cURL 
 * @param string $url to request 
 * @param array $get values to send 
 * @param array $options for cURL 
 * @return string 
 */ 
function qft_pics_curl_get($url, array $get = NULL, array $options = array()) {    
  $defaults = array( 
    CURLOPT_URL => $url, 
    CURLOPT_HEADER => 0, 
    CURLOPT_RETURNTRANSFER => TRUE, 
    CURLOPT_TIMEOUT => 4 
  );
  if ( !is_null($get) ) {
    $defaults[CURLOPT_URL] = $url. (strpos($url, '?') === FALSE ? '?' : ''). http_build_query($get);
  }
  
  $ch = curl_init(); 
  curl_setopt_array($ch, ($options + $defaults)); 
  if( ! $result = curl_exec($ch)) 
  { 
      trigger_error(curl_error($ch)); 
  } 
  curl_close($ch); 
  return $result; 
}

function chmod_R($path, $filemode, $dirmode) { 
  if (is_dir($path) ) { 
      if (!chmod($path, $dirmode)) { 
          $dirmode_str=decoct($dirmode); 
          print "Failed applying filemode '$dirmode_str' on directory '$path'\n"; 
          print "  `-> the directory '$path' will be skipped from recursive chmod\n"; 
          return; 
      } 
      $dh = opendir($path); 
      while (($file = readdir($dh)) !== false) { 
          if($file != '.' && $file != '..') {  // skip self and parent pointing directories 
              $fullpath = $path.'/'.$file; 
              chmod_R($fullpath, $filemode,$dirmode); 
          } 
      } 
      closedir($dh); 
  } else { 
      if (is_link($path)) { 
          print "link '$path' is skipped\n"; 
          return; 
      } 
      if (!chmod($path, $filemode)) { 
          $filemode_str=decoct($filemode); 
          print "Failed applying filemode '$filemode_str' on file '$path'\n"; 
          return; 
      } 
  } 
}