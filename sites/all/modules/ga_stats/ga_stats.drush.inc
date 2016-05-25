<?php
/**
 * Provides drush integration for GA Stats
 */

/**
 * Implements hook_drush_command().
 */
function ga_stats_drush_command() {
  $items = array();

  $items['ga-stats-update'] = array(
    'description' => 'Manually trigger a retrieval of Google Analytics data.',
  );
  $items['ga-stats-generate'] = array(
    'description' => 'Generate "scraped" Google Analytics statistics.',
    'options' => array(
      'metric' => 'Specify the GA metric to populate. Defaults to "pageviews".',
      'timeframe' => 'Special the time range from running the command backwards. Defaults to "monthly".',
      'range' => 'Specify the upper limit for statistics counts. Defaults to 1000.'
    ),
  );

  return $items;
}

/**
 * Command callback for ga-stats-update.
 */
function drush_ga_stats_update() {
  if (ga_stats_update_counts()) {
    drush_log(dt('Retrieved fresh Google Analytics data. Next scheduled retrieval will occur after !time.',
      array('!time' => date('r', ga_stats_data_expiration_date()))), 'success');
  }
}

/**
 *  Implements hook_drush_COMMAND_validate().
 */
function drush_ga_stats_generate_validate() {
  module_load_include('.inc', 'ga_stats', 'inc/ga');

  $metric = drush_get_option('metric', 'pageviews');
  $options = ga_stats_ga_metrics(TRUE);
  if (!array_key_exists($metric, $options)) {
    drush_set_option('metric', drush_choice($options));
  }

  $timeframe = drush_get_option('timeframe', 'month');
  $options = ga_stats_ga_timeframes(FALSE, TRUE);
  if (!array_key_exists($timeframe, $options)) {
    drush_set_option('timeframe', drush_choice($options));
  }

  $range = drush_get_option('range', 1000);
  if (!is_numeric($range) || $range < 1) {
    return drush_set_error('Range must be an integer greater than 1.');
  }
}

/**
 * Command callback for ga-stats-generate.
 */
function drush_ga_stats_generate() {
  $metric = drush_get_option('metric', 'pageviews');
  $timeframe = drush_get_option('timeframe', 'month');
  drush_log(dt('Generating data for metric "!metric" and timeframe "!timeframe".',
    array('!metric' => $metric, '!timeframe' => $timeframe)));

  if (drush_ga_stats_randomize($metric, $timeframe)) {
    drush_log('Google Analytics Statistics have been randomly generated.', 'success');
  }
}

/**
 * Generate a new dataset using randomized counts.
 */
function drush_ga_stats_randomize($metric, $timeframe) {
  global $base_url;

  // Purge all existing data. It is not additive.
  db_query('DELETE FROM {ga_stats_count}');

  // Respect configured limit.
  $max = variable_get('ga_stats_max_results', 100);
  $limit = is_numeric($max) ? " LIMIT $max" : '';
  // For every published node, generate a count with the specified qualifiers.
  $sql = "INSERT INTO {ga_stats_count} (nid, url, metric, timeframe, count)"
    . "(SELECT nid, CONCAT('{$base_url}/node/', nid) AS url, 'pageviews' AS metric, 'month' AS timeframe,"
    . "FLOOR(1 + (RAND(nid+created+CURTIME()) * :range)) AS count FROM node WHERE status = 1$limit)";

  return db_query($sql, array(
    ':range' => drush_get_option('range', 1000) - 1,
  ));
}
