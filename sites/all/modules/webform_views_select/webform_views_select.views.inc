<?php

/**
 * @file
 * This file provides Views hooks implementations.
 */

/**
 * Implements hook_views_plugins().
 */
function webform_views_select_views_plugins() {
  return array(
    'display' => array(
      'webform_select' => array(
        'title' => t('Webform Options'),
        'help' => t("Create a list of options for a webform select component."),
        'handler' => 'WebformViewsSelectHandler',
        'theme' => 'views_view',
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'use more' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Webform Options'),
      ),
    ),
    'style' => array(
      'webform_views_select_style' => array(
        'title' => t('Webform Select List'),
        'help' => t('A predefined select list option set.'),
        'handler' => 'WebformViewsSelectStyle',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses row class' => FALSE,
        'uses grouping' => TRUE,
        'uses options' => TRUE,
        'even empty' => TRUE,
        'type' => 'webform_views_select',
        'theme' => NULL,
      ),
    ),
  );
}
