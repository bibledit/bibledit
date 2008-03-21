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


#ifndef INCLUDED_PROGRESSWINDOW_H
#define INCLUDED_PROGRESSWINDOW_H


#include "libraries.h"
#include <gtk/gtk.h>


class ProgressWindow
{
public:
  ProgressWindow (const ustring& info, bool showcancel);
  ~ProgressWindow ();
  void set_text (const ustring& text);
  void set_fraction (double fraction);
  void set_iterate (double minimum, double maximum, unsigned int max_iteration);
  void iterate ();
  void pulse ();
  void hide ();
  bool cancel;
private:
  GtkWidget *progresswindow;
  GtkWidget *vbox3;
  GtkWidget *infolabel;
  GtkWidget *progressbar;
  GtkWidget *hbox6;
  GtkWidget *cancelbutton;
  static void on_cancelbutton_clicked (GtkButton *button, gpointer user_data);
  void on_cancel ();
  static gboolean on_delete_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);
  void gui ();
  double my_minimum;
  double my_maximum;
  unsigned int my_max_iterations;
  unsigned int iteration_counter;
  unsigned int modulus;
};


#endif
