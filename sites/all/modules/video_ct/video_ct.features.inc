<?php
/**
 * @file
 * video_ct.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function video_ct_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function video_ct_node_info() {
  $items = array(
    'video_ct' => array(
      'name' => t('Video'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
