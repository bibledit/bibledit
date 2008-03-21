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


#ifndef INCLUDED_ODTNOTES_H
#define INCLUDED_ODTNOTES_H


#include "libraries.h"
#include <glib.h>
#include <gtk/gtk.h>
#include "usfm.h"
#include "notecaller.h"
#include "xslfofootnoteparagraph.h"


class OdtFootnote
{
public:
  OdtFootnote (const Usfm& usfm);
  ~OdtFootnote ();
  void new_book ();
  void new_chapter ();
  void transform (ustring& line);
  NoteCaller * notecaller;
private:
  bool show;
  NoteNumberingRestartType note_numbering_restart;
  ustring opening_marker;
  ustring closing_marker;
  bool anchor_apocrypha;
  vector<ustring> content_marker;
  vector <bool> content_apocrypha;
  set<ustring> note_markers;
  XslFoFootnoteParagraph * standardparagraph;
  XslFoFootnoteParagraph * extraparagraph;
  ustring transform_main_parts (const ustring& line);
  unsigned int note_id;
};


class OdtEndnote
{
public:
  OdtEndnote (const Usfm& usfm);
  ~OdtEndnote ();
  void new_book ();
  void new_chapter ();
  void transform (ustring& line);
  NoteCaller * notecaller;
private:
  bool show;
  NoteNumberingRestartType note_numbering_restart;
  ustring opening_marker;
  ustring closing_marker;
  bool anchor_apocrypha;
  vector<ustring> content_marker;
  vector <bool> content_apocrypha;
  set<ustring> note_markers;
  XslFoFootnoteParagraph * standardparagraph;
  XslFoFootnoteParagraph * extraparagraph;
  ustring transform_main_parts (const ustring& line);
  unsigned int note_id;
};


class OdtXref
{
public:
  OdtXref (const Usfm& usfm);
  ~OdtXref ();
  void new_book ();
  void new_chapter ();
  void transform (ustring& line);
  NoteCaller * notecaller;
private:
  bool show;
  NoteNumberingRestartType note_numbering_restart;
  ustring opening_marker;
  ustring closing_marker;
  bool anchor_apocrypha;
  vector<ustring> content_marker;
  vector <bool> content_apocrypha;
  set<ustring> note_markers;
  XslFoFootnoteParagraph * standardparagraph;
  ustring transform_main_parts (const ustring& line);
  unsigned int note_id;
};


#endif
