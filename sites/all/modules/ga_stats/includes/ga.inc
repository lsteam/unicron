<?php

/**
 * Goes through sources and metrics and updates databases
 *
 * Views cannot tell the difference between the various metrics and timeframes
 * so we delete all counts before rebuilding.
 *
 * @return int|bool
 */
function _ga_stats_update_counts() {
  $client = ga_stats_get_client();
  if (isset($client)) {
    $data = ga_stats_collect_data($client);
  }

  // Only wipe the data if we retrieved something to replace it with.
  if (!empty($data)) {
    db_query('DELETE FROM {ga_stats_count}');
    foreach ($data as $record) {
      ga_stats_write_count($record);
    }

    drupal_set_message(t('Counts Successfully Updated'));
    watchdog('ga_stats', 'Updated statistics for !count records.', array('!count' => count($data)));

    // Only schedule a new update if this was successful. If it failed, we should try to run a new
    // update in the next available opportunity. Request flooding is not the purpose of the scheduler.
    return ga_stats_schedule_update();
  }

  return FALSE;
}

/**
 * Retrieve data for all configured metrics and time-frames.
 *
 * @param gapi $client
 *
 * @return array
 */
function ga_stats_collect_data($client) {
  $data = array();
  $current = time();

  foreach (ga_stats_ga_metrics() as $metric => $title) {
    foreach (ga_stats_ga_timeframes() as $key => $timeframe) {
      $filter = isset($timeframe['filter']) ? $timeframe['filter'] : NULL;
      $results = ga_stats_ga_data($client, $metric, $current - $timeframe['secsToSub'], $current);
      $data = array_merge($data, ga_stats_transform_data($results, $metric, $key));
    }
  }

  return $data;
}

/**
 * Transform data from the retrieved structure to one ready for Drupal storage.
 *
 * @param array $results
 *   Set of results retrieved from GA.
 * @param array|string $metric
 *   An array or string of the metrics to pull.
 * @param $timeframe
 *   Identifier for a predetermine timeframe.
 *
 * @return array
 *   An array of obj ready for the ga_stats_count table
 */
function ga_stats_transform_data($results, $metric, $timeframe = '') {
  $data = array();

  foreach ($results as $record) {
    $item = new stdClass;
    $item->url = $record['url'];
    $item->count = $record[$metric];
    $item->metric = $metric;
    $item->nid = FALSE;
    $item->timeframe = $timeframe;
    $item->nid = ga_stats_nid_from_url($record['url']);

    // Only log nodes.
    if ($item->nid) {
      $data[] = $item;
    }
  }

  return $data;
}

/**
 * Attempts to determine a node nid based on the provided URL.
 *
 * @param string $url
 *
 * @return int|bool
 */
function ga_stats_nid_from_url($url) {
  $alias = preg_replace('/^\//', '', $url);
  if (!preg_match('/^node\/([0-9]*)/', $alias, $matches) ) {
    $alias = drupal_lookup_path('source', $alias);
  }
  if (preg_match('/^node\/([0-9]*)/', $alias, $matches)  ) {
    return $matches[1];
  }

  return FALSE;
}

/**
 * Write the data associated with a specific page to the database.
 *
 * @param object $count
 */
function ga_stats_write_count($count) {
  drupal_write_record('ga_stats_count', $count);
}

/**
 *  Use the client library to query GA for fresh data.
 *
 *  @param gapi $client
 *  @param array|string metrics
 *    The metrics to retrieve.
 *  @param int start_date
 *    Beginning of the time range.
 *  @param int end_date
 *    End of the time range.
 *  @param $filter
 *
 *  @return array
 *    Retrieve an array of "external_statistics_count" objects.
 */
function ga_stats_ga_data($client, $metrics, $start_date = 0, $end_date = 0, $filter = FALSE) {
  $url_dim = 'pagePath';
  if (!is_array($metrics)) {
   $metrics = array($metrics);
  }
  $request['dimensions'] = array($url_dim);
  $request['metrics'] = $metrics;
  
  $request['start_date'] = $start_date ? date('Y-m-d', $start_date) : NULL;
  $request['end_date'] = $end_date ? date('Y-m-d', $end_date) : NULL;

  $request['sort_metric'] = "-" . $metrics[0];
  $request['max_results'] = variable_get('ga_stats_max_results', "100");
  
  if ($filter) {
    $request['filter'] = $filter;
  }

  $data_raw = ga_stats_query_data($client, $request);

  // @todo move "data array" stuff up the callstack so this message can follow.
  if (empty($data_raw)) {
    drupal_set_message(t('Failed to retrieve data from Google Analytics.'), 'error', FALSE);
  }

  $data_array = ga_stats_ga_data_array($data_raw);
  foreach ($data_array as $k => $d) {
    $data_array[$k]['url'] = $d[$url_dim];
  }
  
  return $data_array;
}

/**
 * Preprocess the retrieved data.
 *
 * This should be merged into the other transform function so we only do one
 * rearrangement.
 *
 * @param $data_in
 *
 * @return array
 */
function ga_stats_ga_data_array($data_in) {
  $data_all = array();

  foreach ($data_in as $d) {
    $metrics = $d->getMetrics();
    $dimensions = $d->getDimensions();
    $data_all[] = array_merge($metrics, $dimensions);
  }
  return $data_all;
}

/**
 * GA client library factory.
 *
 * @param $suppress
 *   If set to TRUE, will not output error results to the UI.
 *
 * @return gapi
 */
function ga_stats_get_client($suppress = FALSE) {
  require_once 'gapi.class.php';

  $email = variable_get('ga_stats_email', '');
  $password = variable_get('ga_stats_password', '');
  $type = variable_get('ga_stats_acct_type', NULL);

  $client = NULL;
  if (ga_stats_is_ready()) {
    try {
      $client = new gapi($email, $password, NULL, $type);
    }
    catch (Exception $e) {
      $error = t('Invalid Google Analytics authentication attempt.');
      watchdog('ga_stats', $e->getMessage(), array(), WATCHDOG_ERROR);
    }
  }
  else {
    $error = t('Google Analytics Email and password not set.');
    watchdog('ga_stats', 'Google Analytics email and password not set.', array(), WATCHDOG_ERROR);
  }

  if (!$suppress && isset($error)) {
    drupal_set_message($error, 'error');
  }
  if (!isset($error)) {
    watchdog('ga_stats', 'Successfully authenticated client with Google Analytics.');
  }

  return $client;
}

/**
 * Collect GA account for UI display.
 *
 * @param gapi $client
 *
 * @return array
 */
function ga_stats_ga_get_accounts($client) {
  if (!isset($client)) {
    return array();
  }

  try{
    $accounts = $client->requestAccountData(1, 5000);
  }
  catch (Exception $e) {
    drupal_set_message(t('Could not retrieve accounts from Google Analytics.'), 'error');
    watchdog('ga_stats', $e->getMessage(), array(), WATCHDOG_ERROR);

    return array();
  }

  return $accounts;
}

/**
 *  Query GA for a particular dataset.
 *
 *  @param gapi $client
 *  @param array $request
 *    - report_id
 *    - dimensions
 *    - metrics
 *    - sort_metric=null
 *    - filter=null
 *    - start_date=null
 *    - end_date=null
 *    - start_index=1
 *    - max_results=30
 *
 *  @return array
 */
function ga_stats_query_data($client, $request) {
  try {
    $data = $client->requestReportData(
      variable_get('ga_stats_profile', ''),  // aid
      $request['dimensions'],
      $request['metrics'],
      $request['sort_metric'],
      NULL,
      $request['start_date'],
      $request['end_date'],
      1,
      $request['max_results']
    );
  }
  catch (Exception $e) {
    watchdog('ga_stats', $e->getMessage(), array(), WATCHDOG_ERROR);

    return array();
  }

  if(empty($data)) {
    // At this point in the call stack, $request['metrics'] will have only one item.
    watchdog('ga_stats', 'Successfully contacted Google Analytics, but an empty dataset was found for "!metric" between "!start" and "!end".',
      array(
        '!metric' => reset($request['metrics']),
        '!start' => $request['start_date'],
        '!end' => $request['end_date'],
      ), WATCHDOG_WARNING
    );
  }
  else {
    watchdog('ga_stats', 'Successfully retrieved data from Google Analytics.');
  }

  return $data;
}
