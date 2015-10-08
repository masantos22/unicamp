	public static function insert($table,$kv)	
	Example: list(res,id,error) = db::insert("`dbname`.`tablename`",array(array('k'=>1),array('k'=>2)))

	public static function select_all($table, $fields, $args = array())
	array() = db::select_all("`dbname`.`tablename`",array('field1','field2','field3'),array('where'=>array(k=>v),'limit'=>array(5,0)))
	
	public static function select($table, $fields, $args = array())
	array() = db::select_all("`dbname`.`tablename`",array('field1','field2','field3'),array('where'=>array(k=>v),'limit'=>array(5,0)))
	
	public static function query($q,$cond)
    
	public static function delete($table,$cond = array())
	count = db::delete("`dbname`.`tablename`",array(k=>v))

	public static function update($table,$set,$cond =array())
	boolean = db::update("`dbname`.`tablename`",array('key'=>'value')))

    Import Database
    tar zxf dumpfilename.tar.gz
    mysql -u [username] -p [database_name] < [dumpfilename.sql]

    Import All Databases
    tar zxf dumpfilename.tar.gz
    mysql -u root -p < alldb.sql


