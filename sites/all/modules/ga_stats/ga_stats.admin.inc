<?php

/**
 * Callback for GA Stats authentication form.
 */
function ga_stats_auth_settings() {
  $form = array();

  $form['ga_stats_login'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Analytics Login Information'),
    '#collapsible' => TRUE,
    '#collapsed' => ga_stats_is_ready(),
  );

  $form['ga_stats_login']['ga_stats_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Account Email'),
    '#description' => t('The email account you use to log in to Google Analytics.'),
    '#default_value' => variable_get('ga_stats_email', ''),
  );

  $form['ga_stats_login']['ga_stats_password'] = array(
    '#type' => 'password',
    '#title' => t('Account Password'),
    '#description' => t('The password you use to log in to Google Analytics. This is not displayed once set.'),
  );

  $form['ga_stats_login']['ga_stats_acct_type'] = array(
    '#type' => 'select',
    '#title' => t('Account Type'),
    '#description' => t('The account type you use to log in to Google Analytics.'),
    '#options' => array(
      'GOOGLE' => t('Google'),
      'HOSTED' => t('Hosted'),
    ),
    '#default_value' => variable_get('ga_stats_acct_type', NULL),
  );

  $form['#validate'][] = 'ga_stats_auth_form_validate';

  return system_settings_form($form);
}


/**
 * Callback for the GA Stats admin form.
 */
function ga_stats_admin_settings() {
  $form = array();

  $form['ga_stats_accounts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google Analytics Tracking Accounts'),
    '#collapsible' => TRUE,
  );

  if (!ga_stats_is_ready()) {
    drupal_set_message(t('Your Google Analytics account credentials are not set. Visit the !authentication to configure them.',
      array('!authentication' => l(t('authentication tab'), 'admin/config/services/ga_stats/auth'))), 'error');
    $form['#disabled'] = TRUE;
    $accounts = FALSE;
  }
  else {
    require_once 'includes/ga.inc';
    // @todo expand on the error cases here.
    $accounts = ga_stats_ga_get_accounts(ga_stats_get_client());
  }

  $options = array();
  if (!empty($accounts) && is_array($accounts)) {
    foreach ($accounts as $id => $value) {
      $acc = $value->getProperties();
      $options[$acc['profileId']] = "{$acc['title']} ({$acc['webPropertyId']})";
    }

    $form['ga_stats_accounts']['ga_stats_profile'] = array(
      '#type' => 'select',
      '#title' => t('Google Analytics Profile to Use'),
      '#description' => t('The Google Analytics profile from which to retrieve statistics'),
      '#options' => $options,
      '#default_value' => variable_get('ga_stats_profile', ''),
    );
  }
  else {
    $form['ga_stats_accounts']['ga_stats_profile'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="messages warning">'
        . t('Cannot select a tracking account until your Google Analytics !configured.', array('!configured' => l(t('account is configured'), 'admin/config/services/ga_stats/auth')))
        . '</div>'
    );
  }

  $form['enabled_stats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Enabled Statistics'),
    '#description' => t('Make sure to clear the Drupal cache after changing this setting in order to inform Views of the new settings. <br/><em><b>WARNING:</b> Do not disable a setting which is currently in use in Views.</em>')
  );

  $form['enabled_stats']['ga_stats_enabled_metrics'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('ga_stats_enabled_metrics', array('pageviews')),
    '#options' => ga_stats_ga_metrics(TRUE),
    '#title' => t('Metrics'),
    '#description' => t('The metrics that will be available from Google Analytics in Views.'),
  );

  $form['enabled_stats']['ga_stats_enabled_timeframes'] = array(
    '#type' => 'checkboxes',
    '#default_value' => variable_get('ga_stats_enabled_timeframes', array('today', 'month')),
    '#options' => ga_stats_ga_timeframes(TRUE, TRUE),
    '#title' => t('Time Frames'),
    '#description' => t('The timeframes that will be available from Google Analytics in Views.'),
  );

  $form['ga_stats_max_results'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Results per Metric/Timeframe'),
    '#description' => t('The max results that a call (a metric/timeframe combination) can return. MUST be a number.'),
    '#default_value' => variable_get('ga_stats_max_results', '100'),
  );

  if (variable_get('ga_stats_profile', FALSE)) {
    $form['actions']['ga_stats_update'] = array(
      '#type' => 'button',
      '#value' => t('Update Counts'),
      '#weight' => 20,
      '#executes_submit_callback' => TRUE,
      '#submit' => array('ga_stats_update_counts'),
    );
  }

  return system_settings_form($form);
}

/**
 *  Just set the password if we already have one and the form field was ignored.
 */
function ga_stats_auth_form_validate($form, &$form_state) {
  if (!$form_state['values']['ga_stats_password']) {
    $form_state['values']['ga_stats_password'] = variable_get('ga_stats_password', '');
  }
}

/**
 * Submit callback to update the statistics data.
 */
function ga_stats_update_counts_submit($form, &$form_state) {
  if (ga_stats_update_counts()) {
    drupal_set_message(t('Successfully retrieved Analytics data.'));
    drupal_set_message(t('The next scheduled retrieval is on !date', array(
      '!date' => date('r', ga_stats_data_expiration_date()),
    )));
  }
}
