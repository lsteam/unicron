<?php

/**
 * @file
 * Interface and base class for timepicker plugins.
 */

/**
 * Define interface DatePopupTimepickerInterface.
 */
interface DatePopupTimepickerInterface {

  /**
   * Element process callback.
   *
   * @param array $element
   *   Form  API element structure to process.
   * @param array $form_state
   *   Form state.
   * @param array $form
   *   The whole form definition structure.
   *
   * @return array
   *   Processed form element.
   */
  public function processElement(array $element, array &$form_state, array $form);

  /**
   * Library definition for libraries module.
   *
   * @return array
   *   Library definition.
   *
   * @see hook_libraries_info()
   */
  public function librariesInfo();

  /**
   * Define field settings form.
   *
   * All exposed settings should be contained by "timepicker_options" element.
   * Helper elements may be defined outside of "timepicker_options".
   *
   * @param array $form
   *   Settings form definition.
   * @param array $context
   *   Array containing 3 elements: "field info",
   *   "instance info" and "has_data" flag.
   * @param array $settings
   *   Currently saved settings in the field.
   *
   * @return array
   *   Timepicker settings form part definition.
   */
  public function fieldSettingsForm(array $form, array $context, array $settings = array());

  /**
   * Field Settings Form validate callback.
   *
   * @param array $element
   *   Field settings form element/part as defined in the fieldSettingsForm().
   * @param array $values
   *   Submitted settings values for defined settings form element/part.
   * @param array $form
   *   Whole settings form basically defined by field_ui_field_edit_form().
   * @param array $form_state
   *   Whole form state passed to field_ui_field_edit_form_validate()
   *   and other validate callbacks of the field_ui_field_edit_form form.
   *
   * @see field_ui_field_edit_form()
   * @see field_ui_field_edit_form_validate()
   */
  public function fieldSettingsFormValidate(array &$element, array &$values, array &$form, array &$form_state);

  /**
   * Field Settings Form submit callback.
   *
   * @param array $element
   *   Field settings form element/part as defined in the fieldSettingsForm().
   * @param array $values
   *   Submitted settings values for defined settings form element/part.
   * @param array $form
   *   Whole settings form basically defined by field_ui_field_edit_form().
   * @param array $form_state
   *   Whole form state passed to field_ui_field_edit_form_submit()
   *   and other submit callbacks of the field_ui_field_edit_form form.
   *
   * @see field_ui_field_edit_form()
   * @see field_ui_field_edit_form_submit()
   */
  public function fieldSettingsFormSubmit(array &$element, array &$values, array &$form, array &$form_state);

  /**
   * Process field settings set in UI for further use in #timepicker options.
   *
   * For example settings may be sanitized, translated, etc.
   *
   * @param array $settings
   *   Timepicked settings saved in field instanc.
   * @param array $element
   *   Form  API element structure to process.
   * @param array $form_state
   *   Form state.
   * @param array $form
   *   The whole form definition structure.
   *
   * @return array
   *   Processed timepicker options.
   */
  public function processFieldSettings(array $settings, array $element, array &$form_state, array $form);

}

/**
 * Class DatePopupTimepicker.
 */
class DatePopupTimepicker implements DatePopupTimepickerInterface {

  /**
   * {@inheritdoc}
   */
  public function processElement(array $element, array &$form_state, array $form) {
    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function librariesInfo() {
    return array();
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsForm(array $form, array $context, array $settings = array()) {
    return array(
      'timepicker_options' => array(),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsFormValidate(array &$element, array &$values, array &$form, array &$form_state) {
    // Do nothing.
  }

  /**
   * {@inheritdoc}
   */
  public function fieldSettingsFormSubmit(array &$element, array &$values, array &$form, array &$form_state) {
    // Do nothing.
  }

  /**
   * {@inheritdoc}
   */
  public function processFieldSettings(array $settings, array $element, array &$form_state, array $form) {
    return isset($settings['timepicker_options']) ? $settings['timepicker_options'] : array();
  }

}
