<?php

/**
 * function co_simulator_theme_cc().
 */
function co_simulator_theme_cc($data, $tac) {
	//Conditions
	$header_conditions = array(t('Conditions'), t('Total'));
	$table_conditions = co_simulator_theme_table($header_conditions, $data['data_conditions'], 'co-simulator-conditions');
	//proyeccion
	$table_uvr = array();
	$table_pesos = array();
	foreach($data['data'] as $key => $value) {
		$table_uvr[$key] = array(
			'no_cuota' => $value['no_cuota'],
			'uvr_cuota' => $value['uvr_cuota'],
			'uvr_intereses_corrientes' => $value['uvr_intereses_corrientes'],
			'uvr_abono_capital' => $value['uvr_abono_capital'],
			'uvr_saldo' => $value['uvr_saldo'],
		);
		$table_pesos[$key] = array(
			'pesos_cuota' => $value['pesos_cuota'],
			'pesos_interes_corriente' => $value['pesos_interes_corriente'],
			'pesos_abono_capital' => $value['pesos_abono_capital'],
			'pesos_saldo' => $value['pesos_saldo'],
			'pesos_cotizacion' => $value['pesos_cotizacion'],
		);
	}
	$header_main = array(t('In UVR'), t('In pesos'));
	$header_uvr = array(t('Payments number'), t('Payment value'), t('Current interest'), t('Capital payment'), t('Balance'));
	$header_pesos = array(t('Payment value'), t('Interes corriente'), t('Capital payment'), t('Balance'), t('Quote'));
	//create table values
	$theme_uvr = co_simulator_theme_table($header_uvr, $table_uvr, 'co-simulator-table-uvr-' . $tac);
	$theme_pesos = co_simulator_theme_table($header_pesos, $table_pesos, 'co-simulator-table-pesos-' . $tac);
	$table = co_simulator_theme_table($header_main, array(array('uvr' => $theme_uvr, 'pesos' => $theme_pesos)), 'co-simulator-table-' . $tac);
	return array('conditions' => $table_conditions, 'projection' => $table);
}

/**
 * function co_simulator_theme_cm().
 */
function co_simulator_theme_cm($data, $tac) {
	//Conditions
	$header_conditions = array(t('Conditions'), t('Total'));
	$table_conditions = co_simulator_theme_table($header_conditions, $data['data_conditions'], 'co-simulator-conditions');
	//proyeccion
	$table_uvr = array();
	$table_pesos = array();
	foreach($data['data'] as $key => $value) {
		$table_uvr[$key] = array(
			'no_cuota' => $value['no_cuota'],
			'uvr_cuota' => $value['uvr_cuota'],
			'uvr_intereses_corrientes' => $value['uvr_intereses_corrientes'],
			'uvr_abono_capital' => $value['uvr_abono_capital'],
			'uvr_saldo' => $value['uvr_saldo'],
		);
		$table_pesos[$key] = array(
			'pesos_cuota' => $value['pesos_cuota'],
			'pesos_interes_corriente' => $value['pesos_interes_corriente'],
			'pesos_abono_capital' => $value['pesos_abono_capital'],
			'pesos_saldo' => $value['pesos_saldo'],
			'pesos_cotizacion' => $value['pesos_cotizacion'],
		);
	}
	$header_main = array(t('UVR'), t('Pesos'));
	$header_uvr = array(t('Payments number'), t('Payment value'), t('Current interest'), t('Capital payment'), t('Balance'));
	$header_pesos = array(t('Payment value'), t('Interes corriente'), t('Capital payment'), t('Balance'), t('Quote'));
	//create table values
	$theme_uvr = co_simulator_theme_table($header_uvr, $table_uvr, 'co-simulator-table-uvr-' . $tac);
	$theme_pesos = co_simulator_theme_table($header_pesos, $table_pesos, 'co-simulator-table-pesos-' . $tac);
	$table = co_simulator_theme_table($header_main, array(array('uvr' => $theme_uvr, 'pesos' => $theme_pesos)), 'co-simulator-table-' . $tac);
	return array('conditions' => $table_conditions, 'projection' => $table);
}

/**
 * function co_simulator_theme_cb().
 */
function co_simulator_theme_cb($data, $tac) {
	//Conditions
	$header_conditions = array(t('Conditions'), t('Total'));
	$table_conditions = co_simulator_theme_table($header_conditions, $data['data_conditions'], 'co-simulator-conditions');
	//proyeccion
	$header = array(t('Payments number'), t('Payment value UVR'), t('Amortization in UVR'), t('Interest UVR'), t('Balance in UVR'), t('Payment in pesos'), t('Balance in pesos'), t('Quote UVR'));
	$table = co_simulator_theme_table($header, $data['data'], 'co-simulator-table-' . $tac);
	return array('conditions' => $table_conditions, 'projection' => $table);
}

/**
 * function co_simulator_theme_ctf().
 */
function co_simulator_theme_ctf($data, $tac) {
	//Conditions
	$header_conditions = array(t('Conditions'), t('Total'));
	$table_conditions = co_simulator_theme_table($header_conditions, $data['data_conditions'], 'co-simulator-conditions');
	//proyeccion
	$header = array(t('Payments number'), t('Interests'), t('Capital'), t('Payment'), t('Balance'));
	$table = co_simulator_theme_table($header, $data['data'], 'co-simulator-table-' . $tac);
	return array('conditions' => $table_conditions, 'projection' => $table);
}

/**
 * function co_simulator_theme_acc().
 */
function co_simulator_theme_acc($data, $tac) {
	//Conditions
	$header_conditions = array(t('Conditions'), t('Total'));
	$table_conditions = co_simulator_theme_table($header_conditions, $data['data_conditions'], 'co-simulator-conditions');
	//proyeccion
	$header = array(t('Payments number'), t('Payment value'), t('Current interest'), t('Capital payment'), t('Balance'));
	$table = co_simulator_theme_table($header, $data['data'], 'co-simulator-table-' . $tac);
	return array('conditions' => $table_conditions, 'projection' => $table);
}

/**
 * function co_simulator_theme_table().
 */
function co_simulator_theme_table($header, $row, $class) {
	return theme('table', array(
	  'header' => $header,
	  'rows' => $row,
	  'attributes' => array('class' => array($class)),
	));
}