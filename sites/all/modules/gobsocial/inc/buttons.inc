<?php
function gobsocial_block_view_social_bar($url) {
  $output = "<div id=\"social-buttons\">\n<ul>\n";
  $output .= "<li class=\"social-fb\">" . gobsocial_buttons_facebook($url) . "</li>\n";
  $output .= "<li class=\"social-twitter\">" . gobsocial_buttons_twitter() . "</li>\n";
  $output .= "<li class=\"social-plus last\">" . gobsocial_buttons_plus() . "</li>\n";
  $output .= "</ul>\n</div>\n";
  return $output;
}

/**
 * Returns a Facebook Like Button for Social Buttons.
 */
function gobsocial_buttons_facebook($url) {
  return "<div class=\"fb-like\" data-href=\"{$url}\" data-layout=\"button\" data-action=\"like\" data-show-faces=\"false\" data-share=\"false\"></div>";
}

/**
 * Returns a Twitter Button for Social Buttons.
 */
function gobsocial_buttons_twitter() {
  return "<a href=\"https://twitter.com/share\" class=\"twitter-share-button\" data-lang=\"es\" data-count=\"none\">Twittear</a>
<script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?'http':'https';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+'://platform.twitter.com/widgets.js';fjs.parentNode.insertBefore(js,fjs);}}(document, 'script', 'twitter-wjs');</script>";
}

/**
 * Returns a Google Plus Button for Social Buttons.
 */
function gobsocial_buttons_plus() {
  return "<div class=\"g-plusone\" data-size=\"medium\" data-annotation=\"none\"></div>
  <script type=\"text/javascript\">
  window.___gcfg = {lang: 'es-419'};

  (function() {
    var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
    po.src = 'https://apis.google.com/js/platform.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
  })();
</script>\n";
}