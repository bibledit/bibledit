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


#ifndef INCLUDED_MERGE_UTILS_H
#define INCLUDED_MERGE_UTILS_H


#include "libraries.h"
#include <gtk/gtk.h>


ustring merge_conflict_markup (unsigned int number);


class MergeButton
{
public:
  MergeButton (int dummy);
  MergeButton ();
  GtkTextChildAnchor * childanchor;
  GtkWidget * button;
  ustring text;
private:
};


class MergeButtonPair
{
public:
  MergeButtonPair (int dummy);
  MergeButtonPair ();
  MergeButton button1;
  MergeButton button2;
private:
};


#endif
