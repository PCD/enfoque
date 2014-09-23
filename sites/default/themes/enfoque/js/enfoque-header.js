// $Id: $
(function ($) {
  Drupal.behaviors.enfoqueHeader = {
    attach: function (context, settings) {
      /* Header Image */
      /*if ( $('#block-views-header-taxonomy-block').length > 0 ) {
        width = $('#block-views-header-taxonomy-block .header-image img').attr('width');
        height = $('#block-views-header-taxonomy-block .header-image img').attr('height');
        if ( width < 1 ) {
          width = 650;
        }
        if ( height < 1 ) {
          height = 165;
        }
        width_st = (width.toString()) + 'px';
        height_st = (height.toString()) + 'px';
        $('#block-views-header-taxonomy-block .header-image a, #block-views-header-taxonomy-block .header-image a img').css({
          'width': width_st, 
          'height': height_st
        });
        $('#block-views-header-taxonomy-block .header-image').css({
          'width': '650px', 
          'height': '165px'
        });
      }*/
    }
  }
})(jQuery);
