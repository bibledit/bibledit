/*
Copyright (©) 2003-2016 Teus Benschop.

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


#include <unittests/check_versification.h>
#include <unittests/utilities.h>
#include <database/versifications.h>
#include <database/check.h>
#include <filter/string.h>
#include <checks/versification.h>


void test_check_versification ()
{
  trace_unit_tests (__func__);
  
  // Test Available Books
  {
    refresh_sandbox (true);
    Database_Versifications database_versifications;
    database_versifications.create ();
    database_versifications.defaults ();
    Database_Check database_check;
    database_check.create ();
    vector <int> books = database_versifications.getBooks (english ());
    vector <int> fault = filter_string_array_diff (books, {10});
    Checks_Versification::books ("Bible", fault);
    vector <Database_Check_Hit> results = database_check.getHits ();
    evaluate (__LINE__, __func__, 1, (int)results.size());
    if (results.size ()) {
      Database_Check_Hit hit = results[0];
      evaluate (__LINE__, __func__, 1, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 10, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 1, hit.verse);
      evaluate (__LINE__, __func__, "This book is absent from the Bible", hit.data);
    }
  }
  // Test Available Chapters
  {
    refresh_sandbox (true);
    Database_Versifications database_versifications;
    database_versifications.create ();
    database_versifications.defaults ();
    Database_Check database_check;
    database_check.create ();

    // Ruth.
    int book = 8;
    // One missing and one extra chapter.
    vector <int> chapters = {0, 2, 3, 4, 5};
    // Run check.
    Checks_Versification::chapters ("Bible", book, chapters);

    // Verify results.
    vector <Database_Check_Hit> results = database_check.getHits ();
    evaluate (__LINE__, __func__, 2, (int)results.size());
    if (results.size () == 2) {
      Database_Check_Hit hit = results[0];
      evaluate (__LINE__, __func__, 1, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 1, hit.verse);
      evaluate (__LINE__, __func__, "This chapter is missing", hit.data);
      hit = results[1];
      evaluate (__LINE__, __func__, 2, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 5, hit.chapter);
      evaluate (__LINE__, __func__, 1, hit.verse);
      evaluate (__LINE__, __func__, "This chapter is extra", hit.data);
    }
  }
  // Test Available Verses One
  {
    refresh_sandbox (true);
    Database_Versifications database_versifications;
    database_versifications.create ();
    database_versifications.defaults ();
    Database_Check database_check;
    database_check.create ();

    // Ruth 1.
    int book = 8;
    int chapter = 1;
    // One missing and one extra verse.
    vector <int> verses = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24};

    // Run check.
    Checks_Versification::verses ("Bible", book, chapter, verses);

    // Verify results.
    vector <Database_Check_Hit> results = database_check.getHits ();
    evaluate (__LINE__, __func__, 4, (int)results.size());
    if (results.size () == 4) {
      Database_Check_Hit hit = results[0];
      evaluate (__LINE__, __func__, 1, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 1, hit.verse);
      evaluate (__LINE__, __func__, "This verse is missing according to the versification system", hit.data);
      hit = results[1];
      evaluate (__LINE__, __func__, 2, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 24, hit.verse);
      evaluate (__LINE__, __func__, "This verse is extra according to the versification system", hit.data);
      hit = results[2];
      evaluate (__LINE__, __func__, 3, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 2, hit.verse);
      evaluate (__LINE__, __func__, "The verse is out of sequence", hit.data);
      hit = results[3];
      evaluate (__LINE__, __func__, 4, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 24, hit.verse);
      evaluate (__LINE__, __func__, "The verse is out of sequence", hit.data);
    }
  }
  // Test Available Verses Two
  {
    refresh_sandbox (true);
    Database_Versifications database_versifications;
    database_versifications.create ();
    database_versifications.defaults ();
    Database_Check database_check;
    database_check.create ();
    
    // Ruth 1.
    int book = 8;
    int chapter = 1;

    // Verses out of order.
    vector <int> verses = {0, 2, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
    
    // Run check.
    Checks_Versification::verses ("Bible", book, chapter, verses);
    
    // Verify results.
    vector <Database_Check_Hit> results = database_check.getHits ();
    evaluate (__LINE__, __func__, 3, (int)results.size());
    if (results.size () == 3) {
      Database_Check_Hit hit = results[0];
      evaluate (__LINE__, __func__, 1, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 2, hit.verse);
      evaluate (__LINE__, __func__, "The verse is out of sequence", hit.data);
      hit = results[1];
      evaluate (__LINE__, __func__, 2, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 1, hit.verse);
      evaluate (__LINE__, __func__, "The verse is out of sequence", hit.data);
      hit = results[2];
      evaluate (__LINE__, __func__, 3, hit.rowid);
      evaluate (__LINE__, __func__, "Bible", hit.bible);
      evaluate (__LINE__, __func__, 8, hit.book);
      evaluate (__LINE__, __func__, 1, hit.chapter);
      evaluate (__LINE__, __func__, 3, hit.verse);
      evaluate (__LINE__, __func__, "The verse is out of sequence", hit.data);
    }
  }
}
