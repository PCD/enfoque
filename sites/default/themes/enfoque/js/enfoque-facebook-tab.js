// $Id: $
(function ($) {
  Drupal.behaviors.enfoqueFBTab = {
    attach: function (context, settings) {
      /* Make Links Target Blank */
      $('.page-facebook a,.page-facebook form').attr('target', '_blank');
      $('.page-facebook .view-id-noticias_home .pager a').removeAttr('target');
    }
  }
})(jQuery);
