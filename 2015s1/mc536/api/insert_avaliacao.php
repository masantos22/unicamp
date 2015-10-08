<?php

include_once('db.php');

$CPFS = db::select_all("`wa`.`USUARIO`",array('cpf','id_cidade'),array());

//print_r($CPFS);die();

$values = array();
$i = 0;

foreach($CPFS as $cpf){
	$values[$i]['nota'] = rand()%11;
	$values[$i]['cpf'] = $cpf['cpf'];
	$values[$i]['id_cidade'] = $cpf['id_cidade'];
	$i += 1;
}
$r = db::insert("`wa`.`AVALIACAO`",$values);
print_r($r);
?>