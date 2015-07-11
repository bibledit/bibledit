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


#ifndef INCLUDED_SYNC_LOGIC_H
#define INCLUDED_SYNC_LOGIC_H


#include <config/libraries.h>
#include <sqlite3.h>
#include <filter/passage.h>


class Sync_Logic_Range
{
public:
  int low;
  int high;
};


class Sync_Logic
{
public:
  Sync_Logic (void * webserver_request_in);
  ~Sync_Logic ();
  static const int settings_get_total_checksum = 1;
  static const int settings_send_workbench_urls = 2;
  static const int settings_get_workbench_urls = 3;
  static const int settings_send_workbench_widths = 4;
  static const int settings_get_workbench_widths = 5;
  static const int settings_send_workbench_heights = 6;
  static const int settings_get_workbench_heights = 7;
  static const int settings_send_resources_organization = 8;
  static const int settings_get_resources_organization = 9;
  static const int settings_get_bible_id = 10;
  static const int bibles_get_total_checksum = 1;
  static const int bibles_get_bibles = 2;
  static const int bibles_get_bible_checksum = 3;
  static const int bibles_get_books = 4;
  static const int bibles_get_book_checksum = 5;
  static const int bibles_get_chapters = 6;
  static const int bibles_get_chapter_checksum = 7;
  static const int bibles_send_chapter = 8;
  static const int bibles_get_chapter = 9;
  static const int notes_get_total = 1;
  static const int notes_get_identifiers = 2;
  static const int notes_get_summary = 3;
  static const int notes_get_contents = 4;
  static const int notes_get_subscribers = 5;
  static const int notes_get_assignees = 6;
  static const int notes_get_status = 7;
  static const int notes_get_passages = 8;
  static const int notes_get_severity = 9;
  static const int notes_get_bible = 10;
  static const int notes_get_modified = 11;
  static const int notes_put_create_initiate = 12;
  static const int notes_put_create_complete = 13;
  static const int notes_put_summary = 14;
  static const int notes_put_contents = 15;
  static const int notes_put_comment = 16;
  static const int notes_put_subscribe = 17;
  static const int notes_put_unsubscribe = 18;
  static const int notes_put_assign = 19;
  static const int notes_put_unassign = 20;
  static const int notes_put_status = 21;
  static const int notes_put_passages = 22;
  static const int notes_put_severity = 23;
  static const int notes_put_bible = 24;
  static const int notes_put_mark_delete = 25;
  static const int notes_put_unmark_delete = 26;
  static const int notes_put_delete = 27;
  static const int usfmresources_get_total_checksum = 1;
  static const int usfmresources_get_resources = 2;
  static const int usfmresources_get_resource_checksum = 3;
  static const int usfmresources_get_books = 4;
  static const int usfmresources_get_book_checksum = 5;
  static const int usfmresources_get_chapters = 6;
  static const int usfmresources_get_chapter_checksum = 7;
  static const int usfmresources_get_chapter = 8;
  static const int offlineresources_get_total_checksum = 1;
  static const int offlineresources_get_resources = 2;
  static const int offlineresources_get_resource_checksum = 3;
  static const int offlineresources_get_files = 4;
  static const int offlineresources_get_file_checksum = 5;
  static const int offlineresources_get_file = 6;
  static const int offlineresources_get_file_filename = 7;
  static const int changes_delete_modification = 1;
  static const int changes_get_checksum = 2;
  static const int changes_get_identifiers = 3;
  static const int changes_get_modification = 4;
  bool credentials_okay ();
  string settings_checksum (const vector <string> & bibles);
  string checksum (const vector <int> & identifiers);
  vector <Sync_Logic_Range> create_range (int start, int end);
  string post (map <string, string> & post, const string& url, string & error, bool burst = false);
  static string usfm_resources_checksum ();
  static string usfm_resource_checksum (const string& name);
  static string usfm_resource_book_checksum (const string& name, int book);
  static string usfm_resource_chapter_checksum (const string& name, int book, int chapter);
  static string offline_resources_checksum ();
  static string offline_resource_checksum (const string& name);
  static string offline_resource_file_checksum (const string & name, const string & file);
  static string changes_checksum (const string & username);
private:
  void * webserver_request;
};


#endif
