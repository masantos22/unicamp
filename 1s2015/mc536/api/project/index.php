<?php
	include_once('../db.php');
	require 'framework_slim/vendor/autoload.php';
	$app = new \Slim\Slim();

	// simple gets
	$app->get('/','welcome');
	$app->get('/avaliacao','simple_select');
	$app->get('/cidade','simple_select');
	$app->get('/comentario','simple_select');
	$app->get('/dado','simple_select');
	$app->get('/usuario','simple_select');
	$app->get('/website','simple_select');


	// numero de usuarios por cidade
	$app->get('/usuario/cidade/:id_cidade','get_users_per_city');
	// numero de usuarios por cidade
	$app->get('/usuario/cidade/','get_all_users_per_city');

	// avaliacao media de uma cidade
	$app->get('/cidade/:id/media','get_city_avg');
	// melhor avaliacao de uma cidade
	$app->get('/cidade/:id/melhor','get_city_best');
	// pior avaliacao de uma cidade
	$app->get('/cidade/:id/pior','get_city_worst');

	// temperatura maxima
	$app->get('/dado/max_temp','get_max_temp');
	// temperatura minima
	$app->get('/dado/min_temp','get_min_temp');
	// temperatura media de uma cidade
	$app->get('/dado/media_temp/:id_cidade','get_avg_temp');
	// temperatura maxima em um periodo
	$app->get('/dado/max_temp/:begin/:end','get_max_temp');
	// temperatura minima em um periodo
	$app->get('/dado/min_temp/:begin/:end','get_min_temp');
	// temperatura media de uma cidade em um periodo
	$app->get('/dado/media_temp/:id_cidade/:begin/:end','get_avg_temp');
	// precipitacao maxima
	$app->get('/dado/max_precip','get_max_precip');
	// precipitacao minima
	$app->get('/dado/min_precip','get_min_precip');
	// precipitacao maxima em um periodo
	$app->get('/dado/max_precip/:begin/:end','get_max_precip');
	// precipitacao minima em um periodo
	$app->get('/dado/min_precip/:begin/:end','get_min_precip');
	// dados de uma cidade
	$app->get('/dado/cidade/:id','get_city_data');
	// dados de uma cidade em um periodo
	$app->get('/dado/cidade/:id/:begin/:end','get_city_data');

	// todos os comentários para uma dada cidade
	$app->get('/comentario/cidade/:id','get_city_comments');
	// comentario de um dado usuario
	$app->get('/comentario/usuario/:id','get_user_comments');

	$app->run();

	function welcome(){
		echo "WELCOME TO WEATHER ADVISOR!";
	}

	function simple_select(){
		$table = strtoupper(array_pop(explode("/",$_SERVER['REQUEST_URI'])));
		$table = "`wa`."."`".$table."`";
		$data = db::select_all($table,array(),array());
		//print_r($data);
		print get_html($data);

	}

	function get_all_users_per_city(){
		$query = "	SELECT C.nome AS Cidade, 
						   COUNT(U.CPF) AS Usuarios						 
				 	FROM USUARIO AS U 
				 	JOIN CIDADE AS C ON U.id_cidade = C.id_cidade 
				 	GROUP BY C.id_cidade";

		$data = db::query($query);
		print get_html($data);
	}

	function get_users_per_city($id){
		$query = "	SELECT C.nome AS Cidade, 
						   COUNT(U.CPF) AS Usuarios						 
				 	FROM USUARIO AS U 
				 	JOIN CIDADE AS C ON U.id_cidade = C.id_cidade 
				 	WHERE U.id_cidade = ".$id;

		$data = db::query($query);
		print get_html($data);
	}
	
	function get_city_avg($id){
		$query = "	SELECT 	C.nome as Cidade,
							AVG(A.Nota) AS Media 
				  	FROM AVALIACAO AS A
				  	JOIN CIDADE AS C ON A.id_cidade = C.id_cidade
				  	WHERE A.id_cidade = ".$id;

		$data = db::query($query);
		print get_html($data);
	}

	function get_city_best($id){
		$query = "	SELECT C.nome as Cidade,
						   U.nome as Autor,
						   A.nota as Nota
					FROM AVALIACAO as A
					JOIN CIDADE as C on A.`id_cidade` = C.`id_cidade`
					JOIN USUARIO as U on A.cpf = U.cpf
					WHERE A.id_cidade = $id
						  AND A.nota = (
						  		SELECT MAX(A.nota) 
						  		FROM AVALIACAO as A 
						  		WHERE A.id_cidade = $id
						  )";

		$data = db::query($query);
		print get_html($data);
	}

	function get_city_worst($id){
		$query = "	SELECT C.nome as Cidade,
						   U.nome as Autor,
						   A.nota as Nota
					FROM AVALIACAO as A
					JOIN CIDADE as C on A.`id_cidade` = C.`id_cidade`
					JOIN USUARIO as U on A.cpf = U.cpf
					WHERE A.id_cidade = $id
						  AND A.nota = (
						  		SELECT MIN(A.nota) 
						  		FROM AVALIACAO as A 
						  		WHERE A.id_cidade = $id
						  )";
		
		$data = db::query($query);
		print get_html($data);
	}

	function get_max_temp($begin = '2013-01-01',$end = '2015-06-06'){
		$query = "	SELECT C.nome as Cidade,
						   D.dia as Dia,
						   D.max_temp As 'Temperatura Maxima'
				  	FROM DADO AS D
				  	JOIN CIDADE as C ON D.id_cidade = C.id_cidade 
				  	WHERE D.max_temp = (
				  			SELECT MAX(D.max_temp) 
				  			FROM DADO as D
				  			WHERE D.dia >= '$begin'
				  				  AND D.dia <= '$end'
				  		)";
		
		$data = db::query($query);
		print get_html($data);
	}

	function get_min_temp($begin = '2013-01-01',$end = '2015-06-06'){
		$query = "	SELECT C.nome as Cidade,
				   		   D.dia as Dia,
				   		   D.min_temp As 'Temperatura Minima'
		  			FROM DADO AS D
		  			JOIN CIDADE as C ON D.id_cidade = C.id_cidade 
		  			WHERE D.min_temp = (
		  					SELECT MIN(D.min_temp) 
		  					FROM DADO as D
		  					WHERE D.dia >= '$begin'
		  				  	AND D.dia <= '$end'
		  				)";

		$data = db::query($query);
		print get_html($data);
	}

	function get_avg_temp($id_cidade,$begin = '2013-01-01',$end = '2015-06-06'){

		$query = "	SELECT C.nome as Cidade,
	  					   (AVG(min_temp) + AVG(max_temp))/2 as 'Temperatura Media'
					FROM DADO AS D
		  			JOIN CIDADE as C ON D.id_cidade = C.id_cidade 
		  			WHERE D.id_cidade = $id_cidade 
		  				  AND D.dia >= '$begin'
		  				  AND D.dia <= '$end'";

		$data = db::query($query);
		print get_html($data);
	}

	function get_max_precip($begin = '2013-01-01',$end = '2015-06-06'){
		$query = " SELECT C.nome as Cidade,
				   D.dia as Dia,
				   D.precip As 'Precipitacao Maxima'
		  	FROM DADO AS D
		  	JOIN CIDADE as C ON D.id_cidade = C.id_cidade 
		  	WHERE D.precip = (
		  			SELECT MAX(D.precip) 
		  			FROM DADO as D
		  			WHERE D.dia >= '$begin'
		  				  AND D.dia <= '$end'
		  		)";

		$data = db::query($query);
		print get_html($data);
	}

	function get_min_precip($begin = '2013-01-01',$end = '2015-06-06'){
		$query = "	SELECT C.nome as Cidade,
				   D.dia as Dia,
				   D.precip As 'Precipitacao Minima'
		  	FROM DADO AS D
		  	JOIN CIDADE as C ON D.id_cidade = C.id_cidade 
		  	WHERE D.precip = (
		  			SELECT MIN(D.precip) 
		  			FROM DADO as D
		  			WHERE D.dia >= '$begin'
		  				  AND D.dia <= '$end'
		  		)";

		$data = db::query($query);
		print get_html($data);
	}

	function get_city_data($id,$begin = '2013-01-01',$end = '2015-06-06'){
		$query = "	SELECT 	D.dia as Dia ,
							D.max_temp as 'Temperatura Maxima',
							D.min_temp as 'Temperatura Minima',
							D.precip as Precipitacao
				  	FROM DADO AS D 
				  	WHERE D.id_cidade = $id 
				  		  AND D.dia >= '$begin' 
				  		  AND D.dia <= '$end'
				  	ORDER BY D.dia DESC";

		$data = db::query($query);
		print get_html($data);
	}

	function get_city_comments($id){
		$query = "	SELECT 	U.nome as Autor,
							C.nome as Cidade,
							CO.comentario as Comentario
				  	FROM COMENTARIO AS CO
				  	JOIN CIDADE as C ON CO.id_cidade = C.id_cidade
				  	JOIN USUARIO as U ON CO.cpf = U.cpf
				  	WHERE C.id_cidade = $id";

		$data = db::query($query);
		print get_html($data);
	}

	function get_user_comments($id){
		$id = sha1($id);
		$query = "	SELECT 	U.nome as Autor,
							C.nome as Cidade,
							CO.comentario as Comentario
				  	FROM COMENTARIO AS CO
				  	JOIN CIDADE as C ON CO.id_cidade = C.id_cidade
				  	JOIN USUARIO as U ON CO.cpf = U.cpf
				  	WHERE CO.cpf = '$id'";
		$data = db::query($query);
		print get_html($data);
	}

	function get_html($data){
		$html = "<table style='border:1px solid black'>";
		$html .="<tr>";
		foreach($data[0] as $k=>$v){
			$html .="<td style='border:1px solid black'>".$k."</td>";
		}
		$html .="</tr>";
		foreach($data as $d){
			$html .="<tr>";
			foreach($d as $value){
				$html .="<td style='border:1px solid black'>".$value."</td>";
			}
			$html .="</tr>";
		}
		return $html;
	}
?>