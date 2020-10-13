(function($) {
  Drupal.behaviors.DatePopupTimepicker = {
    attach: function(context, settings) {
      for (var id in settings.datePopup) {
        $('#'+ id).each(function() {
          // @todo Do we need to use .once() here? date_popup doesn't use it for some reason.
          if (settings.datePopup[id].func === 'date_popup_timepicker__timepicker') {
            $('#'+ id, context).once('date-popup-timepicker')
              .timepicker(settings.datePopup[id].settings);
          }
          if (settings.datePopup[id].func == 'date_popup_timepicker__timepicker_inline') {
            // Hide the original date element.
            $('#'+ this.id + '-hidden').parent().children().hide();
            // Create inline timepicker.
            $(this).timepicker(settings.datePopup[id].settings);
            // Trigger the original date element when selection changes.
            $('#'+ this.id).bind('change', function() {
              $('#'+ this.id + '-hidden').trigger('change');
            });
          }
        });
      }
    }
  };
})(jQuery);
