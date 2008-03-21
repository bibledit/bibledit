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


#ifndef INCLUDED_XSLFO_FOOTNOTE_PARAGRAPH_H
#define INCLUDED_XSLFO_FOOTNOTE_PARAGRAPH_H


#include "libraries.h"


class XslFoFootnoteParagraph
{
public:
  XslFoFootnoteParagraph (ustring marker_i, double fontsize_i,
                          ustring italic_i, ustring bold_i, ustring underline_i, ustring smallcaps_i,
                          ustring justification_i,
                          double spacebefore_i, double spaceafter_i, 
                          double leftmargin_i, double rightmargin_i, double firstlineindent_i,
                          bool apocrypha_i);
  ~XslFoFootnoteParagraph ();
  ustring marker_open;
  ustring marker_close;
  double fontsize;
  ustring italic;
  ustring bold;
  ustring underline;
  ustring smallcaps;
  ustring justification;
  double spacebefore;
  double spaceafter;
  double leftmargin;
  double rightmargin;
  double firstlineindent;
  bool apocrypha;
private:
};


#endif
