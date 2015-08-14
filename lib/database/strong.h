/*
Copyright (©) 2003-2015 Teus Benschop.

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


#ifndef INCLUDED_DATABASE_STRONG_H
#define INCLUDED_DATABASE_STRONG_H


#include <config/libraries.h>
#include <sqlite3.h>


class Database_Strong
{
public:
  string word (string strong);
  string source (string strong);
  string meaning (string strong);
  string usage (string strong);
private:
  sqlite3 * connectheb ();
  sqlite3 * connectgrk ();
  string get (string strong, string item);
};


#endif
