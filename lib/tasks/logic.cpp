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


#include <tasks/logic.h>
#include <database/logs.h>
#include <filter/string.h>
#include <filter/url.h>


// Folder where the tasks are stored.
string tasks_logic_folder ()
{
  return filter_url_create_root_path ("processes");
}


// Queue $command to run in a thread, with $parameters.
void tasks_logic_queue (string command, vector <string> parameters) // Todo
{
  // The file on disk will contain the command on the first line,
  // and any parameters on the following lines, one parameters per line.
  vector <string> lines;
  lines.push_back (command);
  lines.insert (lines.end(), parameters.begin(), parameters.end());
  // The filename to write to contains seconds and microseconds.
  string seconds = convert_to_string (filter_string_date_seconds_since_epoch ());
  string time = seconds + filter_string_fill (convert_to_string (filter_string_date_numerical_microseconds ()), 8, '0');
  string file = filter_url_create_path (tasks_logic_folder (), time);
  // On Windows the microtime is not fine enough.
  // This leads to one task overwriting a previous one in case it is queued immediately after.
  // Deal with that problem here: Ensure the filename is unique.
  file = filter_url_unique_path (file);
  // Save it.
  command = filter_string_implode (lines, "\n");
  filter_url_file_put_contents (file, command);
}

