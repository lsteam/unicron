INTRODUCTION
------------

This module will let you populate a webform select component with data from a
view. 


REQUIREMENTS
------------

This module requires the following modules:

 * Webform (https://www.drupal.org/project/webform)
 * Views (https://www.drupal.org/project/views)


INSTALLATION
------------

Install as you would normally install a contributed Drupal module. Visit:
https://drupal.org/documentation/install/modules-themes/modules-7 
for further information.


CONFIGURATION
-------------

The module has no menu or modifiable settings. There is no configuration.

Don't disable this module if there are webforms that use a select list from
this module, since it will break those webforms.


TROUBLESHOOTING
---------------

 * Creating the View
  - Create a new view
  - Create a "Webform Options" display
  - Select the Webform Select List format
  - Add the fields you want to use as key and value
  - Filter and sort as needed
  - In the format settings, select the field to use as key and value.

 * Using the View
  Add the select component to your webform and pick your view under "Load a 
  Pre-Built Option List".

 * Caveat
  Please keep in mind that the list that will loaded on demand from the view is
  not a hard coded list. When the webform is viewed, the component will get its
  data directly from the view.

  Also note that you can no longer see the item in the webform results when the
  item is removed from the View results.

 * Using view arguments in a multi-part webform
  Using a hook_form_alter, it is possible to change the option list to make use
  of view arguments. Here's an example on how to do that (thanks jonny5th!):

// Form alter to select options from views based on input
function MYMODULE_form_alter(&$form, &$form_state, $form_id) {
  if($form_id == 'webform_client_form_####'
      && isset($form_state['submitted'])
      && $form_state['submitted'] == TRUE
      && isset($form_state['webform_completed'])
      && $form_state['webform_completed'] == FALSE
      && $form_state['webform']['page_num'] == 2) {

    // Set the view and display.
    $view = 'your_view';
    $display = 'webform_select_1';

    // shorthand variable
    $submitted = $form_state['input']['submitted'];
    $options = array();
    // Check if input field on first page is filled in:
    if(isset($submitted['your_field']) && $submitted['your_field'] != "") {
      // Get data the way Views likes its arguments. $arguments must be an
      // array!
      $view_arguments = MYMODULE_get_view_args($submitted['your_field']);
      $args = array(
				'view' => $view,
				'display_id' => $display,
				'view_args' => $arguments
      );
      $options = webform_views_select_options(NULL, FALSE, $args);
    }

    // If there are no options returned uusing our views arguments above, use
    // this default argument:
    if(sizeof($options) < 1) {
      $args = array(
        'view' => $view,
        'display_id' => $display,
        'view_args' => array('all')
      );
      $options = webform_views_select_options(NULL, FALSE, $args);
    }

    // Set the #options array for our webform component.
    $form['submitted']['select_component']['#options'] = $options;
  }
}
