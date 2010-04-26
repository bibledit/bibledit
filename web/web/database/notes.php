<?php
/**
* @package bibledit
*/
/*
 ** Copyright (©) 2003-2009 Teus Benschop.
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 3 of the License, or
 ** (at your option) any later version.
 **  
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **  
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **  
 */


class Database_Notes
{
  private static $instance;
  private function __construct() {
  } 
  public static function getInstance() 
  {
    if ( empty( self::$instance ) ) {
      self::$instance = new Database_Notes();
    }
    return self::$instance;
  }

  public function verify () {
    $database_instance = Database_Instance::getInstance();
$query = <<<EOD
CREATE TABLE IF NOT EXISTS notes (
id int auto_increment primary key,
identifier int NOT NULL,  # Note identifier.
modified int NOT NULL,    # Date modified.
assigned text,            # Note assigned to: lists users.
bible varchar (256),      # The Bible, if any, the note refers to.
passage text,             # The passage or verse the note refers to.
status varchar (256),     # Note status: New / Need info / Confirmed / Invalid / In progress / Done
severity int,             # Severity of note: Wish / Minor / Normal / Important / Major / Critical
private bool,             # Whether note is private.
summary varchar (256),    # Note summary.
contents text             # Note contents.
);
EOD;
    $database_instance->runQuery ($query);
    // Table update. Subscriptions: Contains users subscribed to this note.
    $database_instance->runQuery ("ALTER TABLE notes ADD subscriptions text AFTER assigned;");
  }

  public function optimize () {
    $database_instance = Database_Instance::getInstance();
    $database_instance->runQuery ("OPTIMIZE TABLE notes;");
  }


  public function identifierExists ($identifier)
  {
    $server = Database_Instance::getInstance ();
    $id = Database_SQLInjection::no ($id);
    $query = "SELECT identifier FROM notes WHERE identifier = $identifier;";
    $result = $server->runQuery ($query);
    return ($result->num_rows > 0);
  }
  
  
  public function getNewUniqueIdentifier ()
  {
    do {
      $identifier = rand (100000000, 999999999);
    } while ($this->identifierExists ($identifier));
    return $identifier;
  }


  private function assembleContents ($identifier, $contents)
  {
    $new_contents = "";
    if (is_numeric ($identifier)) {
      $new_contents = $this->getContents ($identifier);
    }
    $session_logic = Session_Logic::getInstance();
    $datetime = new DateTime();
    Filter_Datetime::user_zone ($datetime);
    $datetime = $datetime->format(DATE_RSS);
    $user = $session_logic->currentUser ();
    $new_contents .= "<p>$user ($datetime):</p>\n";
    $lines = explode ("\n", $contents);
    foreach ($lines as $line) {
      $line = trim ($line);
      $line = Filter_Html::sanitize ($line);
      $new_contents .= "<p>$line</p>\n";
    }
    return $new_contents;
  }

  
  public function storeNewNote ($bible, $book, $chapter, $verse, $summary, $contents)
  {
    // Store new default note into the database.
    $server = Database_Instance::getInstance ();
    $identifier = $this->getNewUniqueIdentifier ();
    $modified = time();
    $bible = Database_SQLInjection::no ($bible);
    $passage = Database_SQLInjection::no ($this->getPassageMarkup ($book, $chapter, $verse));
    // If the $summary is not given, take the first line of the $contents as the $summary.
    if ($summary == "") {
      $summary = explode ("\n", $contents);
      $summary = $summary[0];
    }
    $summary = Database_SQLInjection::no ($summary);
    $contents = $this->assembleContents ($identifier, $contents);
    $contents = Database_SQLInjection::no ($contents);
    if (($contents == "") && ($summary == "")) return;
    $query = "INSERT INTO notes VALUES (NULL, $identifier, $modified, '', '', '$bible', '$passage', 'New', 'Normal', 0, '$summary', '$contents')"; // Todo current user to get subscribed immediately.
    $server->runQuery ($query);
    // Return this new note´s identifier.
    return $identifier;
  }


  public function getPassageMarkup ($book, $chapter, $verse)
  {
    $markup = " $book.$chapter.$verse ";
    return $markup;
  }


  /**
  * Returns an array of note identifiers selected.
  */
  public function selectNotes ()
  {
    $identifiers = array ();
    $server = Database_Instance::getInstance ();
    $query = "SELECT identifier FROM notes;";
    $result = $server->runQuery ($query);
    for ($i = 0; $i < $result->num_rows; $i++) {
      $row = $result->fetch_row();
      $identifier = $row[0];
      $identifiers []= $identifier;
    }
    return $identifiers;
  }
  
  
  public function getSummary ($identifier)
  {
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $query = "SELECT summary FROM notes WHERE identifier = $identifier;";
    $result = $server->runQuery ($query);
    if ($result->num_rows > 0) {
      $row = $result->fetch_row();
      return $row[0];
    }
    return "";
  }
  

  public function getContents ($identifier)
  {
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $query = "SELECT contents FROM notes WHERE identifier = $identifier;";
    $result = $server->runQuery ($query);
    if ($result->num_rows > 0) {
      $row = $result->fetch_row();
      return $row[0];
    }
    return "";
  }
  
  
  public function delete ($identifier)
  {
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $query = "DELETE FROM notes WHERE identifier = $identifier;";
    $result = $server->runQuery ($query);
  }

  /**
  * Add a $comment to an exiting note identified by $identifier.
  */
  public function addComment ($identifier, $comment)
  {
    $identifier = Database_SQLInjection::no ($identifier);
    if ($comment == "") return;

    $server = Database_Instance::getInstance ();
    $session_logic = Session_Logic::getInstance();

    $modified = time();
    $query = "UPDATE notes SET modified = $modified WHERE identifier = $identifier;";
    $server->runQuery ($query);

    $contents = $this->assembleContents ($identifier, $comment);
    $contents = Database_SQLInjection::no ($contents);
    $query = "UPDATE notes SET contents = '$contents' WHERE identifier = $identifier;";
    $server->runQuery ($query);
  }
  
  
  /**
  * Subscribe the current user to the note identified by $identifier.
  */
  public function subscribe ($identifier) // Todo Take in account existing subscriptions. Todo PHPUnit.
  {
    $session_logic = Session_Logic::getInstance();
    $user = $session_logic->currentUser ();
    $this->subscribeUser ($identifier, $user);
  }


  /**
  * Subscribe the $user to the note identified by $identifier.
  */
  public function subscribeUser ($identifier, $user) // Todo use
  {
    // If the user already is subscribed to the note, bail out.
    $subscribers = $this->getSubscribers ($identifier);
    if (in_array ($user, $subscribers)) return;
    // Subscribe $user.
    $subscribers[]= "$user";
    $subscribers = implode ("\n", $subscribers);
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $subscribers = Database_SQLInjection::no ($subscribers);
    $query = "UPDATE notes SET subscriptions = '$subscribers' WHERE identifier = $identifier;";
    $server->runQuery ($query);
  }


  /**
  * Returns an array with the subscribers to the note identified by $identifier.
  */  
  public function getSubscribers ($identifier) // Todo use.
  {
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $query = "SELECT subscriptions FROM notes WHERE identifier = $identifier;";
    $result = $server->runQuery ($query);
    $row = $result->fetch_row();
    $subscribers = explode ("\n", $row[0]);
    $subscribers = array_diff ($subscribers, array (""));
    return $subscribers;
  }


  /**
  * Returns true if $user is subscribed to the note identified by $identifier.
  */
  public function isSubscribed ($identifier, $user) // Todo use
  {
    $subscribers = $this->getSubscribers ($identifier);
    return in_array ($user, $subscribers);
  }

  
  /**
  * Unsubscribes the currently logged in user from the note identified by $identifier.
  */
  public function unsubscribe ($identifier) // Todo use
  {
    $session_logic = Session_Logic::getInstance();
    $user = $session_logic->currentUser ();
    $this->unsubscribeUser ($identifier, $user);
  }


  /**
  * Unsubscribes $user from the note identified by $identifier.
  */
  public function unsubscribeUser ($identifier, $user) // Todo use
  {
    // If the user is not subscribed to the note, bail out.
    $subscribers = $this->getSubscribers ($identifier);
    if (!in_array ($user, $subscribers)) return;
    // Unsubscribe $user.
    $subscribers = array_diff ($subscribers, array ($user));
    $subscribers = implode ("\n", $subscribers);
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $subscribers = Database_SQLInjection::no ($subscribers);
    $query = "UPDATE notes SET subscriptions = '$subscribers' WHERE identifier = $identifier;";
    $server->runQuery ($query);
  }


  /**
  * Assign the note identified by $identifier to $user.
  */
  public function assignUser ($identifier, $user) // Todo implement / PHPUnit / Use.
  {
    // If the note already is assigned to the user, bail out.
    $assignees = $this->getAssignees ($identifier);
    if (in_array ($user, $assignees)) return;
    // Assign the note to the user.
    $assignees[]= "$user";
    $assignees = implode ("\n", $assignees);
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $assignees = Database_SQLInjection::no ($assignees);
    $query = "UPDATE notes SET assigned = '$assignees' WHERE identifier = $identifier;";
    $server->runQuery ($query);
  }


  /**
  * Returns an array with the assignees to the note identified by $identifier.
  */  
  public function getAssignees ($identifier) // Todo implement / PHPUnit / Use.
  {
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $query = "SELECT assigned FROM notes WHERE identifier = $identifier;";
    $result = $server->runQuery ($query);
    $row = $result->fetch_row();
    $assignees = explode ("\n", $row[0]);
    $assignees = array_diff ($assignees, array (""));
    return $assignees;
  }


  /**
  * Returns true if the note identified by $identifier has been assigned to $user.
  */
  public function isAssigned ($identifier, $user)  // Todo implement / PHPUnit / Use.
  {
    $assignees = $this->getAssignees ($identifier);
    return in_array ($user, $assignees);
  }

  
  /**
  * Unassignes the currently logged in user from the note identified by $identifier.
  */
  public function unassign ($identifier)  // Todo implement / PHPUnit / Use.
  {
    $session_logic = Session_Logic::getInstance();
    $user = $session_logic->currentUser ();
    $this->unassignUser ($identifier, $user);
  }


  /**
  * Unassigns $user from the note identified by $identifier.
  */
  public function unassignUser ($identifier, $user)  // Todo implement / PHPUnit / Use.
  {
    // If the notes is not assigned to the user, bail out.
    $assignees = $this->getAssignees ($identifier);
    if (!in_array ($user, $assignees)) return;
    // Remove assigned $user.
    $assignees = array_diff ($assignees, array ($user));
    $assignees = implode ("\n", $assignees);
    $server = Database_Instance::getInstance ();
    $identifier = Database_SQLInjection::no ($identifier);
    $assignees = Database_SQLInjection::no ($assignees);
    $query = "UPDATE notes SET assigned = '$assignees' WHERE identifier = $identifier;";
    $server->runQuery ($query);
  }

  




  

}



?>
