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


#ifndef INCLUDED_OT_QUOTATIONS_H
#define INCLUDED_OT_QUOTATIONS_H


#include "libraries.h"
#include "reference.h"
#include "mapping.h"


class OTQuotation
{
public:
  OTQuotation (int dummy);
  Reference nt;
  vector <Reference> ot;
  bool lxx;
private:
};


class OTQuotations
{
public:
  OTQuotations (int dummy);
  void get (Reference& reference, vector <Reference>& references, vector <ustring>& comments);
  void read ();
  vector <OTQuotation> quotations;
private:
  ustring comment (gchar * text, bool lxx);
};


#endif
