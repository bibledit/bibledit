/*
Copyright (©) 2003-2014 Teus Benschop.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include <database/mappings.h>
#include <filter/url.h>
#include <filter/string.h>
#include <filter/passage.h>
#include <database/sqlite.h>
#include <database/books.h>


// This is a database for the verse mapping systems.
// Resilience: It is normally not written to, so corruption is unlikely.


Database_Mappings::Database_Mappings ()
{
}


Database_Mappings::~Database_Mappings ()
{
}


sqlite3 * Database_Mappings::connect ()
{
  return database_sqlite_connect ("mappings");
}


void Database_Mappings::create1 ()
{
  sqlite3 * db = connect ();
  string sql = 
    "CREATE TABLE IF NOT EXISTS maps ("
    "name text,"
    "book integer,"
    "chapter integer,"
    "verse integer,"
    "origbook integer,"
    "origchapter integer,"
    "origverse integer"
    ");";
  database_sqlite_exec (db, sql);
  sql = "DROP INDEX IF EXISTS bible;";
  database_sqlite_exec (db, sql);
  sql = "DROP INDEX IF EXISTS original;";
  database_sqlite_exec (db, sql);
  database_sqlite_disconnect (db);
}


void Database_Mappings::create2 ()
{
  string sql;
  sqlite3 * db = connect ();
  sql = "CREATE INDEX IF NOT EXISTS bible ON maps (name, book, chapter, verse);";
  database_sqlite_exec (db, sql);
  sql = "CREATE INDEX IF NOT EXISTS original ON maps (name, book, chapter, verse);";
  database_sqlite_exec (db, sql);
  database_sqlite_disconnect (db);
}


// Import the default mappings that come with Bibledit-Web.
void Database_Mappings::defaults ()
{
  string folder = filter_url_create_root_path ("mapping");
  vector <string> files = filter_url_scandir (folder);
  for (auto & file : files) {
    string name (file);
    string extension = filter_url_get_extension (name);
    if (extension != "txt") continue;
    name = name.substr (0, strlen (name.c_str()) - 4);
    name = filter_string_str_replace ("_", " ", name);
    string path = filter_url_create_path (folder, file);
    string data = filter_url_file_get_contents (path);
    import (name, data);
  }
}


void Database_Mappings::optimize ()
{
  sqlite3 * db = connect ();
  database_sqlite_exec (db, "VACUUM maps;");
  database_sqlite_disconnect (db);
}


void Database_Mappings::import (const string& name, const string& data)
{
  // Delete existing mapping with this name.
  erase (name);

  sqlite3 * db = connect ();

  // Begin a transaction for better speed.
  database_sqlite_exec (db, "BEGIN;");
  
  Passage lastPassage = Passage ("", 1, 1, "1");
  Passage lastOriginal = Passage ("", 1, 1, "1");
  
  vector <string> lines = filter_string_explode (data, '\n');
  for (string line : lines) {
    
    // Each line looks like this: 
    // Haggai 2:15 = Haggai 2:14
    // At the left is the passage in this versification.
    // At the right is the passage in the original Hebrew and Greek versification.
    line = filter_string_trim (line);
    if (line.empty ()) continue;

    // Cut the line into two: The two passages.
    vector <string> entry = filter_string_explode (line, '=');
    if (entry.size() != 2) continue;
    
    string spassage = filter_string_trim (entry [0]);
    string soriginal = filter_string_trim (entry [1]);
    
    Passage passage = Passage (); // C++Port = Filter_Books::interpretPassage (lastPassage, spassage);
    lastPassage.book = passage.book;
    lastPassage.chapter = passage.chapter;
    lastPassage.verse = passage.verse;
    Passage original = Passage (); // C++Port = Filter_Books::interpretPassage (lastOriginal, soriginal);
    lastOriginal.book = original.book;
    lastOriginal.chapter = original.chapter;
    lastOriginal.verse = original.verse;

    int book        = passage.book;
    int chapter     = passage.chapter;
    int verse       = convert_to_int (passage.verse);
    int origbook    = original.book;
    int origchapter = original.chapter;
    int origverse   = convert_to_int (original.verse);

    SqliteSQL sql = SqliteSQL ();
    sql.add ("INSERT INTO maps VALUES (");
    sql.add (name);
    sql.add (",");
    sql.add (book);
    sql.add (",");
    sql.add (chapter);
    sql.add (",");
    sql.add (verse);
    sql.add (",");
    sql.add (origbook);
    sql.add (",");
    sql.add (origchapter);
    sql.add (",");
    sql.add (origverse);
    sql.add (");");
    database_sqlite_exec (db, sql.sql);

  }

  // Commit the transaction.
  database_sqlite_exec (db, "COMMIT;");

  database_sqlite_disconnect (db);
}


// Exports a mapping.
string Database_Mappings::output (const string& name)
{
  vector <string> data;
  SqliteSQL sql = SqliteSQL ();
  sql.add ("SELECT * FROM maps WHERE name =");
  sql.add (name);
  sql.add ("ORDER BY book ASC, chapter ASC, verse ASC;");
  sqlite3 * db = connect ();
  map <string, vector <string> > result = database_sqlite_query (db, sql.sql);
  database_sqlite_disconnect (db);
  vector <string> books = result ["book"];
  vector <string> chapters = result ["chapter"];
  vector <string> verses = result ["verse"];
  vector <string> origbooks = result ["origbook"];
  vector <string> origchapters = result ["origchapter"];
  vector <string> origverses = result ["origverse"];

  for (unsigned int i = 0; i < books.size (); i++) {
    int book = convert_to_int (books [i]);
    string bookname = Database_Books::getEnglishFromId (book);
    string chapter = chapters [i];
    string verse = verses [i];
    int origbook = convert_to_int (origbooks[i]);
    string origbookname = Database_Books::getEnglishFromId (origbook);
    string origchapter = origchapters[i];
    string origverse = origverses [i];
    string item = bookname + " " + chapter + ":" + verse + " = " + origbookname + " " + origchapter + ":" + origverse;
    data.push_back (item);
  }
  return filter_string_implode (data, "\n");
}


public function create ($name)
{
  $name = Database_SQLiteInjection::no ($name);
  // Insert one entry, so the $name makes it into the database.
  $query = "INSERT INTO maps VALUES ('$name', 1, 1, 1, 1, 1, 1);";
  Database_SQLite::exec ($this->db, $query);
}


public function erase ($name)
{
  $name = Database_SQLiteInjection::no ($name);
  $query = "DELETE FROM maps WHERE name = '$name';";
  Database_SQLite::exec ($this->db, $query);
}


// Returns the mapping names in the database.
public function names ()
{
  $names = array ();
  
  $query = "SELECT DISTINCT name FROM maps;";
  $result = Database_SQLite::query ($this->db, $query);
  foreach ($result as $row) {
    $names [] = $row[0];
  }

  // Ensure the original mapping is there too.
  if (!in_array ($this->original (), $names)) {
    $names [] = $this->original ();
  }

  sort ($names);

  return $names;
}


public function original ()
{
  return "Hebrew Greek";
}


// This function translates a $book, $chapter, and $verse 
// in the $input versification to a $book, $chapter and $verse 
// in the $output versification.
// It returns an array with one passage in most cases.
// When the verses in the $input and $output versifications overlap,
// it may return an array with two passages.
public function translate ($input, $output, $book, $chapter, $verse)
{
  // Care for situation that the input and output are the same.
  if ($input == $output) {
    $passage = array ($book, $chapter, $verse);
    return array ($passage);
  }

  // Get the $input mapping for the passage from the database.
  // This maps the $input to the Hebrew/Greek versification system.
  // Skip this phase if the $input mapping is Hebrew / Greek.
  $origpassage = array ();
  if ($input != $this->original ()) {
    $input = Database_SQLiteInjection::no ($input);
    $book = (integer) $book;
    $chapter = (integer) $chapter;
    $verse = (integer) $verse;
    $query = "SELECT origbook, origchapter, origverse FROM maps WHERE name = '$input' AND book = $book AND chapter = $chapter AND verse = $verse;";
    $result = Database_SQLite::query ($this->db, $query);
    foreach ($result as $row) {
      unset ($row ['origbook']);
      unset ($row ['origchapter']);
      unset ($row ['origverse']);
      $origpassage [] = $row;
    }
  }
  
  // Check that the search yields a passage.
  // If there is none, it means that the $input passage is the same as in Hebrew/Greek.
  if (empty ($origpassage)) {
    $passage = array ($book, $chapter, $verse);
    $origpassage = array ($passage);
  }

  // If the $output mapping is Hebrew/Greek, then we're done.
  if ($output == $this->original ()) {
    return $origpassage;
  }
  
  // Get the $output mapping for the passage or two passages from the database.
  // This is a translation from Hebrew/Greek to the $output system.
  $targetpassage = array ();
  $output = Database_SQLiteInjection::no ($output);
  foreach ($origpassage as $passage) {
    $origbook = $passage [0];
    $origchapter = $passage [1];
    $origverse = $passage [2];
    $query = "SELECT book, chapter, verse FROM maps WHERE name = '$output' AND origbook = $origbook AND origchapter = $origchapter AND origverse = $origverse;";
    $result = Database_SQLite::query ($this->db, $query);
    foreach ($result as $row) {
      unset ($row ['book']);
      unset ($row ['chapter']);
      unset ($row ['verse']);
      $row [0] = (integer) $row [0];
      $row [1] = (integer) $row [1];
      $row [2] = (integer) $row [2];
      if (array_search ($row, $targetpassage) === false) {
        $targetpassage [] = $row;
      }
    }
  }
  
  // Check that the search yields a passage.
  // If none, it means that the $output passage is the same as in Hebrew/Greek.
  if (empty ($targetpassage)) {
    $targetpassage = $origpassage;
  }
  
  // Result.
  return $targetpassage;
}



/* Todo old stuff

void Database_Mappings::clear ()
{
  sqlite3 * db = connect ();
  database_sqlite_exec (db, "DROP TABLE IF EXISTS bibleactions;");
  database_sqlite_disconnect (db);
}


void Database_Mappings::optimize ()
{
  sqlite3 * db = connect ();
  database_sqlite_exec (db, "VACUUM bibleactions;");
  database_sqlite_disconnect (db);
}


vector <int> Database_Mappings::getChapters (string bible, int book)
{
  SqliteSQL sql = SqliteSQL ();
  sql.add ("SELECT DISTINCT chapter FROM bibleactions WHERE bible =");
  sql.add (bible);
  sql.add ("AND book =");
  sql.add (book);
  sql.add ("ORDER BY chapter;");
  sqlite3 * db = connect ();
  vector <string> result = database_sqlite_query (db, sql.sql)["chapter"];
  database_sqlite_disconnect (db);
  vector <int> chapters;
  for (auto chapter : result) chapters.push_back (convert_to_int (chapter));
  return chapters;
}


string Database_Mappings::getUsfm (string bible, int book, int chapter)
{
  SqliteSQL sql = SqliteSQL ();
  sql.add ("SELECT usfm FROM bibleactions WHERE bible =");
  sql.add (bible);
  sql.add ("AND book =");
  sql.add (book);
  sql.add ("AND chapter =");
  sql.add (chapter);
  sql.add (";");
  sqlite3 * db = connect ();
  vector <string> result = database_sqlite_query (db, sql.sql)["usfm"];
  database_sqlite_disconnect (db);
  for (auto usfm : result) return usfm;
  return "";
}


*/
