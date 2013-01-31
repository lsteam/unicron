<?php

/**
 * @file
 * Contains our the class for the views plugin for fading ajax.
 */

class views_plugin_ajax_fade extends views_plugin_display_extender {

  /**
   * Provides definitions for our custom options.
   */
  function options_definition_alter(&$options) {
    // Default options for our custom configuration form.
    $options['vaf_use_fade'] = array('default' => 0);
    $options['vaf_fade_duration'] = array('default' => 1500);

    return $options;
  }

  /**
   * Provides the form for setting options.
   */
  function options_form(&$form, &$form_state) {
    switch ($form_state['section']) {
      // Our Form.
      case 'vaf_use_fade':
        $use_fade = $this->display->get_option('vaf_use_fade');
        $duration = $this->display->get_option('vaf_fade_duration');

        // The 'Yes/No' Toggle.
        $form['#title'] = 'Fade Ajax Views';
        $form['vaf_use_fade'] = array(
          '#title' => t('Use fade effect on ajax:'),
          '#description' => t('Applies a fade out/in effect to pager transitions when "Use Ajax" set to "Yes" for the display.'),
          '#type' => 'radios',
          '#options' => drupal_map_assoc(array(t('Yes'), t('No'))),
          '#default_value' => $this->display->get_option('vaf_use_fade'),
        );
        // The fade duration textbox.
        $form['vaf_fade_duration'] = array(
          '#title' => t('Fade Duration (in milliseconds):'),
          '#description' => t('Duration you want the content of the view to fade in and out.'),
          '#type' => 'textfield',
          '#default_value' => $this->display->get_option('vaf_fade_duration'),
          '#size' => 30,
        );
        break;

    }
  }

  /**
   * Validate the inputted code.
   */
  function options_validate(&$form, &$form_state) {

    switch ($form_state['section']) {
      case 'vaf_use_fade':
        // Get the values of the form.
        $values = $form_state['values'];
        // Initialize the values we need to validate
        // against here.
        $use_fade = $values['vaf_use_fade'];
        $duration = $values['vaf_fade_duration'];

        // Throw an an error if the user has checked "Yes"
        // but has not given a duration value.
        if (($use_fade == "Yes") && ($duration == '')) {
          form_set_error('vaf_use_fade', t('Please specify the duration you want the content to fade in.'));
        }
        break;
    }
  }

  /**
   * Helper function to insert the settings from the form.
   */
  function options_submit(&$form, &$form_state) {
    $use_fade   = $form_state['values']['vaf_use_fade'];
    $duration   = $form_state['values']['vaf_fade_duration'];

    // Grab the current settings from the form and save them
    // use set_option(). See views_plugin_display::set_option.
    switch ($form_state['section']) {
      case 'vaf_use_fade':
        $this->display->set_option('vaf_use_fade', $use_fade);
        $this->display->set_option('vaf_fade_duration', $duration);
        break;
    }
  }

  /**
   * Helper function to provide settings in Views UI.
   *
   * Provides settings that are seen is the main views display UI page
   * under 'Advanced'.
   */
  function options_summary(&$categories, &$options) {
    $use_fade = check_plain(trim($this->display->get_option('vaf_use_fade')));
    $duration = check_plain(trim($this->display->get_option('vaf_fade_duration')));
    if ($use_fade == 'Yes') {
      $use_fade = t('Yes') . ' (' . $duration . 'ms)';
    }
    else {
      $use_fade = t('No');
    }
    $options['vaf_use_fade'] = array(
      'category' => 'other',
      'title' => t('Use Ajax + Fade'),
      'value' => $use_fade,
      'desc' => t('Change custom CSS rules for this display.'),
    );
  }
}
