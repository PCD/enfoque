// $Id: $
(function ($) {
  Drupal.behaviors.enfoqueVideos = {
    attach: function (context, settings) {
      /* Hero Carousel */
      if ( $('#block-views-videos-block').length > 0 ) {
        $('#block-views-videos-block .view-content').bxSlider({
          mode: 'fade', 
          controls: false, 
          auto: true, 
          autoHover: true, 
          pause: 5000
        });
      }
    }
  }
})(jQuery);
