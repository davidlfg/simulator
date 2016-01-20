<?php
	
/**
 * function form co_simulator_settings_general_form().
 */
function co_simulator_settings_general_form($form, $form_state) {
	$form['co_simulator_variable_term_in_years'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Term in years'),
	  '#description' => t('Years must be separated by commas. Example: 5, 10, 12, 15'),
	  '#default_value' => variable_get('co_simulator_variable_term_in_years', '5, 10, 12, 15'),
	  '#required' => TRUE,
	);
	$form['co_simulator_variable_pla'] = array(
		'#title' => t('Plazo en años (PLA) por defecto'),
		'#description' => t('La opción seleccionada aparecerá como valor por defecto en el simulador.'),
    '#type' => 'radios',
    '#options' => co_simulator_fixing_array_years(variable_get('co_simulator_variable_term_in_years', '5, 10, 12, 15')),
    '#default_value' => variable_get('co_simulator_variable_pla', '5'),
	);
	$form['co_simulator_variable_uvr'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Cotización Inicial en UVR (UVR)'),
	  '#description' => t('Administración manual del UVR del Banco de la Republica (www.banrep.gov.co)'),
	  '#default_value' => variable_get('co_simulator_variable_uvr', ''),
	  '#size' => 8, 
	  '#maxlength' => 8, 
	  '#required' => TRUE,
	  '#field_prefix' => '$',
	);
	//Labels select amortization
	$form['amortization_labels'] = array(
		'#type' => 'fieldset', 
		'#title' => t('Titles for labels amortization'), 
		'#weight' => 5, 
		'#collapsible' => TRUE, 
		'#collapsed' => FALSE,
	);
	//label cuota cíclica
	$form['amortization_labels']['co_simulator_amortization_title_cc'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Cuota cíclica (Decreciente mensual por años)'),
	  '#default_value' => variable_get('co_simulator_amortization_title_cc', t('Cuota cíclica')),
	  '#required' => TRUE,
	);
	//label cuota media
	$form['amortization_labels']['co_simulator_amortization_title_cm'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Cuota media (Abono constante a capital en UVR)'),
	  '#default_value' => variable_get('co_simulator_amortization_title_cm', t('Cuota media')),
	  '#required' => TRUE,
	);
	//label cuota baja
	$form['amortization_labels']['co_simulator_amortization_title_cb'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Cuota baja (Cuota constante en UVR)'),
	  '#default_value' => variable_get('co_simulator_amortization_title_cb', t('Cuota baja')),
	  '#required' => TRUE,
	);
	//label cuota total fija
	$form['amortization_labels']['co_simulator_amortization_title_ctf'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Cuota total fija en pesos'),
	  '#default_value' => variable_get('co_simulator_amortization_title_ctf', t('Cuota total fija')),
	  '#required' => TRUE,
	);
	//label Amortización constante a capital
	$form['amortization_labels']['co_simulator_amortization_title_acc'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Amortización constante a capital en pesos'),
	  '#default_value' => variable_get('co_simulator_amortization_title_acc', t('Amortización constante a capital')),
	  '#required' => TRUE,
	);
	//label Amortización constante a capital
	$form['amortization_labels']['co_simulator_amortization_title_sumbmit'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Submit'),
	  '#default_value' => variable_get('co_simulator_amortization_title_sumbmit', t('Calculate')),
	  '#required' => TRUE,
	);
	$form['co_simulator_variable_siamc'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Redirect - See information about mortgage credits'),
	  '#default_value' => variable_get('co_simulator_variable_siamc', ''),
	  '#required' => TRUE,
	);
	return system_settings_form($form);
}

/**
 * function form co_simulator_settings_cuota_ciclica_form().
 */
function co_simulator_settings_cuota_ciclica_form($form, $form_state) {
	drupal_set_title(t('Configuración - Cuota cíclica (Decreciente mensual por años).'));
	//cuota ciclica = cc
	$form['co_simulator_variable_cc_tea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Tasa Efectiva Anual (TEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cc_tea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$form['co_simulator_variable_cc_ccsfp'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Cerrar Crédito si falta # UVR'), 
	  '#default_value' => variable_get('co_simulator_variable_cc_ccsfp', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	);
	$form['co_simulator_variable_cc_iea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Inflación Esperada Anual (IEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cc_iea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$defaults = array(
    'value' => '',
    'format' => filter_default_format(),
  );
  $my_richtext_field = variable_get('co_simulator_variable_cc_summary', $defaults);
	$form['co_simulator_variable_cc_summary'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#default_value' => $my_richtext_field['value'],
    '#format' => $my_richtext_field['format'],
  );
	return system_settings_form($form);
}

/**
 * function form co_simulator_settings_cuota_media_form().
 */
function co_simulator_settings_cuota_media_form($form, $form_state) {
	drupal_set_title(t('Configuración - Cuota media (Abono constante a capital en UVR).'));
	//cuota media = cm
	$form['co_simulator_variable_cm_tea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Tasa Efectiva Anual (TEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cm_tea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$form['co_simulator_variable_cm_ccsfp'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Cerrar Crédito si falta # UVR'), 
	  '#default_value' => variable_get('co_simulator_variable_cm_ccsfp', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	);
	$form['co_simulator_variable_cm_iea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Inflación Esperada Anual (IEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cm_iea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$defaults = array(
    'value' => '',
    'format' => filter_default_format(),
  );
  $my_richtext_field = variable_get('co_simulator_variable_cm_summary', $defaults);
	$form['co_simulator_variable_cm_summary'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#default_value' => $my_richtext_field['value'],
    '#format' => $my_richtext_field['format'],
  );
	return system_settings_form($form);
}

/**
 * function form co_simulator_settings_cuota_baja_form().
 */
function co_simulator_settings_cuota_baja_form($form, $form_state) {
	drupal_set_title(t('Configuración - Cuota baja (Cuota constante en UVR).'));
	//cuota baja = cb
	$form['co_simulator_variable_cb_tea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Tasa Efectiva Anual (TEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cb_tea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$form['co_simulator_variable_cb_ccsfp'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Cerrar Crédito si falta # UVR'), 
	  '#default_value' => variable_get('co_simulator_variable_cb_ccsfp', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	);
	$form['co_simulator_variable_cb_iea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Inflación Esperada Anual (IEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cb_iea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$defaults = array(
    'value' => '',
    'format' => filter_default_format(),
  );
  $my_richtext_field = variable_get('co_simulator_variable_cb_summary', $defaults);
	$form['co_simulator_variable_cb_summary'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#default_value' => $my_richtext_field['value'],
    '#format' => $my_richtext_field['format'],
  );
	return system_settings_form($form);
}

/**
 * function form co_simulator_settings_cuota_total_fija_form().
 */
function co_simulator_settings_cuota_total_fija_form($form, $form_state) {
	drupal_set_title(t('Configuración - Cuota total fija en pesos.'));
	//cuota fija total = cft
	$form['co_simulator_variable_cft_tea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Tasa Efectiva Anual (TEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_cft_tea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$form['co_simulator_variable_cft_ccsfp'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Cerrar Crédito si falta # Pesos'), 
	  '#default_value' => variable_get('co_simulator_variable_cft_ccsfp', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	);
	$defaults = array(
    'value' => '',
    'format' => filter_default_format(),
  );
  $my_richtext_field = variable_get('co_simulator_variable_ctf_summary', $defaults);
	$form['co_simulator_variable_ctf_summary'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#default_value' => $my_richtext_field['value'],
    '#format' => $my_richtext_field['format'],
  );
	return system_settings_form($form);
}

/**
 * function form co_simulator_settings_amortizacion_constante_capital_form().
 */
function co_simulator_settings_amortizacion_constante_capital_form($form, $form_state) {
	drupal_set_title(t('Configuración - Amortización constante a capital en pesos.'));
	//amortización constante a capital = acc
	$form['co_simulator_variable_acc_tea'] = array(
	  '#type' => 'textfield', 
	  '#title' => t('Tasa Efectiva Anual (TEA)'), 
	  '#default_value' => variable_get('co_simulator_variable_acc_tea', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	  '#field_suffix' => '%',
	);
	$form['co_simulator_variable_acc_ccsfp'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Cerrar Crédito si falta # Pesos'), 
	  '#default_value' => variable_get('co_simulator_variable_acc_ccsfp', ''),
	  '#size' => 6, 
	  '#maxlength' => 6, 
	  '#required' => TRUE,
	);
	$defaults = array(
    'value' => '',
    'format' => filter_default_format(),
  );
  $my_richtext_field = variable_get('co_simulator_variable_acc_summary', $defaults);
	$form['co_simulator_variable_acc_summary'] = array(
    '#type' => 'text_format',
    '#base_type' => 'textarea',
    '#default_value' => $my_richtext_field['value'],
    '#format' => $my_richtext_field['format'],
  );
	return system_settings_form($form);
}

/**
 * Function co_simulator_fixing_array_years
 */
function co_simulator_fixing_array_years($years) {
	$years = explode(",", $years);
	$options = array();
	foreach ($years as $key => $year) {
		if (is_numeric($year))
			$options[$year] = $year;
	}
	return $options;
}