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


#ifndef INCLUDED_EXPORT_UTILS_H
#define INCLUDED_EXPORT_UTILS_H


#include "libraries.h"
#include <gtk/gtk.h>


enum ExportNotesFormat {BibleditVersion3, ScriptureNotesVersion20};


void export_to_usfm (GtkWidget * parent, bool zipped);
void export_to_bibleworks (GtkWidget * parent);
void export_translation_notes (const ustring& filename, ExportNotesFormat format, const vector<unsigned int> & ids_to_display, bool export_all, GtkWidget * parent);
void export_to_sword_interactive ();
void export_to_sword_script (const ustring& project, ustring directory, bool gui);
void export_to_opendocument (GtkWidget * parent);


#endif
