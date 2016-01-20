<?php

include_once(drupal_get_path('module','co_simulator') . '/inc/co_simulator.theme.inc');
include_once(drupal_get_path('module','co_simulator') . '/inc/co_simulator.admin.form.inc');

/**
 * function form co_simulator_calculator_form().
 */
function co_simulator_calculator_form($form, &$form_state) {
	$form['#attached'] = array(
		'css' => array(drupal_get_path('module', 'co_simulator') . '/css/co_simulator.css',),
		'js' => array(drupal_get_path('module', 'co_simulator') . '/js/co_simulator.js',),
	);
	$form['simulator_form'] = array(
		'#type' => 'container',
		'#weight' => 0,
		'#attributes' => array(
      'class' => array(
        'field-type-' . drupal_html_class('co-simulator-calculator-form'),
      ),
    ),
	);
	//Type credit amortization = tac
	$form['simulator_form']['tac'] = array(
		'#title' => t('Type credit amortization'),
    '#type' => 'select',
    '#options' => array(
    	'cc' => variable_get('co_simulator_amortization_title_cc'),
    	'cm' => variable_get('co_simulator_amortization_title_cm'),
    	'cb' => variable_get('co_simulator_amortization_title_cb'),
    	'ctf' => variable_get('co_simulator_amortization_title_ctf'),
    	'acc' => variable_get('co_simulator_amortization_title_acc'),
    ),
    '#default_value' => isset($_SESSION['co_simulator_user_tac']) ? $_SESSION['co_simulator_user_tac'] : 'cc',
	);
	//Credit in pesos = crp
	$form['simulator_form']['crp'] = array(
	  '#type' => 'textfield',
	  '#title' => t('Credit in pesos'),
	  '#default_value' => isset($_SESSION['co_simulator_user_crp']) ? number_format($_SESSION['co_simulator_user_crp'], 0, '.', ',') : '',
	  '#size' => 14,
	  '#maxlength' => 13,
	  '#required' => TRUE,
	  '#field_prefix' => '$',
	  '#attributes' => array('class' => array('field-simulator-crp')),
	);
	//Term in years = pla
	$form['simulator_form']['pla'] = array(
		'#title' => t('Term in years'),
    '#type' => 'select',
    '#options' => co_simulator_fixing_array_years(variable_get('co_simulator_variable_term_in_years', '5, 10, 12, 15')),
    '#default_value' => isset($_SESSION['co_simulator_user_pla']) ? $_SESSION['co_simulator_user_pla'] : variable_get('co_simulator_variable_pla', '5'),
	);
	$form['simulator_form']['submit'] = array(
		'#type' => 'submit',
		'#value' => variable_get('co_simulator_amortization_title_sumbmit', t('Calculate')),
	);
	//if is page simulator
	if (current_path() == 'simulator') {
		drupal_set_title(t('Credit Simulator'));
		$form['simulator_form']['submit']['#ajax'] = array(
      'callback' => 'co_simulator_calculator_form_callback',
      'wrapper' => 'co-simulator-calculator-result-html',
      'effect' => 'fade',
    );
	}
	else {//if is page block
		$form['#validate'] = array('co_simulator_calculator_block_form_validate');
		$form['#submit'] = array('co_simulator_calculator_block_form_submit');
	}
	/*$form['simulator_form']['link'] = array(
	  '#markup' => l(t('See information about mortgage credits'), variable_get('co_simulator_variable_siamc', '')),
	);*/

	$form['simulator_result'] = array(
		'#type' => 'container',
		'#weight' => 1,
		'#attributes' => array(
      'class' => array(
        'field-type-' . drupal_html_class('co-simulator-calculator-result'),
      ),
    ),
	);
	if (isset($_SESSION['co_simulator_user_crp']) && $_SESSION['co_simulator_user_tac'] && $_SESSION['co_simulator_user_pla'])
		$html = co_simulator_calcularor_custom_submit($_SESSION['co_simulator_user_crp'], $_SESSION['co_simulator_user_tac'], $_SESSION['co_simulator_user_pla']);
	else
		$html = '<div id="co-simulator-calculator-result-html"></div>';
	$form['simulator_result']['result'] = array(
		'#markup' => $html,
	);
	return $form;
}

/**
 * function validate co_simulator_calculator_form_callback().
 *
 * cc  = Cuota cíclica
 * cm  = Cuota media
 * cb  = Cuota baja
 * ctf = Cuota fija total
 * acc = Amortización constante a capital
 */
function co_simulator_calculator_form_callback($form, &$form_state) {
	$form_state['values']['crp'] = str_replace(',','',$form_state['values']['crp']);
	include_once(drupal_get_path('module','co_simulator') . '/inc/co_calculator.'.$form_state['values']['tac'] . '.inc');
	//Calculator
	$data = call_user_func_array('co_calculator_' . $form_state['values']['tac'], array($form_state['values']['crp'], $form_state['values']['pla']));
	//Create markup
	$markup = call_user_func_array('co_simulator_theme_' . $form_state['values']['tac'], array($data, $form_state['values']['tac']));
  $my_richtext_field = variable_get('co_simulator_variable_'.$form_state['values']['tac'].'_summary', '');
	return theme('co_simulator_page', array(
		'table_conditional' => $markup['conditions'],
    'summary' => $my_richtext_field['value'],
    'title_table' => t('Result of the projection'),
    'table_projection' => $markup['projection'],)
	);
}

/**
 * function validate co_simulator_calculator_block_form_validate().
 */
function co_simulator_calculator_block_form_validate($form, &$form_state) {
	$form_state['values']['crp'] = str_replace(',','',$form_state['values']['crp']);
	if (!is_numeric($form_state['values']['crp']))
		form_set_error('crp', t('Enter a correct value'));
}

/**
 * function submit co_simulator_calculator_block_form_submit().
 */
function co_simulator_calculator_block_form_submit($form, &$form_state) {
	$_SESSION['co_simulator_user_tac'] = $form_state['values']['tac']; 
	$_SESSION['co_simulator_user_crp'] = $form_state['values']['crp']; 
	$_SESSION['co_simulator_user_pla'] = $form_state['values']['pla'];
	drupal_goto('simulator');
}

/**
 * function co_simulator_calcularor_custom_submit(),
 */
function co_simulator_calcularor_custom_submit($crp, $tac, $pla) {
	include_once(drupal_get_path('module','co_simulator') . '/inc/co_calculator.' . $tac . '.inc');
	//Calculator
	$data = call_user_func_array('co_calculator_' . $tac, array($crp, $pla));
	//Create markup
	$markup = call_user_func_array('co_simulator_theme_' . $tac, array($data, $tac));
	unset($_SESSION['co_simulator_user_tac']);
	unset($_SESSION['co_simulator_user_crp']);
	unset($_SESSION['co_simulator_user_pla']);
	$my_richtext_field = variable_get('co_simulator_variable_'.$tac.'_summary', '');
	return theme('co_simulator_page', array(
		'table_conditional' => $markup['conditions'],
    'summary' => $my_richtext_field['value'],
    'title_table' => t('Result of the projection'),
    'table_projection' => $markup['projection'],)
	);
}