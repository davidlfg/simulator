<?php
/* acc  = 'Amortización constante a capital'
 *
 * Todos los valores divididos sobre 100 son variables tipo %
 */

/**
 * function co_calculator_acc().
 */
function co_calculator_acc($CRP, $PLA) {
	//Plazo en meses
	$PLM = $PLA * 12;
	//Tasa efectiva anual
	$TEA = variable_get('co_simulator_variable_acc_tea'); //Parametrizable por el administrador
	//Tasa mensual vencida
	$TMV = (pow((1+($TEA/100)),(1/12))-1)*100; //=((1+[TEA])^(1/12))-1
	//Cerrar Crédito si falta # Pesos
	$CCSFP = variable_get('co_simulator_variable_acc_ccsfp'); //Parametrizable por administrador
	$proyeccion = array();
	$proyeccion_format = array();
	$proyeccion[] = array(
		'cuota' => 0,
		'intereses' => 0,
		'capital' => 0,
		'saldo' => $CRP,
	);
	$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, 0);
	$saldo = $CRP;
	//Condición para cerrar credito
	while ($saldo >= 0.2) {
		$key = count($proyeccion)-1;
		$cuota = 0;
		$intereses = 0;
		$capital = 0;
		$saldo = 0;
		//Interes
		$intereses = ($proyeccion[$key]['saldo'] * ($TMV/100));
		if ($proyeccion[$key]['saldo'] > $CCSFP) {
			//Capital -IF(G16>$D$10,$G$16/$D$6,0)
			$capital = ($CRP / $PLM);
			//Saldo - IF(G16>$D$10,G16-F17,0)
			$saldo = ($proyeccion[$key]['saldo'] - $capital);
		}
		//Cuota - =+E17+F17
		$cuota = $intereses + $capital;
		//Mapeo
		$proyeccion[] = array(
			'cuota' => $cuota,
			'intereses' => $intereses,
			'capital' => $capital,
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
		'cuota' => '$ '.number_format(round($proyeccion[$key]['cuota'], 0), 0, '.', ','),
		'intereses' => '$ '.number_format(round($proyeccion[$key]['intereses'], 0), 0, '.', ','),
		'capital' => '$ '.number_format(round($proyeccion[$key]['capital'], 0), 0, '.', ','),
		'saldo' => '$ '.number_format(round($proyeccion[$key]['saldo'], 0), 0, '.', ','),
	);
	return $proyeccion_format;
}