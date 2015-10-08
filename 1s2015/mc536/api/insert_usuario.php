<?php
include_once('db.php');

$ids = db::select_all("`wa`.`CIDADE`",array('id_cidade'),array());

$f = fopen('names.txt','r');
$values = array();
$i = 1;
$total = 197;
$city = 0;

while(($line = fgets($f)) !== false){
	$i += 1;
	$values[$i-2]['id_cidade'] = $ids[$city]['id_cidade'];	
	$values[$i-2]['nome'] = substr($line,0,-2);
	$values[$i-2]['cpf'] = sha1($i);
	if($i%$total == 0)
		$city += 1;
}

print_r($values);
$r = db::insert("`wa`.`USUARIO`",$values);
print_r($r);
fclose($f);

?>