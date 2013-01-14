<?php

/**
 * Checks if a specific role reference is present.
 *
 * @param $variables
 *   An array containing each of the variables saved in the database necessary
 *   to evaluate this condition of the rule.
 * @return
 *   TRUE if condition applies successfully. Otherwise FALSE.
 */
function menu_position_rolereference_menu_position_condition_rolereference($variables) {
  // Check for the userreference field.
  if ($variables['context']['entity_type']) {

    // Grab the variables stored statically in the rule.
    $rid = $variables['rid'];

    // Determine what kind of entity page this is.
    $entity_type = $variables['context']['entity_type'];
    $bundle_name = $variables['context']['bundle_name'];
    $entity = $variables['context'][$entity_type];

    // Build a list of each role reference fields.
    $rr_fields = &drupal_static(__FUNCTION__, NULL);
    if (!isset($rr_fields)) {
      $rr_fields = array();
      $field_info = field_info_fields();
      foreach (array_keys(field_info_instances($entity_type, $bundle_name)) as $key) {
        if ($field_info[$key]['type'] == 'rolereference') {
          $rr_fields[$key] = $field_info[$key]['translatable'];
        }
      }
    }

    foreach ($rr_fields as $field => $translatable) {
      $language = $translatable ? $entity->language : LANGUAGE_NONE;
      if (!empty($entity->{$field}[$language])) {
        // Check for matching roles.
        foreach ($entity->{$field}[$language] as $role) {
          if ($role['rid'] == $rid) {
            return TRUE;
          }
        }

      }
    }
  }
  return FALSE;
}

/**
 * Adds form elements for the "rolereference" plugin to the rule configuration form.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array. New form elements should be
 *   added directly to this array.
 * @param $form_state
 *   A reference to the current form state.
 */
function menu_position_rolereference_menu_position_rule_rolereference_form(&$form, &$form_state) {
  // If this is an existing rule, load the variables stored in the rule for this plugin.
  $variables = !empty($form_state['#menu-position-rule']['conditions']['rolereference']) ? $form_state['#menu-position-rule']['conditions']['rolereference'] : array();

  $form['conditions']['rolereference'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role reference'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'menu_position_rolereference') . '/plugins/menu_position.rolereference.js'),
  ),
  );
  $form['conditions']['rolereference']['description'] = array(
    '#type' => 'item',
    '#description' => t('Apply this rule only on node that reference given role.'),
  );


  // Load the role references.
  $user_roles = user_roles();
  $user_roles_options = array();
  foreach ($user_roles as $key => $role) {
    $user_roles_options[$key] = $role;
  }
  $form['conditions']['rolereference']['rolereference_rid'] = array(
    '#type' => 'select',
    '#title' => t('Role'),
    '#default_value' => !empty($variables['rid']) ? $variables['rid'] : '0',
    '#options' => array('0' => t('- None -')) + $user_roles_options,
  );

  // Add a submit handler.
  $form['#submit'][] = 'menu_position_rolereference_menu_position_rule_rolereference_form_submit';
}

/**
 * Prepares the "rolereference" variables to be stored in the rule.
 *
 * @param $form
 *   A reference to the "add/edit rule" form array.
 * @param $form_state
 *   A reference to the current form state, including submitted values.
 */
function menu_position_rolereference_menu_position_rule_rolereference_form_submit(&$form, &$form_state) {
  if (!empty($form_state['values']['rolereference_rid'])) {
    $variables = array(
        'rid' => $form_state['values']['rolereference_rid'],
    );
    $form_state['values']['conditions']['rolereference'] = $variables;
  }
}