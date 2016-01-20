<?php
/* cb = 'Cuota baja'
 *
 * Todos los valores divididos sobre 100 son variables tipo %
 */

/**
 * function co_calculator_cb().
 */
function co_calculator_cb($CRP, $PLA) {
	//Plazo en meses
	$PLM = $PLA * 12;
	//Cotización Inicial en UVR (UVR)
	$UVR = variable_get('co_simulator_variable_uvr'); //UVR del Banco de la Republica (www.banrep.gov.co)
	//Crédito en UVR (CRU)
	$CRU = $CRP / $UVR;
	//Tasa efectiva anual
	$TEA = variable_get('co_simulator_variable_cb_tea'); //Parametrizable por el administrador
	//Tasa mensual vencida - ((1+[TEA])^(1/12))-1s
	$TMV = (pow((1+($TEA/100)),(1/12))-1);
	//Inflación Esperada Anual (IEA) - Parametrizable por administrador
	$IEA = variable_get('co_simulator_variable_cb_iea');
	//Inflación Mensual Vencido (IEV)
	$IEV = (pow((1+($IEA) / 100), (1/12))-1)*100;//((1+D12)^(1/12))-1
	//Cerrar Crédito si falta # UVR
	$CCSFU = variable_get('co_simulator_variable_cb_ccsfp');//Parametrizable por administrador
	$proyeccion = array();
	$proyeccion_format = array();
	$proyeccion[] = array(
		'cuota_uvr' => 0,
		'amortizacion_uvr' => 0,
		'interes_uvr' => 0,
		'saldo_uvr' => ($CRP/$UVR),
		'cuota_pesos' => 0,
		'saldo_pesos' => $CRP,
		'cotizacion_uvr' => $UVR,
	);
	$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, 0);
	$saldo_pesos = $CRP;
	//Condición para cerrar credito
	while ($saldo_pesos >= $CCSFU) {
		$key = count($proyeccion)-1;
		//Declaración de variables
		$cuota_uvr = 0;
		$amortizacion_uvr = 0;
		$interes_uvr = 0;
		$saldo_uvr = 0;
		$cuota_pesos = 0;
		$saldo_pesos = 0;
		$cotizacion_uvr = 0;
		//Interes UVR
		if ($proyeccion[$key]['saldo_uvr'] > $CCSFU) {
			if ($key == 0)
				$interes_uvr = $CRU * $TMV;//IF(G22>$D$15,D9*$D$11,0)
			else
				$interes_uvr = $proyeccion[$key]['saldo_uvr'] * $TMV;//=IF(G23>$D$15,G23*$D$11,0)
			//Valor cuota UVR - IF(G22>$D$15,($D$9*$D$11)/(1-(1+$D$11)^-$D$7),0)
			$cuota_uvr = ($CRU * $TMV) / (1-pow((1+$TMV), -$PLM));
			//Amortización //=IF(G22>$D$15,D23-$F$23,0)
			$amortizacion_uvr = $cuota_uvr - $interes_uvr;
			//Saldo en UVR // =IF(G22>$D$15,D9-E23,0)
			if ($key == 0)
				$saldo_uvr = $CRU - $amortizacion_uvr;
			else
				$saldo_uvr = $proyeccion[$key]['saldo_uvr'] - $amortizacion_uvr;
			//Cotización UVR //IF(G22>$D$15,$J$22*(1+$D$13)^C23,0)
			$cotizacion_uvr = $proyeccion[0]['cotizacion_uvr']*pow((1+($IEV/100)),($key+1));
		}
		//Cuotas en pesos //cuota_pesos
		$cuota_pesos = $cuota_uvr * $cotizacion_uvr;
		//Saldo en pesos
		$saldo_pesos = $saldo_uvr * $cotizacion_uvr;
		$proyeccion[] = array(
			'cuota_uvr' => $cuota_uvr,
			'amortizacion_uvr' => $amortizacion_uvr,
			'interes_uvr' => $interes_uvr,
			'saldo_uvr' => $saldo_uvr,
			'cuota_pesos' => $cuota_pesos,
			'saldo_pesos' => $saldo_pesos,
			'cotizacion_uvr' => $cotizacion_uvr,
		);
		$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, $key+1);
	}
	$IMLM = (ceil((($proyeccion[1]['cuota_pesos']/0.3)/1000))*1000); //Información para el usuario);
	//Conditions
	$row_conditions = array();
	$row_conditions[] = array(t('Crédito en pesos (CRP)'), '$ '.number_format(round($CRP, 0), 0, '.', ','),);
	$row_conditions[] = array(t('Plazo en años (PLA)'), $PLA,);
	$row_conditions[] = array(t('Plazo en meses (PLM)'), $PLM,);
	$row_conditions[] = array(t('Cotización Inicial en UVR (UVR)'), '$ '.round($UVR, 4),);
	$row_conditions[] = array(t('Crédito en UVR (CRU)'), number_format($CRU, 0, '.', ','),);
	$row_conditions[] = array(t('Tasa Efectiva Anual (TEA)'), $TEA .'%',);
	$row_conditions[] = array(t('Tasa Mensual Vencido (TMV)'), round($TMV, 6),);
	$row_conditions[] = array(t('Inflación Esperada Anual (IEA)'), $IEA.'%',);
	$row_conditions[] = array(t('Inflación Mensual Vencido (IEV)'), round($IEV, 2).'%',);
	$row_conditions[] = array(t('Ingresos Minímos Libres Mes'), '$ '.number_format($IMLM, 0, '.', ','),);
	$row_conditions[] = array(t('Cerrar Crédito si falta # UVR'), $CCSFU,);
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
		'cuota_uvr' => round($proyeccion[$key]['cuota_uvr'], 4),
		'amortizacion_uvr' => round($proyeccion[$key]['amortizacion_uvr'], 4),
		'interes_uvr' => round($proyeccion[$key]['interes_uvr'], 4),
		'saldo_uvr' => round($proyeccion[$key]['saldo_uvr'], 4),
		'cuota_pesos' => '$ '.number_format(round($proyeccion[$key]['cuota_pesos'], 0), 0, '.', ','),
		'saldo_pesos' => '$ '.number_format(round($proyeccion[$key]['saldo_pesos'], 0), 0, '.', ','),
		'cotizacion_uvr' => '$ '.round($proyeccion[$key]['cotizacion_uvr'], 4),
	);
	return $proyeccion_format;
}