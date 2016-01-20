<?php
/* ctf  = 'Cuota fija total'
 *
 * Todos los valores divididos sobre 100 son variables tipo %
 */

/**
 * function co_calculator_ctf().
 */
function co_calculator_ctf($CRP, $PLA) {
	//Plazo en meses
	$PLM = $PLA * 12;
	//Tasa efectiva anual
	$TEA = variable_get('co_simulator_variable_cft_tea'); //administration
	//Tasa mensual vencida
	$TMV = (pow((1+($TEA/100)),(1/12))-1)*100; //((1+[TEA])^(1/12))-1
	//Cerrar Crédito si falta # Pesos
	$CCSFP = variable_get('co_simulator_variable_cft_ccsfp'); //administration
	$proyeccion = array();
	$proyeccion_format = array();
	$proyeccion[] = array(
		'intereses' => 0,
		'capital' => 0,
		'cuota' => 0,
		'saldo' => $CRP,
	);
	$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, 0);
	$saldo = $CRP;
	//Condición para cerrar credito
	while ($saldo >= 0.2) {
		$key = count($proyeccion)-1;
		//Declaración de variables
		$intereses = 0;
		$capital = 0;
		$cuota = 0;
		$saldo = 0;
		//Calculo cuota
		if ($proyeccion[$key]['saldo'] > $CCSFP)
			//$cuota = round(($CRP * ($TMV/100)) / (1 - (pow((1 + ($TMV/100)), -$PLM))), 0);
			$cuota = ($CRP * ($TMV/100)) / (1 - (pow((1 + ($TMV/100)), -$PLM)));
		//Intereses - round(($proyeccion[$key]['saldo'] * ($TMV/100)),0);
		$intereses = ($proyeccion[$key]['saldo'] * ($TMV/100));
		//Calculo Capital
		if ($proyeccion[$key]['saldo'] > $CCSFP)
			//$capital - round(($cuota - $intereses), 0);
			$capital = ($cuota - $intereses);
		//$saldo - round(($proyeccion[$key]['saldo'] - $capital), 0);
		$saldo = ($proyeccion[$key]['saldo'] - $capital);
		$proyeccion[] = array(
			'intereses' => $intereses,
			'capital' => $capital,
			'cuota' => $cuota,
			'saldo' => $saldo,
		);
		$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, $key+1);
	}
	//Ingresos minimos libres mes
	$IMLM = (ceil((($proyeccion[1]['cuota']/0.3)/1000))*1000); //Información para el usuario
	//Conditions
	$row_conditions = array();
	$row_conditions[] = array(t('Crédito en pesos (CRP)'), '$ '.number_format(round($CRP, 0), 0, '.', ','),);
	$row_conditions[] = array(t('Plazo en años (PLA)'), $PLA,);
	$row_conditions[] = array(t('Plazo en meses (PLM)'), $PLM,);
	$row_conditions[] = array(t('Tasa Efectiva Anual (TEA)'), $TEA .'%',);
	$row_conditions[] = array(t('Tasa Mensual Vencido (TMV)'), round($TMV, 2) .'%',);
	$row_conditions[] = array(t('Ingresos Minímos Libres Mes'), '$ '.number_format($IMLM, 0, '.', ','),);
	$row_conditions[] = array(t('Cerrar Crédito si falta # Pesos'), $CCSFP,);
	return array('data' => $proyeccion_format, 'data_conditions' => $row_conditions, 'IMLM' => $IMLM);
}

/**
 * function co_calculator_cm_format()
 *
 * ROUND values
 **/
function co_calculator_cm_format($proyeccion, $proyeccion_format, $key) {
	$proyeccion_format[$key] = array(
		'no_cuota' => $key,
		'intereses' => '$ '.number_format(round($proyeccion[$key]['intereses'], 0), 0, '.', ','),
		'capital' => '$ '.number_format(round($proyeccion[$key]['capital'], 0), 0, '.', ','),
		'cuota' => '$ '.number_format(round($proyeccion[$key]['cuota'], 0), 0, '.', ','),
		'saldo' => '$ '.number_format(round($proyeccion[$key]['saldo'], 0), 0, '.', ','),
	);
	return $proyeccion_format;
}