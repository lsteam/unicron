(function($) {
  Drupal.behaviors.accordion_blocks = {
	  attach: function() {
	    $('.accordion_blocks_container').accordion({header: "h2", autoHeight: false, collapsible: true, active: false});
	  }
  };
})(jQuery);


