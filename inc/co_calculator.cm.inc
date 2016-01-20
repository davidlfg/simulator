<?php
/*'cm' => t('Cuota media')
 *
 * Todos los valores divididos sobre 100 son variables tipo %
 */

/**
 * function co_calculator_cm().
 */
function co_calculator_cm($CRP, $PLA) {
	//Plazo en meses
	$PLM = $PLA * 12;
	//Cotización Inicial en UVR (UVR)
	$UVR = variable_get('co_simulator_variable_uvr');//UVR del Banco de la Republica (www.banrep.gov.co)
	//Crédito en UVR (CRU)
	$CRU = $CRP / $UVR;
	//Tasa efectiva anual
	$TEA = variable_get('co_simulator_variable_cm_tea');//Parametrizable por el administrador
	//Tasa mensual vencida - ((1+[TEA])^(1/12))-1
	$TMV = (pow((1+($TEA/100)),(1/12))-1); 
	//Inflación Esperada Anual (IEA)
	$IEA = variable_get('co_simulator_variable_cm_iea');//Parametrizable por administrador
	//Inflación Mensual Vencido (IEV) - ((1+D12)^(1/12))-1
	$IEV = (pow((1+($IEA) / 100), (1/12))-1)*100;
	//Cerrar Crédito si falta # UVR
	$CCSFU = variable_get('co_simulator_variable_cm_ccsfp');//Parametrizable por administrador
	$proyeccion = array();
	$proyeccion_format = array();
	//Cuota No 0
	$proyeccion[] = array(
		'uvr_cuota' => 0,
		'uvr_intereses_corrientes' => 0,
		'uvr_abono_capital' => 0,
		'uvr_saldo' => ($CRP/$UVR),
		'pesos_cuota' => 0,
		'pesos_interes_corriente' => 0,
		'pesos_abono_capital' => 0,
		'pesos_saldo' => $CRP,
		'pesos_cotizacion' => $UVR,
	);
	$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, 0);
	$pesos_saldo = $CRP;
	//Condición para cerrar credito
	while ($pesos_saldo >= $CCSFU) {
		$key = count($proyeccion)-1;
		//Declaración de variables
		$uvr_cuota = 0;
		$uvr_intereses_corrientes = 0;
		$uvr_abono_capital = 0;
		$uvr_saldo = 0;
		$pesos_cuota = 0;
		$pesos_interes_corriente = 0;
		$pesos_abono_capital = 0;
		$pesos_saldo = 0;
		$pesos_cotizacion = 0;
		
		if ($proyeccion[$key]['uvr_saldo'] > $CCSFU) {
			//Cotización - IF(G23>$D$14,$L$23*(1+$D$12)^C24,0)
			$pesos_cotizacion = $proyeccion[0]['pesos_cotizacion']*pow((1+($IEV/100)), ($key+1));
			//Abono a capital - IF(G23>$D$14,$D$8/$D$6,0)
			$uvr_abono_capital = $CRU/$PLM;
			//UVR Saldo - IF(G23>$D$14,G23-F24,0)
			$uvr_saldo = $proyeccion[$key]['uvr_saldo'] - $uvr_abono_capital;
		}
		//Solo para la cuota No 1
		if ($key == 0) {
			//Interes corriente
			$uvr_intereses_corrientes = $CRU * $TMV;
			//UVR Valor cuota
			$uvr_cuota = $uvr_intereses_corrientes + $uvr_abono_capital;
			//Pesos Valor cuota
			$pesos_cuota = $uvr_cuota * $pesos_cotizacion;
			//Pesos Interes corriente
			$pesos_interes_corriente = $uvr_intereses_corrientes * $proyeccion[$key]['pesos_cotizacion'];
			//Pesos Abono a capital
			$pesos_abono_capital = $uvr_abono_capital * $proyeccion[$key]['pesos_cotizacion'];
			//Pesos Saldo
			$pesos_saldo = $uvr_saldo * $proyeccion[$key]['pesos_cotizacion'];
		} else {
			//Interes corriente
			$uvr_intereses_corrientes = $proyeccion[$key]['uvr_saldo'] * $TMV;
			//UVR Valor cuota
			$uvr_cuota = $uvr_intereses_corrientes + $uvr_abono_capital;
			//Pesos Valor cuota
			$pesos_cuota = $uvr_cuota * $pesos_cotizacion;
			//Pesos Interes corriente
			$pesos_interes_corriente = $uvr_intereses_corrientes * $pesos_cotizacion;
			//Pesos Abono a capital
			$pesos_abono_capital = $uvr_abono_capital * $pesos_cotizacion;
			//Pesos Saldo
			$pesos_saldo = $uvr_saldo * $pesos_cotizacion;
		}
		$proyeccion[] = array(
			'uvr_cuota' => $uvr_cuota,
			'uvr_intereses_corrientes' => $uvr_intereses_corrientes,
			'uvr_abono_capital' => $uvr_abono_capital,
			'uvr_saldo' => $uvr_saldo,
			'pesos_cuota' => $pesos_cuota,
			'pesos_interes_corriente' => $pesos_interes_corriente,
			'pesos_abono_capital' => $pesos_abono_capital,
			'pesos_saldo' => $pesos_saldo,
			'pesos_cotizacion' => $pesos_cotizacion,
		);
		$proyeccion_format = co_calculator_cm_format($proyeccion, $proyeccion_format, $key+1);
	}
	//Información para el usuario
	$IMLM = (ceil((($proyeccion[1]['pesos_cuota']/0.3)/1000))*1000);
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
	$row_conditions[] = array(t('Inflación Mensual Vencido (IEV)'), round($IEV, 5).'%',);
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
		'uvr_cuota' => round($proyeccion[$key]['uvr_cuota'], 4),
		'uvr_intereses_corrientes' => round($proyeccion[$key]['uvr_intereses_corrientes'], 4),
		'uvr_abono_capital' => round($proyeccion[$key]['uvr_abono_capital'], 4),
		'uvr_saldo' => round($proyeccion[$key]['uvr_saldo'], 4),
		'pesos_cuota' => '$ '.number_format(round($proyeccion[$key]['pesos_cuota'], 0), 0, '.', ','),
		'pesos_interes_corriente' => '$ '.number_format(round($proyeccion[$key]['pesos_interes_corriente'], 0), 0, '.', ','),
		'pesos_abono_capital' => '$ '.number_format(round($proyeccion[$key]['pesos_abono_capital'], 0), 0, '.', ','),
		'pesos_saldo' => '$ '.number_format(round($proyeccion[$key]['pesos_saldo'], 0), 0, '.', ','),
		'pesos_cotizacion' => '$ '.round($proyeccion[$key]['pesos_cotizacion'], 4),
	);
	return $proyeccion_format;
}