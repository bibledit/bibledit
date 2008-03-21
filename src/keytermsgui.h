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


#ifndef INCLUDED_KEYTERMS_GUI_H
#define INCLUDED_KEYTERMS_GUI_H


#include "libraries.h"
#include <glib.h>
#include <gtk/gtk.h>
#include "reference.h"


class KeytermsGUI
{
public:
  KeytermsGUI (GtkWidget * notebook_vbox, 
               GtkWidget * textview_check_text_in);
  ~KeytermsGUI ();
  void focus_entry ();
  void copy_clipboard ();
  Reference * new_reference_showing;
  GtkWidget *signal;
  bool focused ();
  void focus ();
private:
  // Widgets.
  GtkWidget *label23;
  GtkWidget *entry_keyterm; 
  GtkWidget *label24;
  GtkWidget *combobox_collection;
  GtkWidget *label25;
  GtkWidget *scrolledwindow_keyterm;
  GtkWidget *treeview_keyterm;
  GtkWidget *scrolledwindow_comments;
  GtkWidget *textview_comments;
  GtkWidget *scrolledwindow_renderings;
  GtkWidget *treeview_renderings;
public:
  GtkWidget *textview_check_text;
private:

  // Underlying constructions.
  GtkTreeStore *treestore_keywords;
  GtkTreeViewColumn *treecolumn_keywords;
  GtkTreeSelection *treeselect_keywords;
  GtkTreeStore *treestore_renderings;
  GtkTreeSelection *treeselect_renderings;

  // Callbacks.
  static void on_entry_keyterm_changed (GtkEditable *editable, gpointer user_data);
  static void on_entry_keyterm_activate (GtkEntry *entry, gpointer user_data);
  static void on_combobox_keyterm_collection_changed (GtkComboBox *combobox, gpointer user_data);
  static gboolean on_treeview_keyterm_button_press_event (GtkWidget *widget,  GdkEventButton *event, gpointer user_data);
  static gboolean on_treeview_keyterm_key_press_event (GtkWidget *widget,  GdkEventKey *event, gpointer user_data);
  static void on_treeview_keyterm_row_activated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
  static void keyterm_whole_word_toggled (GtkCellRendererToggle *cell, gchar *path_str, gpointer data);
  static void keyterm_case_sensitive_toggled (GtkCellRendererToggle *cell, gchar *path_str, gpointer data);  
  static void cell_edited (GtkCellRendererText *cell, const gchar *path_string, const gchar *new_text, gpointer data);
  static gboolean on_textview_keyterm_text_button_press_event (GtkWidget *widget, GdkEventButton *event, gpointer user_data);
  static gboolean on_textview_keyterm_text_button_release_event (GtkWidget *widget, GdkEventButton*event, gpointer user_data);
  static gboolean on_textview_keyterm_text_key_press_event (GtkWidget *widget, GdkEventKey *event, gpointer user_data);

  // Action routines.
  void on_entry_keyterm_change ();
  void on_entry_keyterm_activated ();
  void on_combobox_keyterm_collection ();
  void on_treeview_keyterm_button_press ();
  void on_treeview_keyterm_key_press ();
  void on_treeview_keyterm_activated ();
  void on_treeview_change ();
  static bool treeview_changed_timeout (gpointer data);
  void show_information ();
  void load_renderings ();
  void save_renderings ();
  void on_rendering_toggle (GtkCellRendererToggle *cell, gchar *path_str, bool first_toggle);
  void on_cell_edited (GtkCellRendererText *cell, const gchar *path_string, const gchar *new_text);
  void add_to_renderings (const ustring& rendering, bool wholeword);
  void show_text ();
  void check_text ();
  gboolean on_textview_keyterm_text_button_press (GdkEventButton *event);
  gboolean on_textview_keyterm_text_button_release (GdkEventButton *event);
  gboolean on_textview_keyterm_text_key_press (GdkEventKey *event);
  void check_move_new_reference ();

  // Data routines.
  unsigned int selected_id ();
  ustring all_categories ();
  ustring enter_new_rendering_here ();
  void get_renderings (vector <ustring>& renderings, vector<bool>& wholewords, vector<bool>& casesensitives);

  // Variables.
  GtkTextTag *approved_rendering_tag;    // Tag for showing approved renderings.
  GtkTextTag *disapproved_rendering_tag; // Tag for showing disapproved rendeirngs.
  vector<Reference> myreferences;        // The references in the textview.
  vector<gint> mytextstarts;             // Marks the start location of verse text in the textview.
  vector<gint> mytextends;               // Marks the end location of verse text in the textview.
  unsigned int myid;                     // The id of the keyword now being checked.
  ustring myproject;                     // The project now being checked.
  ustring myversification;               // Versification of project now being checked.
  guint previous_reference_id;           // The previous id of the reference that shows.
  guint signal_id;
};


#endif
