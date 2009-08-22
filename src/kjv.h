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


#ifndef INCLUDED_KJV_H
#define INCLUDED_KJV_H


#include "libraries.h"
#include "reference.h"
#include <glib.h>
#include <sqlite3.h>


void kjv_import (GKeyFile *keyfile);
void kjv_import_sword ();
void kjv_import_zefania ();
void kjv_get_strongs_data (const Reference& reference, vector <unsigned int>& strongs, vector <ustring>& phrases, bool include_unmarked);
vector <Reference> kjv_get_strongs_verses (const Reference& reference, unsigned int strongs);
ustring kjv_get_verse (const Reference& reference);
void kjv_get_lemmata_and_morphology (const Reference& reference, vector <ustring>& words,
                                     vector <unsigned int>& lemmata_positions, vector <unsigned int>& lemmata_values, 
																		 vector <unsigned int>& morphology_positions, vector <ustring>& morphology_values);
vector <Reference> kjv_search (ustring text);


#endif
