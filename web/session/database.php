<?php




require_once ("bootstrap/bootstrap.php");



class Session_Database
{



  private static $instance;    // Current object instance.




  // The class constructor.    
  private function __construct() {
  } 




  // Gets or creates the object instance, making it a singleton.
  public static function getInstance() 
  {
    if ( empty( self::$instance ) ) {
      self::$instance = new Session_Database();
    }
    return self::$instance;
  }


  /**
  * getAdministratorCount - Returns how many administrators there are
  */
  public function getAdministratorCount(){
    include ("levels.php");
    $server = Database_Instance::getInstance ();
    $query = "SELECT * FROM users WHERE username = " . ADMIN_LEVEL;
    $result = $server->mysqli->query ($query);
    return $result->num_rows; 
  }


  /**
  * matchUsernamePassword - Returns true if the username and password match
  */
  public function matchUsernamePassword($username, $password){
    $server = Database_Instance::getInstance ();
    $username = Database_SQLInjection::no ($username);
    $password = md5 ($password);
    $query = "SELECT * FROM users WHERE username = '$username' and password = '$password'";
    $result = $server->mysqli->query ($query);
    return $result->num_rows; 
  }


  /**
  * matchEmailPassword - Returns true if the email and password match
  */
  public function matchEmailPassword($email, $password){
    $server = Database_Instance::getInstance ();
    $email = Database_SQLInjection::no ($email);
    $password = md5 ($password);
    $query = "SELECT * FROM users WHERE email = '$email' and password = '$password'";
    $result = $server->mysqli->query ($query);
    return $result->num_rows; 
  }


  /**
  * addNewUser - Inserts the given (username, password, level) into the database.
  */
  public function addNewUser($username, $password, $level, $email){
    $username = Database_SQLInjection::no ($username);
    $password = md5 ($password);
    $email = Database_SQLInjection::no ($email);
    $time = time();
    $query = "INSERT INTO users VALUES ('$username', '$password', '', $level, '$email', $time)";
    $server = Database_Instance::getInstance ();
    return $server->mysqli->query ($query);
  }


  /**
  * getEmailToUser - Returns the username that belongs to the $email.
  */
  public function getEmailToUser ($email) {
    $email = Database_SQLInjection::no ($email);
    $query = "SELECT username FROM users WHERE email = '$email';";
    if ($result->num_rows == 0)
      return "";
    $result_array = $result->fetch_array();
    return $result_array['username'];
  }   


  /**
  * usernameExists - Returns true if the username exists in the database
  */
  public function usernameExists($user){
    $server = Database_Instance::getInstance ();
    $user = Database_SQLInjection::no ($user);
    $query = "SELECT * FROM users WHERE username = '$user'";
    $result = $server->mysqli->query ($query);
    return ($result->num_rows > 0);
  }


  /**
  * getUserLevel - Returns the level that belongs to the user.
  */
  public function getUserLevel ($user) {
    $user = Database_SQLInjection::no ($user);
    $server = Database_Instance::getInstance ();
    $query = "SELECT level FROM users WHERE username = '$user';";
    $result = $server->mysqli->query ($query);
    if ($result->num_rows == 0) {
      include ("session/levels.php");
      return GUEST_LEVEL;
    }
    $result_array = $result->fetch_array();
    return $result_array['level'];
  }   


  /**
  * updateUserLevel - Returns the MySQL query that would update the level of a given user.
  */
  public function updateUserLevel ($user, $level) {
    $user = Database_SQLInjection::no ($user);
    $level = Database_SQLInjection::no ($level);
    $query = "UPDATE users SET level = $level WHERE username = '$user';";
    return $query;
  }   


  /**
  * removeUser - Remove a user from the database.
  */
  public function removeUser($user){
    $server = Database_Instance::getInstance ();
    $user = Database_SQLInjection::no ($user);
    $query = "DELETE FROM users WHERE username = '$user'";
    $result = $server->mysqli->query ($query);
  }





}



?>
