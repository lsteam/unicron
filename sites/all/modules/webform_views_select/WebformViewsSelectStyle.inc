<?php

/**
 * @file
 * Contains WebformViewsSelectStyle.
 */

/**
 * The style plugin for Webform Views Select.
 */
class WebformViewsSelectStyle extends views_plugin_style_mapping {

  /**
   * Implements views_plugin_style_mapping::define_mapping().
   */
  public function define_mapping() {
    return array(
      'webform_select_key' => array(
        '#title' => t('Short, raw options (keys)'),
        '#required' => TRUE,
      ),
      'webform_select_value' => array(
        '#title' => t('Full, human-readable options (values)'),
        '#required' => TRUE,
      ),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['html'] = array(
      'contains' => array(
        'strip' => array(
          'default' => TRUE,
          'bool' => TRUE,
        ),
        'tags_allowed' => array(
          'default' => 'a em strong cite blockquote code ul ol li dl dt dd',
        ),
      ),
    );

    return $options;
  }

  /**
   * {@inheritdoc}
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['html'] = array(
      '#type' => 'fieldset',
      '#title' => t('HTML filter'),
    );

    $form['html']['strip'] = array(
      '#type' => 'checkbox',
      '#title' => t('No HTML tags allowed.'),
      '#default_value' => $this->options['html']['strip'],
    );

    $form['html']['tags_allowed'] = array(
      '#type' => 'textfield',
      '#title' => t('Allowed HTML tags'),
      '#default_value' => $this->options['html']['tags_allowed'],
      '#description' => t('A space-separated list of HTML tags allowed. Disallowed tags are stripped from the content.'),
      '#states' => array(
        'enabled' => array(
          ':input[name="style_options[html][strip]"]' => array('checked' => FALSE),
        ),
      ),
    );
  }

  /**
   * Implements views_plugin_style::render_grouping_sets().
   */
  public function render_grouping_sets($sets, $level = 0) {
    $key = $this->options['mapping']['webform_select_key'];
    $value = $this->options['mapping']['webform_select_value'];

    $output = array();
    foreach ($sets as $set) {
      // Render as a grouping set.
      if (is_array($set) && isset($set['group'])) {
        reset($set['rows']);
        $output[htmlspecialchars_decode(strip_tags($set['group']))] = $this->render_grouping_sets($set['rows'], $level + 1);
      }
      // Render as a record set.
      else {
        $result = (array) $set;
        // We strip HTML tags from the field output because some fields (e.g.
        // Date fields) include tags in their output.
        $rendered_key = htmlspecialchars_decode(strip_tags($result[$key]), ENT_QUOTES);
        if ($this->options['html']['strip'] == FALSE) {
          $tags_allowed = explode(' ', $this->options['html']['tags_allowed']);
          $rendered_value = filter_xss($result[$value], $tags_allowed);
        }
        else {
          $rendered_value = htmlspecialchars_decode(strip_tags($result[$value]), ENT_QUOTES);
        }

        $output[$rendered_key] = $rendered_value;
      }
    }

    return $output;
  }

}
