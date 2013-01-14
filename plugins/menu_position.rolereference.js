(function ($) {

/**
 * Provide the summary information for the taxonomy plugin's vertical tab.
 */
Drupal.behaviors.menuPositionRolereference = {
  attach: function (context) {
    $('fieldset#edit-rolereference', context).drupalSetSummary(function (context) {
      if ($('input[name="rolereference"]', context).val()) {
        return Drupal.t('Rolerefernece: %rolereference', {'%rolereference' : $('input[name="rolereference"]', context).val()});
      }
      else if ($('select[name="rolereference_rid"]', context).val() != 0) {
        return Drupal.t('Role: %role', {'%role' : $('select[name="rolereference_rid"] option:selected', context).text()});
      }
      else {
        return Drupal.t('Any role');
      }
    });
  }
};

})(jQuery);