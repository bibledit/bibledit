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


#ifndef INCLUDED_DIALOG_RADIOBUTTON_H
#define INCLUDED_DIALOG_RADIOBUTTON_H


#include <gtk/gtk.h>
#include "ustring.h"


class RadiobuttonDialog
{
public:
  RadiobuttonDialog (const ustring& title, const ustring& info, const vector <ustring>& labels, unsigned int selection);
  ~RadiobuttonDialog ();
  int run ();
  unsigned int selection;
protected:
  GtkWidget *radiobuttondialog;
  GtkWidget *dialog_vbox1;
  GtkWidget *label;
  GtkWidget *radiobutton;
  GtkWidget *dialog_action_area1;
  GtkWidget *cancelbutton;
  GtkWidget *okbutton;
private:
  vector <GtkWidget *> radiobuttons;
  static void on_okbutton_clicked (GtkButton *button, gpointer user_data);
  void on_okbutton ();
};


#endif
