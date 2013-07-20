<?php


class Database_UsfmResources
{
  private static $instance;
  private function __construct() {
  } 
  public static function getInstance() 
  {
    if ( empty( self::$instance ) ) {
      self::$instance = new Database_UsfmResources ();
    }
    return self::$instance;
  }


  public function optimize () {
    $database_instance = Database_Instance::getInstance();
    $database_instance->runQuery ("OPTIMIZE TABLE usfmresources;");
  }


  public function getResources ()
  {
    $resources = array ();
    $database_instance = Database_Instance::getInstance();
    $query = "SELECT DISTINCT name FROM usfmresources ORDER BY name ASC;";
    $result = $database_instance->runQuery ($query);
    for ($i = 0; $i < $result->num_rows; $i++) {
      $row = $result->fetch_row();
      $resources[] = $row[0];
    }
    return $resources;
  }


  public function deleteResource ($name)
  {
    $database_instance = Database_Instance::getInstance();
    $name = Database_SQLInjection::no ($name);
    $query = "DELETE FROM usfmresources WHERE name = '$name';";
    $database_instance->runQuery ($query);
  }

  
  public function storeChapter ($name, $book, $chapter, $usfm)
  {
    $database_instance = Database_Instance::getInstance();
    $name = Database_SQLInjection::no ($name);
    $book = Database_SQLInjection::no ($book);
    $chapter = Database_SQLInjection::no ($chapter);
    $usfm = Database_SQLInjection::no ($usfm);
    $query = "DELETE FROM usfmresources WHERE name = '$name' AND book = $book AND chapter = $chapter;";
    $database_instance->runQuery ($query);
    $query = "INSERT INTO usfmresources VALUES (NULL, '$name', $book, $chapter, '$usfm');";
    $database_instance->runQuery ($query);
  }


  public function getBooks ($name)
  {
    $database_instance = Database_Instance::getInstance();
    $name = Database_SQLInjection::no ($name);
    $query = "SELECT DISTINCT book FROM usfmresources WHERE name = '$name';";
    $result = $database_instance->runQuery ($query);
    $books = array ();
    for ($i = 0; $i < $result->num_rows; $i++) {
      $row = $result->fetch_row();
      $books[] = $row[0];
    }
    sort ($books);
    return $books;
  }


  public function getChapters ($name, $book)
  {
    $database_instance = Database_Instance::getInstance();
    $name = Database_SQLInjection::no ($name);
    $book = Database_SQLInjection::no ($book);
    $query = "SELECT DISTINCT chapter FROM usfmresources WHERE name = '$name' AND book = $book ORDER BY chapter ASC;";
    $result = $database_instance->runQuery ($query);
    $chapters = array ();
    for ($i = 0; $i < $result->num_rows; $i++) {
      $row = $result->fetch_row();
      $chapters[] = $row[0];
    }
    return $chapters;
  }


  public function getUsfm ($name, $book, $chapter)
  {
    $database_instance = Database_Instance::getInstance();
    $name = Database_SQLInjection::no ($name);
    $book = Database_SQLInjection::no ($book);
    $chapter = Database_SQLInjection::no ($chapter);
    $query = "SELECT usfm FROM usfmresources WHERE name = '$name' AND book = $book AND chapter = $chapter;";
    $result = $database_instance->runQuery ($query);
    if ($result->num_rows > 0) {
      $row = $result->fetch_row();
      return $row[0];
    }
    return "";    
  }
      

}



?>
