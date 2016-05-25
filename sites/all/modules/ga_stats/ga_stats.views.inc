<?php

/**
 * Implements hook_views_data().
 */
function ga_stats_views_data() {
  $data = array();

  $times = ga_stats_ga_timeframes(true);
  $metrics = ga_stats_ga_metrics();

  foreach($metrics as $mk => $mv) {
    // Google provides camelCase key values but views requires lowercase identifiers.
    $mkl = strtolower($mk); 

    foreach($times as $tk => $tv) {
      $data['ga_stats_count_'.$mkl.'_'.$tk]['table']['group']  = t('Statistics');
      $data['ga_stats_count_'.$mkl.'_'.$tk]['table']['join']  = array(
        'node' => array(
          'table' => 'ga_stats_count',
          'left_field' => 'nid',
          'field' => 'nid',
          'type' => 'LEFT OUTER',
          'extra' => 'ga_stats_count_'.$mkl.'_'.$tk.'.metric=\''.$mk.'\' AND ga_stats_count_'.$mkl.'_'.$tk.'.timeframe=\''.$tk.'\''
        )
      );
      $data['ga_stats_count_'.$mkl.'_'.$tk]['count'] = array(
        'title' => "$mv $tv",
        'help' => t('Information retrieved from Google Analytics'),
        'field' => array(
          'handler'=> 'views_handler_field_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        )
      );
    }
  }
   
  return $data;
}
