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


#ifndef INCLUDED_DATABASE_SQLITE_H
#define INCLUDED_DATABASE_SQLITE_H


#include <config/libraries.h>



#include <sqlite3.h>


using namespace std;


string database_sqlite_file (string database);
sqlite3 * database_sqlite_connect (string database);
string database_sqlite_no_sql_injection (string sql);
void database_sqlite_exec (sqlite3 * db, string sql);
map <string, vector <string> > database_sqlite_query (sqlite3 * db, string sql);
void database_sqlite_disconnect (sqlite3 * database);
bool database_sqlite_healthy (string database);


// Stores values collected during a reading session of sqlite3.
class SqliteReader
{
public:
  SqliteReader (int dummy);
  ~SqliteReader ();
  map <string, vector <string> > result;
  static int callback (void *userdata, int argc, char **argv, char **column_names);
private:
};


#endif
