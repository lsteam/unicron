<?php

/**
 * @file
 * The jQuery UI Timepicker plugin.
 */

$plugin = array(
  'title' => t('jQuery UI Timepicker'),
  'description' => t('Provides jQuery UI Timepicker integration'),
  'handler' => array(
    'class' => 'DatePopupTimepickerTimepicker',
  ),
);

/**
 * Class DatePopupTimepickerTimepicker.
 */
class DatePopupTimepickerTimepicker extends DatePopupTimepicker {

  /**
   * Get default settings for the plugin.
   *
   * @return array
   *   Settings structured array.
   */
  public function settingsDefaults() {
    $settings = array(
      // The character to use to separate hours and minutes.
      'timeSeparator' => ':',
      // Define whether or not to show a leading zero for hours < 10.
      'showLeadingZero' => TRUE,
      // Define whether or not to show a leading zero for minutes < 10.
      'showMinutesLeadingZero' => TRUE,
      // Define whether or not to show AM/PM with selected time.
      'showPeriod' => FALSE,
      // Define if the AM/PM labels on the left are displayed.
      'showPeriodLabels' => TRUE,
      // The character to use to separate the time from the time period.
      'periodSeparator' => ' ',
      // Define an alternate input to parse selected time to.
      'altField' => '#alternate_input',
      // Used as default time when input field is empty
      // or for inline timePicker.
      // Set to 'now' for the current time, '' for no highlighted time.
      'defaultTime' => 'now',
      // Trigger options.
      // Define when the timepicker is shown.
      // 'focus': when the input gets focus,
      // 'button' when the button trigger element is clicked.
      // 'both': when the input gets focus and when the button is clicked.
      'showOn' => 'focus',
      // jQuery selector that acts as button trigger. ex: '#trigger_button'.
      'button' => NULL,
      // Localization.
      // Define the locale text for "Hours".
      'hourText' => 'Hour',
      // Define the locale text for "Minute".
      'minuteText' => 'Minute',
      // Define the locale text for periods.
      'amPmText' => array('AM', 'PM'),
      // Position.
      // Corner of the dialog to position,
      // used with the jQuery UI Position utility if present.
      'myPosition' => 'left top',
      // Corner of the input to position.
      'atPosition' => 'left bottom',
      // Events.
      // Callback function executed before
      // the timepicker is rendered and displayed.
      'beforeShow' => NULL,
      // Define a callback function when an hour / minutes is selected.
      'onSelect' => NULL,
      // Define a callback function when the timepicker is closed.
      'onClose' => NULL,
      // Define a callback to enable / disable certain hours.
      // ex: function onHourShow(hour).
      'onHourShow' => NULL,
      // Define a callback to enable / disable certain minutes.
      // ex: function onMinuteShow(hour, minute).
      'onMinuteShow' => NULL,
      // Custom hours and minutes.
      'hours' => array(
        // First displayed hour.
        'starts' => 0,
        // Last displayed hour.
        'ends' => 23,
      ),
      'minutes' => array(
        // First displayed minute.
        'starts' => 0,
        // Last displayed minute.
        'ends' => 55,
        // Interval of displayed minutes.
        'interval' => 5,
        // Optional extra entries for minutes.
        'manual' => array(),
      ),
      // Number of rows for the input tables, minimum 2,
      // makes more sense if you use multiple of 2.
      'rows' => 4,
      // Define if the hours section is displayed or not.
      // Set to false to get a minute only dialog.
      'showHours' => TRUE,
      // Define if the minutes section is displayed or not.
      // Set to false to get an hour only dialog.
      'showMinutes' => TRUE,

      // Min and Max time.
      // Set the minimum time selectable by the user, disable hours and minutes
      // previous to min time.
      'minTime' => array(
        'hour' => NULL,
        'minute' => NULL,
      ),
      // Set the minimum time selectable by the user, disable hours and minutes
      // after max time.
      'maxTime' => array(
        'hour' => NULL,
        'minute' => NULL,
      ),
      // Buttons.
      // Shows an OK button to confirm the edit.
      'showCloseButton' => FALSE,
      // Text for the confirmation button (ok button).
      'closeButtonText' => 'Done',
      // Shows the 'now' button.
      'showNowButton' => FALSE,
      // Text for the now button.
      'nowButtonText' => 'Now',
      // Shows the deselect time button.
      'showDeselectButton' => FALSE,
      // Text for the deselect button.
      'deselectButtonText' => 'Deselect',
      // Timepicker type.
      'timepickerType' => 'popup',
    );
    return $settings;
  }

  /**
   * {@inheritdoc}
   */
  public function processElement(array $element, array &$form_state, array $form) {
    $granularity = date_popup_time_granularity($element);
    $options_default = array(
      'showHours' => in_array('hour', $granularity),
      'showMinutes' => in_array('minute', $granularity),
    );
    $element['#timepicker_options'] = array_replace_recursive($options_default, $element['#timepicker_options']);
    $element['#attached']['library'][] = array('system', 'ui.core');
    $element['#attached']['library'][] = array('system', 'ui.widget');

    // Add Timepicker library.
    $element['#attached']['libraries_load'][] = array('timepicker');
    // @todo is it the best place for plugin specific .js?
    $element['#attached']['js'][] = drupal_get_path('module', 'date_popup_timepicker') . "/js/date_popup_timepicker.timepicker.js";

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function librariesInfo() {
    $libraries = array();
    $libraries['timepicker'] = array(
      'name' => 'jQuery UI Timepicker',
      'vendor url' => 'https://fgelinas.com/code/timepicker',
      'download url' => 'https://fgelinas.com/code/timepicker/releases/jquery-ui-timepicker-0.3.3.zip',
      'version arguments' => array(
        'file' => 'jquery.ui.timepicker.js',
        'pattern' => '/version\: \"(\d\.\d.\d)\"/',
        'lines' => 43,
      ),
      'files' => array(
        'js' => array(
          'jquery.ui.timepicker.js',
        ),
        'css' => array(
          'jquery.ui.timepicker.css',
        ),
      ),
      'variants' => array(
        'source' => array(
          'files' => array(
            'js' => array(
              'jquery.ui.timepicker.js',
            ),
            'css' => array(
              'jquery.ui.timepicker.css',
            ),
          ),
        ),
      ),
    );

    return $libraries;
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsForm(array $form, array $context, array $settings = array()) {
    $element = array();
    $defaults = $this->settingsDefaults();
    $options = isset($settings['timepicker_options']) ? $settings['timepicker_options'] : array();
    $options = array_replace_recursive($defaults, $options);
    $element['showLeadingZero'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show hours leading zero'),
      '#description' => t('Define whether or not to show a leading zero for hours < 10.'),
      '#default_value' => $options['showLeadingZero'],
    );
    $element['showPeriodLabels'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show period labels'),
      '#description' => t('Define if the AM/PM labels on the left are displayed.'),
      '#default_value' => $options['showPeriodLabels'],
    );
    // Define an alternate input to parse selected time to
    // $form['altField'] = array();
    // @todo Use date_popup element.
    $element['timepickerType'] = array(
      '#title' => t('Choose how to render the timepicker'),
      '#type' => 'radios',
      '#options' => array(
        'popup' => t('Popup'),
        'inline' => t('Inline'),
      ),
      '#default_value' => $options['timepickerType'],
    );
    $element['defaultTime'] = array(
      '#type' => 'textfield',
      '#title' => t('Default time'),
      '#description' => t("Used as default time when input field is empty or for inline timePicker. Set to 'now' for the current time, '' for no highlighted time."),
      '#default_value' => $options['defaultTime'],
    );
    $element['showOn'] = array(
      '#type' => 'select',
      '#title' => t('Show on'),
      '#description' => t("Define when the timepicker is shown."),
      '#options' => array(
        'focus' => t('Focus'),
        'button' => t('Button'),
        'both' => t('Both'),
        'none' => t('None'),
      ),
      '#default_value' => $options['showOn'],
    );
    // jQuery selector that acts as button trigger. ex: '#trigger_button'
    // $element['button'] = array();
    $element['hourText'] = array(
      '#type' => 'textfield',
      '#title' => t('Hour text'),
      '#default_value' => $options['hourText'],
    );
    $element['minuteText'] = array(
      '#type' => 'textfield',
      '#title' => t('Minute text'),
      '#default_value' => $options['minuteText'],
    );
    // Corner of the dialog to position,
    // used with the jQuery UI Position utility if present.
    // $element['myPosition'] = array();
    // Corner of the input to position
    // $element['atPosition'] = array();
    $element['hours'] = array(
      '#type' => 'fieldset',
      '#title' => t('Hours'),
      '#collapsible' => FALSE,
      'starts' => array(
        '#type' => 'textfield',
        '#title' => t('Starts'),
        '#description' => t('First displayed hour.'),
        '#default_value' => $options['hours']['starts'],
      ),
      'ends' => array(
        '#type' => 'textfield',
        '#title' => t('Ends'),
        '#description' => t('Last displayed hour.'),
        '#default_value' => $options['hours']['ends'],
      ),
    );
    $element['minutes'] = array(
      '#type' => 'fieldset',
      '#title' => t('Minutes'),
      '#collapsible' => FALSE,
      'starts' => array(
        '#type' => 'textfield',
        '#title' => t('Starts'),
        '#description' => t('First displayed minute.'),
        '#default_value' => $options['minutes']['starts'],
      ),
      'ends' => array(
        '#type' => 'textfield',
        '#title' => t('Ends'),
        '#description' => t('Last displayed minute.'),
        '#default_value' => $options['minutes']['ends'],
      ),
      'interval' => array(
        '#type' => 'textfield',
        '#title' => t('Interval'),
        '#description' => t('Interval of displayed minutes.'),
        '#default_value' => $options['minutes']['interval'],
      ),
      // Optional extra entries for minutes
      // 'manual' => array(),
    );
    $element['rows'] = array(
      '#type' => 'textfield',
      '#title' => t('Rows'),
      '#description' => t('Number of rows for the input tables, minimum 2, makes more sense if you use multiple of 2.'),
      '#default_value' => $options['rows'],
    );
    $element['minTime'] = array(
      '#type' => 'fieldset',
      '#title' => t('Min time'),
      '#description' => t('Set the minimum time selectable by the user, disable hours and minutes previous to min time.'),
      '#collapsible' => FALSE,
      'hour' => array(
        '#type' => 'textfield',
        '#title' => t('Min hour'),
        '#default_value' => $options['minTime']['hour'],
      ),
      'minute' => array(
        '#type' => 'textfield',
        '#title' => t('Min minute'),
        '#default_value' => $options['minTime']['minute'],
      ),
    );
    $element['maxTime'] = array(
      '#type' => 'fieldset',
      '#title' => t('Max time'),
      '#description' => t('Set the minimum time selectable by the user, disable hours and minutes after max time.'),
      '#collapsible' => FALSE,
      'hour' => array(
        '#type' => 'textfield',
        '#title' => t('Max hour'),
        '#default_value' => $options['maxTime']['hour'],
      ),
      'minute' => array(
        '#type' => 'textfield',
        '#title' => t('Max minute'),
        '#default_value' => $options['maxTime']['minute'],
      ),
    );
    $element['showCloseButton'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show close button'),
      '#description' => t('Shows an OK button to confirm the edit.'),
      '#default_value' => $options['showCloseButton'],
    );
    $element['closeButtonText'] = array(
      '#type' => 'textfield',
      '#title' => t('Close button text'),
      '#description' => t('Text for the confirmation button (ok button).'),
      '#default_value' => $options['closeButtonText'],
    );
    $element['showNowButton'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show now button'),
      '#description' => t('Shows the "now" button.'),
      '#default_value' => $options['showNowButton'],
    );
    $element['nowButtonText'] = array(
      '#type' => 'textfield',
      '#title' => t('Now button text'),
      '#description' => t('Text for the now button.'),
      '#default_value' => $options['nowButtonText'],
    );
    $element['showDeselectButton'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show deselect button'),
      '#description' => t('Shows the deselect time button.'),
      '#default_value' => $options['showDeselectButton'],
    );
    $element['deselectButtonText'] = array(
      '#type' => 'textfield',
      '#title' => t('Deselect button text'),
      '#description' => t('Text for the deselect button.'),
      '#default_value' => $options['deselectButtonText'],
    );
    return array(
      // There is no need to define #tree => TRUE.
      'timepicker_options' => $element,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsFormValidate(array &$element, array &$values, array &$form, array &$form_state) {
    // Validate int hours and minutes settings.
    foreach (array('hours', 'minutes', 'minTime', 'maxTime') as $key) {
      foreach ($values['timepicker_options'][$key] as $subkey => $value) {
        // Init validation limits.
        if ($key == 'minutes' && $subkey == 'interval') {
          $limits = array(1, 59);
        }
        elseif ($key == 'hours' || $subkey == 'hour') {
          $limits = array(0, 23);
        }
        // Remaining things are minutes.
        else {
          $limits = array(0, 59);
        }
        // Validate.
        if ($value !== '') {
          if (!is_numeric($value) || intval($value) != $value || $value < $limits[0] || $value > $limits[1]) {
            $t_args = array(
              '%name' => $element['timepicker_options'][$key][$subkey]['#title'],
              '@start' => $limits[0],
              '@end' => $limits[1],
            );
            form_error($element['timepicker_options'][$key][$subkey],
              t('%name must be an integer between @start and @end.', $t_args));
          }
          else {
            form_set_value($element['timepicker_options'][$key][$subkey], (int) $value, $form_state);
          }
        }
        else {
          form_set_value($element['timepicker_options'][$key][$subkey], NULL, $form_state);
        }
      }
    }

    // Validate rows part.
    $value = $values['timepicker_options']['rows'];
    if ($value !== '') {
      if (!is_numeric($value) || intval($value) != $value || $value < 2) {
        $t_args = array(
          '%name' => $element['timepicker_options']['rows']['#title'],
        );
        form_error($element['timepicker_options']['rows'],
          t('%name must be an integer greater than 1.', $t_args));
      }
      else {
        form_set_value($element['timepicker_options']['rows'], (int) $value, $form_state);
      }
    }
    else {
      form_set_value($element['timepicker_options']['rows'], NULL, $form_state);
    }

    // Define format for Date Entry.
    $input_format = $form_state['values']['instance']['widget']['settings']['input_format'];
    $granularity = $form_state['values']['field']['settings']['granularity'];
    $date_granularity = array('year', 'month', 'day');
    foreach ($granularity as $key => $value) {
      if (!$value || in_array($key, $date_granularity)) {
        unset($granularity[$key]);
      }
    }
    $date_entry_format = date_limit_format($input_format, $granularity);

    // If date field expect seconds, we should not show timepicker,
    // as it not supports seconds entry and will lead to invalid value entry.
    // s = Seconds with leading zeros.
    if (stripos($date_entry_format, 's') === FALSE) {
      // Depending on format, apply the appropriate settings.
      // We can omit leading zeros setting for hours.
      // g = 12-hour format of an hour without leading zeros.
      // G = 24-hour format of an hour without leading zeros.
      // h = 12-hour format of an hour with leading zeros.
      // H = 24-hour format of an hour with leading zeros.
      if (strpos($date_entry_format, 'H') !== FALSE ||
        strpos($date_entry_format, 'G') !== FALSE) {
        if (strpos($date_entry_format, 'a') === FALSE) {
          $values['timepicker_options']['showHours'] = TRUE;
          $values['timepicker_options']['showPeriod'] = FALSE;
        }
        else {
          drupal_set_message(t('Error occured. Please contact site administrator'), 'error');
          watchdog('Date popup timepicker', 'Error occured. 24-hour format should not have am/pm time period.', array(), WATCHDOG_ERROR);
          $values['timepicker_options']['showOn'] = 'none';
        }
      }
      elseif (strpos($date_entry_format, 'h') !== FALSE ||
        strpos($date_entry_format, 'g') !== FALSE) {
        if (strpos($date_entry_format, 'a') !== FALSE) {
          $values['timepicker_options']['showHours'] = TRUE;
          $values['timepicker_options']['showPeriod'] = TRUE;
        }
        else {
          drupal_set_message(t('Error occured. Please contact site administrator'), 'error');
          watchdog('Date popup timepicker', 'Error occured. 12-hour format should have am/pm time period.', array(), WATCHDOG_ERROR);
          $values['timepicker_options']['showOn'] = 'none';
        }
      }
      else {
        $values['timepicker_options']['showHours'] = FALSE;
        // Date Field may ask to enter values like :00am,
        // it's not supported by timepicker.
        if (strpos($date_entry_format, 'a') === FALSE) {
          $values['timepicker_options']['showPeriod'] = FALSE;
        }
        else {
          drupal_set_message(t("Timepicker can't be used when hours disabled and
          am/pm period is enabled. To enable timepicker again, go to field settings,
          enable hours attribute or change date entry format and change 'Show on'
          setting state."), 'warning');
          $values['timepicker_options']['showOn'] = 'none';
        }
      }

      // If date field expect minutes, show minutes selector.
      // i = Minutes with leading zeros.
      if (stripos($date_entry_format, 'i') !== FALSE) {
        $values['timepicker_options']['showMinutes'] = TRUE;
      }
      else {
        $values['timepicker_options']['showMinutes'] = FALSE;
      }

      // Options that are not supported by the Date module:
      // Date module supports only colon as time separator.
      $values['timepicker_options']['timeSeparator'] = ':';
      // Date Popup Timepicker supports minutes without leading
      // zeros, but Date module only uses standard PHP Date options.
      $values['timepicker_options']['showMinutesLeadingZero'] = FALSE;
      // Period separator is not supported in Date module.
      $values['timepicker_options']['periodSeparator'] = '';
      // Only lowercase am/pm text allowed in Date module.
      $values['timepicker_options']['amPmText'] = ['am', 'pm'];
    }
    else {
      drupal_set_message(t("Timepicker can't be used with seconds and was
      disabled. To enable timepicker again, go to field settings, disable
      seconds attribute and change 'Show on' setting state."), 'warning');
      $values['timepicker_options']['showOn'] = 'none';
    }

    // Field validation by ron_s.
    // See https://www.drupal.org/project/date_popup_timepicker/issues/2561147.
    if ($values['timepicker_options']['showOn'] != 'none') {
      $options = $values['timepicker_options'];

      $format = '';
      if ($options['showHours']) {
        $format .= $options['showPeriod'] ? 'g' : 'H';
      }

      if ($options['showHours'] && $options['showMinutes']) {
        $format .= $options['timeSeparator'] . 'i';
      }
      elseif ($options['showMinutes']) {
        $format .= 'i';
      }
      if ($options['periodSeparator']) {
        $format .= $options['periodSeparator'];
      }
      // Date Popup Timepicker supports any period label,
      // but Date module only uses standard PHP Date options.
      // a = Lowercase am/pm
      // A = Uppercase AM/PM
      // \ = Escape words added as label.
      $period_label_error = FALSE;
      if ($options['showPeriod']) {
        // Capture matches when AM and PM periods text
        // includes an "am" or "pm" (case insensitive).
        preg_match('/^(.*)(am)(.*)$/i', $options['amPmText'][0], $am_matches);
        preg_match('/^(.*)(pm)(.*)$/i', $options['amPmText'][1], $pm_matches);
        // If either AM or PM periods text do not have a match,
        // determine if there is a match in one of them.
        if (empty($am_matches) || empty($pm_matches)) {
          // If there is an AM periods text format match,
          // use it to determine am/pm case.
          if (!empty($am_matches)) {
            if (strtoupper($am_matches[2]) == $am_matches[2]) {
              $format .= $am_matches[1] . 'A' . $am_matches[3];
            }
            else {
              $format .= $am_matches[1] . 'a' . $am_matches[3];
            }
          }
          // If there is a PM periods text format match,
          // use it to determine am/pm case.
          elseif (!empty($pm_matches)) {
            if (strtoupper($pm_matches[2]) == $pm_matches[2]) {
              $format .= $pm_matches[1] . 'A' . $pm_matches[3];
            }
            else {
              $format .= $pm_matches[1] . 'a' . $pm_matches[3];
            }
          }
          // If there is an AM periods text value,
          // use it to create an escaped am/pm label.
          elseif (!empty($options['amPmText'][0])) {
            $format .= preg_replace('/([a-z0-9])/i', '\\\\$1', $options['amPmText'][0]);
          }
          // If there is a PM periods text value,
          // use it to create an escaped am/pm label.
          elseif (!empty($options['amPmText'][1])) {
            $format .= preg_replace('/([a-z0-9])/i', '\\\\$1', $options['amPmText'][1]);
          }
        }
        // If both AM and PM periods text have matches, check that
        // preceding and following text for each is consistent.
        elseif ($am_matches[1] != $pm_matches[1] || $am_matches[3] != $pm_matches[3]) {
          form_error($element['timepicker_options']['amPmText'], t('Format of AM and PM periods text is inconsistent. Ensure that any text preceding and following the "AM" and "PM" are the same.'));
          $period_label_error = TRUE;
        }
        // If both AM and PM periods text have matches and are consistent,
        // determine the PHP date format used for am/pm ("A" or "a").
        else {
          // Check if both AM and PM periods text are uppercase.
          if (strtoupper($am_matches[2]) == $am_matches[2] && strtoupper($pm_matches[2]) == $pm_matches[2]) {
            $format .= $am_matches[1] . 'A' . $am_matches[3];
          }
          // Check if either AM or PM periods text is uppercase,
          // while the other is lowercase.
          elseif ((strtoupper($am_matches[2]) != $am_matches[2] && strtoupper($pm_matches[2]) == $pm_matches[2]) || (strtoupper($am_matches[2]) == $am_matches[2] && strtoupper($pm_matches[2]) != $pm_matches[2])
          ) {
            form_error($element['timepicker_options']['amPmText'], t('Format of AM and PM periods text is inconsistent. Ensure "AM" and "PM" are either both uppercase or both lowercase.'));
            $period_label_error = TRUE;
          }
          // Both AM and PM periods text are lowercase.
          else {
            $format .= $am_matches[1] . 'a' . $am_matches[3];
          }
        }
      }
      // Compare Date Entry and Date Popup Timepicker formats.
      if (!$period_label_error && $format != $date_entry_format) {
        form_error(
          $element['timepicker_options'],
          t(
            'Date Popup Timepicker (@format) and Date Entry (@date_entry_format) formats do not match. They must have the same format, otherwise data entry errors will occur.',
            array(
              '@format' => $format,
              '@date_entry_format' => $date_entry_format,
            )
          )
        );
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsFormSubmit(array &$element, array &$values, array &$form, array &$form_state) {
    $new = $values['timepicker_options'];
    // Convert boolean settings to boolean.
    $boolean = array(
      'showLeadingZero',
      'showPeriodLabels',
      'showCloseButton',
      'showNowButton',
      'showDeselectButton',
    );
    foreach ($boolean as $key) {
      $new[$key] = (bool) $values['timepicker_options'][$key];
    }
    // Final cleanup.
    $not_null = function ($el) {
      return isset($el);
    };
    foreach (array('hours', 'minutes', 'minTime', 'maxTime') as $key) {
      $new[$key] = array_filter($values['timepicker_options'][$key], $not_null);
      if (empty($new[$key])) {
        unset($new[$key]);
      }
    }
    if (!isset($new['rows'])) {
      // Make sure that NULL value is removed from settings.
      unset($new['rows']);
    }
    // Set processed values back.
    form_set_value($element['timepicker_options'], $new, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function processFieldSettings(array $settings, array $element, array &$form_state, array $form) {
    $options = isset($settings['timepicker_options']) ? $settings['timepicker_options'] : array();
    if (!empty($options)) {
      // @todo Define this list somewhere else since it's used in the DatePopupTimepickerTimepicker::fieldSettingsFormSubmit() as well.
      // @todo Shorten code if possible.
      $groups = array(
        'boolean' => array(
          'showLeadingZero',
          'showMinutesLeadingZero',
          'showPeriod',
          'showPeriodLabels',
          'showHours',
          'showMinutes',
          'showCloseButton',
          'showNowButton',
          'showDeselectButton',
        ),
        'int' => array(
          'hours',
          'minutes',
          'rows',
          'hour',
          'minute',
          'interval',
          'starts',
          'ends',
        ),
        'no_filtering' => array(
          'timeSeparator',
          'periodSeparator',
        ),
      );
      // Callback for the array_walk_recursive().
      $filter = function (&$item, $key, $groups) {
        if (in_array($key, $groups['boolean'], TRUE)) {
          $item = (bool) $item;
        }
        elseif (in_array($key, $groups['int'], TRUE)) {
          $item = (int) $item;
        }
        elseif (in_array($key, $groups['no_filtering'], TRUE)) {
          // Do nothing.
        }
        else {
          // @todo Use filter_xss_admin() instead?
          $item = check_plain($item);
        }
      };
      // Filter user submitted settings since plugin builds output by just
      // concatenation of strings so it's possible, for example,
      // to insert html into labels.
      array_walk_recursive($options, $filter, $groups);
    }
    return $options;
  }

}
