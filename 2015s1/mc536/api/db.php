<?php
include_once('db_default.php');

class dbinst{
    private static $session;
    public static function db(){
        if(!self::$session)
            self::$session = new PDO(sprintf("mysql:host=%s;dbname=%s",MYDB_HOST,MYDB_NAME),MYDB_USER,MYDB_PASS);
        return self::$session;
    }
}
class db{
	private $dbh = NULL;
	private $debug = false;
	public function __construct($debug = false){
		$this->debug = $debug;
        $this->dbh = dbinst::db(); 
        //new PDO(sprintf("mysql:host=%s;dbname=%s",DBHOST,DBNAME),DBUSER,DBPASS);
	}
	public static function insert($table,$kv){
		//size of keys must match size of values
/*
		if(count($keys) != count($vals)){
			throw new Exception('Size of Keys does not match size of vals array');
		}
*/
		if(!isset($kv[0]))
			$kv = array($kv);
		$db = new db();
		$db->dbh->beginTransaction();
		$cols = array();
		//PDO values
		$first =& $kv[0];
		$vals = ':'.implode(',:',array_keys($first));
		foreach($first as $k => &$v){
			$cols[] = "`$k`";	
		}
		$s_cols = implode(',',$cols);
		
		//Assemble INSERT Query
		$q = sprintf("INSERT INTO %s (%s) VALUES (%s)",$table,$s_cols,$vals);	
		db::debug($q);
		try{
			$d = $db->dbh->prepare($q);		
			foreach($kv as $row){
				foreach($row as $k => &$v){
					$v = ($v == 'NULL')? NULL : $v;
					$v = ($v == NULL)? NULL : $v;
					$d->bindParam(":{$k}",$v,(is_int($v))? PDO::PARAM_INT : PDO::PARAM_STR);
				}
				$d->execute();
			}
			db::debug($db->dbh);
			$r = $db->dbh->commit();// = $d->execute();
			return array($r, $db->dbh->lastInsertId(), $d->errorInfo());
		}catch (PDOException $e){
			db::debug($e->getMessage());
		}
	
	}
	
	public static function selectq($q, $cond, $fields = NULL){
		$db = new db();
		if($fields){
			/*
			foreach($fields as $field){
				
			}
			*/
		}
	}
	public static function select_all($table, $fields, $args = array()){
        $_field = '';
        if(empty($fields))
        	$_field = '* ';
        foreach($fields as $field){
            $_field .= " `$field`,";
        }
        $_field = substr($_field,0,-1);
        $sets = '';
        if(isset($args['where'])){
            foreach($args['where'] as $k=>$v){
                $a_sets[] = "`$k`=:$k";
            }
            $sets .= ' WHERE '.implode(' AND ',$a_sets);
        }
        if(isset($args['limit'])){
            $size = (isset($args['limir'][1]))? "{$args['limit'][0]},{$args['limit'][1]}" : "{$args['limit'][0]}";
            $sets .= " LIMIT $size";
        }
        $q = trim(sprintf("SELECT %s FROM %s %s",$_field,$table,$sets));
        db::debug($q);
        try{
            $db = new db();
            $d = $db->dbh->prepare($q);
            if(isset($args['where'])){
                foreach($args['where'] as $k=> &$v){
                    $v = ($v == 'NULL')? 0 : $v;
                    $v = ($v == NULL)? 0 : $v;
                    $d->bindParam($k,$v,(is_int($v))? PDO::PARAM_INT : PDO::PARAM_STR);
                }
            }
            $d->execute();
            $r = $d->fetchAll(PDO::FETCH_ASSOC);
            //        db::debug($r);
      //      db::debug($d->errorInfo());
            return $r;
        }catch(PDOException $ex){
            
        }
	}
	public static function select($table, $fields, $args = array()){
        $_field = '';
        foreach($fields as $field){
            $_field .= " `$field`,";
        }
        $_field = substr($_field,0,-1);
        $sets = '';
        if(isset($args['where'])){
            foreach($args['where'] as $k=>$v){
                $a_sets[] = "`$k`=:$k";
            }
            $sets .= ' WHERE '.implode(' AND ',$a_sets);
        }
        if(isset($args['limit'])){
            $size = (isset($args['limir'][1]))? "{$args['limit'][0]},{$args['limit'][1]}" : "{$args['limit'][0]}";
            $sets .= "LIMIT $size";
        }
        $q = trim(sprintf("SELECT %s FROM %s %s",$_field,$table,$sets));
        db::debug($q);
        try{
            $db = new db();
            $d = $db->dbh->prepare($q);
            if(isset($args['where'])){
                foreach($args['where'] as $k=> &$v){
                    $v = ($v == 'NULL')? 0 : $v;
                    $v = ($v == NULL)? 0 : $v;
                    $d->bindParam($k,$v,(is_int($v))? PDO::PARAM_INT : PDO::PARAM_STR);
                }
            }
            $d->execute();
            $r = $d->fetch(PDO::FETCH_ASSOC);
            //        db::debug($r);
      //      db::debug($d->errorInfo());
            return $r;
        }catch(PDOException $ex){
            
        }
	}
	public static function query($q){
	    try{
            $db = new db();
            $d = $db->dbh->prepare($q);
            
            $d->execute();
            $r = $d->fetchAll(PDO::FETCH_ASSOC);
                    db::debug($r);
      			db::debug($d->errorInfo());
            return $r;
        }catch(PDOException $ex){
            
        }
	}
    public static function delete($table,$cond = array()){
		$where = '';
		if(count($cond) > 0){
			foreach($cond as $k => $v){
				$a_where[] = "`$k`=:w_$k";
				$params['w_'.$k] = $v;
			}
			$where = " WHERE ".implode(" && ",$a_where);
		}
		$q = trim(sprintf("DELETE FROM %s %s",$table,$where));
		try{
			$db = new db();
			$d = $db->dbh->prepare($q);
			foreach($params as $k=> &$v){
				$v = ($v == 'NULL')? 0 : $v;
				$v = ($v == NULL)? 0 : $v;
				$d->bindParam($k,$v,(is_int($v))? PDO::PARAM_INT : PDO::PARAM_STR);
			}
			$r = $d->execute();
			db::debug($r);
			db::debug($d->errorInfo());
			return array($r,$d->errorInfo());
		}catch(PDOException $ex){
		}
    }
	public static function update($table,$set,$cond =array()){
		$params = array();
		$a_where = array();
		$where = '';
		if(count($cond) > 0){
			foreach($cond as $k => $v){
				$a_where[] = "`$k`=:w_$k";
				$params['w_'.$k] = $v;
			}
			$where = " WHERE ".implode(" && ",$a_where);
		}
		
		$a_sets = array();
		foreach($set as $k=>$v){
			$a_sets[] = "`$k`=:$k";
		}
		$sets = implode(', ',$a_sets);
		$params = array_merge($set,$params);	
		db::debug($params);
		$q = trim(sprintf("UPDATE %s SET %s %s",$table,$sets, $where));
		db::debug($q);
		try{
			$db = new db();
			$d = $db->dbh->prepare($q);
			foreach($params as $k=> &$v){
				$v = ($v == 'NULL')? 0 : $v;
				$v = ($v == NULL)? 0 : $v;
				$d->bindParam($k,$v,(is_int($v))? PDO::PARAM_INT : PDO::PARAM_STR);
			}
			$r = $d->execute();
			db::debug($r);
			db::debug($d->errorInfo());
			return array($r,$d->errorInfo());
		}catch(PDOException $ex){
		}
	}
	public static function debug($input){
		if(DB_DEBUG){
			if(is_array($input))
				print_r($input);
			else
				var_dump($input);
			echo "\n";
		}	
	}	
}
?>