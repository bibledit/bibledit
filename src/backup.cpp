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


#include "backup.h"
#include "gwrappers.h"
#include "directories.h"
#include "tiny_utilities.h"
#include "resource_utils.h"


void backup_bible (const ustring& bible, const ustring& filename)
// Backs up a complete Bible.
{
  unlink (filename.c_str());
  GwSpawn spawn ("tar");
  spawn.arg ("-czf");
  spawn.arg (filename);
  spawn.arg (".");
  spawn.workingdirectory (tiny_project_directory (bible));
  spawn.progress ("Backing up Bible " + bible, false);
  spawn.run ();
}


void backup_notes (const ustring& filename)
{
  unlink (filename.c_str());
  GwSpawn spawn ("tar");
  spawn.arg ("-czf");
  spawn.arg (filename);
  spawn.arg (".");
  spawn.workingdirectory (directories_get_notes ());
  spawn.progress ("Backing up notes", false);
  spawn.run ();
}


void backup_resource (const ustring& resource, const ustring& filename)
// Backs up a Resource.
{
  ustring templatename;  
  vector <ustring> filenames;
  vector <ustring> resources = resource_get_resources(filenames, false);
  for (unsigned int i = 0; i < resources.size(); i++) {
    if (resource == resources[i]) {
      templatename = filenames[i];
    }
  }
  if (templatename.empty()) {
    return;
  }
  ustring folder = gw_path_get_dirname (templatename);
  unlink (filename.c_str());
  GwSpawn spawn ("tar");
  spawn.arg ("-czf");
  spawn.arg (filename);
  spawn.arg (".");
  spawn.workingdirectory (folder);
  spawn.progress ("Backing up Resource " + resource, false);
  spawn.run ();
}


void backup_all (const ustring& filename)
{
  unlink (filename.c_str());
  GwSpawn spawn ("tar");
  spawn.arg ("-czf");
  spawn.arg (filename);
  spawn.arg (".");
  spawn.workingdirectory (directories_get_root ());
  spawn.progress ("Backing up everything", false);
  spawn.run ();
}

/*

Todo backup  / restore Resource.

In order to 'transport' a resource xxx from computer 1 to computer 2 (or mail it) you have to
- navigate to .bibledit/resources,
- chose the folder and
- create a zipped archive out of it.

The same would be to install a resource from computer 2 on computer 1:
- copy the zipped file to .bibledit/resources
- unzip it there
and (I suppose) that BE will recognize this as a loaded resource.

-> it would be nice to have tool (like the PT backup tool) to
- create a zipped form of a resource ready to pass on
- install a zipped form of a new resource 

*/
