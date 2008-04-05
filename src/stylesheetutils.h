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


#ifndef INCLUDED_STYLESHEETUTILS_H
#define INCLUDED_STYLESHEETUTILS_H


#include "libraries.h"
#include "style.h"
#include "types.h"


void stylesheet_get_ones_available (vector<ustring>& names);
void stylesheet_create_new (const ustring& name, StylesheetType stylesheettype);
void stylesheet_delete (const ustring& name);
void stylesheet_copy (const ustring& from_name, const ustring& to_name);
ustring stylesheet_import (const ustring& filename);
void stylesheet_export (const ustring& name, const ustring& filename);
bool stylesheet_exists (const ustring& name);
void stylesheet_get_styles (const ustring& stylesheet, vector<Style>& styles);
vector<ustring> stylesheet_get_markers (const ustring& stylesheet, vector<ustring> * names);
void stylesheet_delete_style (const ustring& stylesheet, const ustring& marker);
void stylesheet_new_style (const ustring& stylesheet, const ustring& marker);
void stylesheet_save_style (const ustring& stylesheet, const ustring& marker,
                            const ustring& name, const ustring& info,
                            StyleType type, int subtype,
                            double fontsize, int fontpercentage,
                            const ustring& italic, const ustring& bold, 
                            const ustring& underline, const ustring& smallcaps,
                            bool superscript, const ustring& justification,
                            double spacebefore, double spaceafter,
                            double leftmargin, double rightmargin,
                            double firstlineindent, bool spancolumns,
                            unsigned int color, bool print,
                            bool userbool1, bool userbool2, bool userbool3,
                            int userint1, int userint2, int userint3,
                            ustring userstring1, ustring userstring2, ustring userstring3);
int stylesheet_style_get_pointer (const vector<Style>& styles, const ustring& marker);
void stylesheets_upgrade ();
void stylesheet_get_recently_used (const ustring& stylesheet, vector<ustring>& markers, vector<unsigned int>& count);
void stylesheet_set_recently_used (const ustring& stylesheet, vector<ustring>& styles, vector<unsigned int>& counts);
void stylesheet_save_style (const ustring& stylesheet, const Style& style);
void stylesheet_load_style (const ustring& stylesheet, Style& style);
void stylesheet_vacuum (const ustring& stylesheet, unsigned int starttime);
set <ustring> stylesheet_get_styles_of_type (StylesheetType stylesheettype);


#endif
