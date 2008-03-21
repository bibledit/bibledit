/*
** Copyright (©) 2003-2008 Teus Benschop.
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
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
**  
*/


#ifndef INCLUDED_GIT_EXEC_H
#define INCLUDED_GIT_EXEC_H


#include "libraries.h"


void git_exec_initialize_project (const ustring& project);
void git_exec_store_chapter (const ustring& project, unsigned int book, unsigned int chapter);
void git_exec_commit_project (const ustring& project);
void git_exec_commit_directory (const ustring& directory);
vector <ustring> git_exec_update_project (const ustring& project, const ustring& data);
void git_exec_message (const ustring& message, bool eol);


#endif
