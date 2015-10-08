<?php
include_once('db.php');

$days = array('01'=>31,'02'=>28,'03'=>31,'04'=>30,'05'=>31,'06'=>30,'07'=>31,'08'=>31,'09'=>30,'10'=>31,'11'=>30,'12'=>31);

$r = db::select_all("`wa`.`CIDADE`",array('nome','id_cidade'),array());

$path = 'output';
$data = array();
$values = array();
$i = 0;
foreach($r as $arr){
		$data[$i]['nome'] = str_replace(' ','-',strtolower($arr['nome']));
		$data[$i]['id_cidade'] = $arr['id_cidade'];
		$i += 1;
	}
$i = -1;

$directories = array_slice(scandir($path),2);
$files = array_slice(scandir($path.'/'.$directories[0].'/db'),2);

foreach($directories as $city){
	$id = get_id($city,$data);
	$files = array_slice(scandir($path.'/'.$city.'/db'),2);
	foreach($files as $file){
		$date = explode('.',explode('_',$file)[1])[0];
		$mes = explode('-',$date)[0];
		$ano = explode('-',$date)[2];
		$f = fopen($path.'/'.$city.'/db/'.$file,'r');
		for($dia = 1;$dia < $days[$mes] + 1; $dia +=1){
			$line = fgets($f);
			$i += 1;
        	$values[$i]['max_temp'] = explode(',',$line)[0];
        	$values[$i]['min_temp'] = explode(',',$line)[1];
        	$values[$i]['precip'] = explode(',',$line)[2];
        	$values[$i]['neve'] = substr(explode(',',$line)[3],0,-1); 
        	$values[$i]['dia'] = $ano.'-'.$mes.'-'.$dia;
        	$values[$i]['id_cidade'] = $id; 
		}
		fclose($f);
	}
}
$r = db::insert("`wa`.`DADO`",$values);
			

function get_id($city,$values){
	foreach($values as $value){
		if($value['nome'] == $city){
			return $value['id_cidade'];
		}
	}
	return -1;

}

?>