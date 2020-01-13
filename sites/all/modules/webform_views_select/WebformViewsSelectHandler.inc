<?php

/**
 * @file
 * Contains WebformViewsSelectHandler.
 */

/**
 * The plugin that handles webform_views_select_handler.
 */
class WebformViewsSelectHandler extends views_plugin_display {

  /**
   * {@inheritdoc}
   */
  public function render() {
    $options = $this->execute();
    return theme('select', array('element' => array('#options' => $options)));
  }

  /**
   * {@inheritdoc}
   */
  public function get_style_type() {
    return 'webform_views_select';
  }

  /**
   * {@inheritdoc}
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['style_plugin']['default'] = 'webform_views_select_style';
    $options['defaults']['default']['style_plugin'] = FALSE;
    return $options;
  }

  /**
   * Returns the list of select options in the format required by Webform.
   */
  public function execute($flat = FALSE) {
    // Render the fields.
    $results = array();
    if (!empty($this->view->result) || $this->view->style_plugin->even_empty()) {
      $results = $this->view->style_plugin->render_fields($this->view->result);
    }
    else {
      // There are no results. Stop processing here.
      return array();
    }

    // Do not group if a flat list is requested.
    if ($flat) {
      $grouping_options = array();
    }
    else {
      // Check if there are grouping options set.
      if (isset($this->view->style_options['grouping'])) {
        $grouping_options = $this->view->style_options['grouping'];
      }
      else {
        $grouping_options = array();
      }
    }

    // Group the rows according to the grouping instructions, if specified.
    $sets = $this->view->style_plugin->render_grouping($results, $grouping_options, TRUE);
    $rows = $this->view->style_plugin->render_grouping_sets($sets);
    if (empty($grouping_options) && !empty($rows)) {
      $rows = $rows[''];
    }

    return $rows;
  }

}
