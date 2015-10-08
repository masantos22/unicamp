<?php

include_once('db.php');

$CPFS = db::select_all("`wa`.`USUARIO`",array('cpf'),array());
$ID_CIDADE = db::select_all("`wa`.`CIDADE`",array('id_cidade'),array());

$file = file_get_contents("quotes.txt");
$quotes = explode("\n.", $file);

$values = array();
$i = 0;

// Generate 2 random quotes per user in random cities
foreach($CPFS as $cpf){
	$quote1 = $quotes[array_rand($quotes)];
	$quote2 = $quotes[array_rand($quotes)];
	$city1 = $ID_CIDADE[array_rand($ID_CIDADE)]['id_cidade'];
	$city2 = $ID_CIDADE[array_rand($ID_CIDADE)]['id_cidade'];
	
	$values[$i]['comentario'] = $quote1;
	$values[$i]['cpf'] = $cpf['cpf'];
	$values[$i]['id_cidade'] = $city1;

	$values[$i+1]['comentario'] = $quote2;
	$values[$i+1]['cpf'] = $cpf['cpf'];
	$values[$i+1]['id_cidade'] = $city2;
	$i += 2;
}
$r = db::insert("`wa`.`COMENTARIO`",$values);

?>