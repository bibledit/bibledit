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
 ** Foundation, Inc.,  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **  
 */

#ifndef INCLUDED_MAINWINDOW_H
#define INCLUDED_MAINWINDOW_H

#include <gtk/gtk.h>
#include "windowsoutpost.h"
#include "ustring.h"
#include "bibletime.h"
#include "usfm.h"
#include "note_editor.h"
#include "editor.h"
#include "httpd.h"
#include "reference.h"
#include "gui_navigation.h"
#include "displayprojectnotes.h"
#include "git.h"
#include "windowlayout.h"
#include "windowshowkeyterms.h"
#include "windowshowquickrefs.h"
#include "windowmerge.h"
#include "windowresource.h"
#include "windowoutline.h"
#include "windowcheckkeyterms.h"
#include "windowstyles.h"
#include "windownotes.h"
#include "windowreferences.h"
#include "windoweditor.h"

class MainWindow
{
public:
  MainWindow(unsigned long xembed);
  ~MainWindow();
  int run();
  GtkWidget *mainwindow;
protected:
  /* IMPORTANT If more menu items are added see whether they should be made 
   * in(sensitive) depending on whether a project has been opened. */
  GtkWidget *vbox1;
  GtkWidget *menubar1;
  GtkWidget *menuitem_file;
  GtkWidget *menuitem_file_menu;
  GtkWidget *file_project;
  GtkWidget *image463;
  GtkWidget *file_project_menu;
  GtkWidget *new1;
  GtkWidget *image903;
  GtkWidget *open1;
  GtkWidget *delete1;
  GtkWidget *properties1;
  GtkWidget *image4995;
  GtkWidget *import1;
  GtkWidget *image464;
  GtkWidget *export_project;
  GtkWidget *image3298;
  GtkWidget *export_project_menu;
  GtkWidget *export_usfm_files;
  GtkWidget *image12814;
  GtkWidget *export_zipped_unified_standard_format_markers1;
  GtkWidget *image17639;
  GtkWidget *to_bibleworks_version_database_compiler;
  GtkWidget *image3299;
  GtkWidget *export_to_sword_module;
  GtkWidget *image11392;
  GtkWidget *export_opendocument;
  GtkWidget *image15162;
  GtkWidget *copy_project_to;
  GtkWidget *image2688;
  GtkWidget *compare_with1;
  GtkWidget *image2764;
  GtkWidget *project_backup;
  GtkWidget *image18535;
  GtkWidget *project_backup_menu;
  GtkWidget *project_backup_incremental;
  GtkWidget *image18536;
  GtkWidget *project_backup_flexible;
  GtkWidget *image18537;
  GtkWidget *project_changes;
  GtkWidget *image19115;
  GtkWidget *file_projects_merge;
  GtkWidget *file_references;
  GtkWidget *image465;
  GtkWidget *file_references_menu;
  GtkWidget *open_references1;
  GtkWidget *image466;
  GtkWidget *references_save_as;
  GtkWidget *close_references;
  GtkWidget *image468;
  GtkWidget *delete_references;
  GtkWidget *image469;
  GtkWidget *reference_hide;
  GtkWidget *image6483;
  GtkWidget *style;
  GtkWidget *image10735;
  GtkWidget *style_menu;
  GtkWidget *stylesheets_expand_all;
  GtkWidget *stylesheets_collapse_all;
  GtkWidget *style_insert;
  GtkWidget *stylesheet_edit_mode;
  GtkWidget *style_new;
  GtkWidget *style_properties;
  GtkWidget *style_delete;
  GtkWidget *menu_stylesheet;
  GtkWidget *menu_stylesheet_menu;
  GtkWidget *stylesheet_switch;
  GtkWidget *stylesheets_new;
  GtkWidget *stylesheets_delete;
  GtkWidget *stylesheets_rename;
  GtkWidget *stylesheets_import;
  GtkWidget *stylesheets_export;
  GtkWidget *notes2;
  GtkWidget *image936;
  GtkWidget *notes2_menu;
  GtkWidget *new_note;
  GtkWidget *delete_note;
  GtkWidget *image963;
  GtkWidget *import_notes;
  GtkWidget *image1455;
  GtkWidget *export_notes;
  GtkWidget *image4068;
  GtkWidget *file_resources;
  GtkWidget *image27365;
  GtkWidget *file_resources_menu;
  GtkWidget *file_resources_open;
  GtkWidget *image27366;
  GtkWidget *file_resources_close;
  GtkWidget *image27367;
  GtkWidget *file_resources_new;
  GtkWidget *image27514;
  GtkWidget *file_resources_edit;
  GtkWidget *image27515;
  GtkWidget *file_resources_delete;
  GtkWidget *image27664;
  GtkWidget *print;
  GtkWidget *quit1;
  GtkWidget *menuitem_edit;
  GtkWidget *menuitem_edit_menu;
  GtkWidget *cut1;
  GtkWidget *copy1;
  GtkWidget *copy_without_formatting;
  GtkWidget *image18220;
  GtkWidget *paste1;
  GtkWidget *separator2;
  GtkWidget *undo1;
  GtkWidget *image295;
  GtkWidget *redo1;
  GtkWidget *image296;
  GtkWidget *separator4;
  GtkWidget *find1;
  GtkWidget *find_and_replace1;
  GtkWidget *find_in_notes1;
  GtkWidget *image1430;
  GtkWidget *separator7;
  GtkWidget *get_references_from_note;
  GtkWidget *image3158;
  GtkWidget *separator15;
  GtkWidget *edit_revert;
  GtkWidget *image19262;
  GtkWidget *separator17;
  GtkWidget *edit_bible_note;
  GtkWidget *image20483;
  GtkWidget *separator21;
  GtkWidget *editstatus;
  GtkWidget *image25815;
  GtkWidget *edit_planning;
  GtkWidget *image26801;
  GtkWidget *menuitem_view;
  GtkWidget *menuitem_view_menu;
  GtkWidget *view_font;
  GtkWidget *image20234;
  GtkWidget *view_font_menu;
  GtkWidget *view_text_font;
  GtkWidget *image20235;
  /* Because of switching to GtkHtml for displaying and editing fonts, the fonts no longer can be set in the menu.
   GtkWidget *view_notes_font;
   GtkWidget *image20236;
   */
  GtkWidget *viewnotes;
  GtkWidget *image2627;
  GtkWidget *view_git_tasks;
  GtkWidget *image18685;
  GtkWidget *parallel_passages1;
  GtkWidget *view_usfm_code;
  GtkWidget *image25006;
  GtkWidget *view_status;
  GtkWidget *image25963;
  GtkWidget *view_planning;
  GtkWidget *image26812;
  GtkWidget *view_screen_layout;
  GtkWidget *view_keyterms;
  GtkWidget *view_quick_references;
  GtkWidget *view_outline;
  GtkWidget *insert1;
  GtkWidget *insert1_menu;
  GtkWidget *standard_text_1;
  GtkWidget *image1963;
  GtkWidget *standard_text_2;
  GtkWidget *image1964;
  GtkWidget *standard_text_3;
  GtkWidget *image1965;
  GtkWidget *standard_text_4;
  GtkWidget *image1966;
  GtkWidget *separator9;
  GtkWidget *current_reference1;
  GtkWidget *image3797;
  GtkWidget *separator20;
  GtkWidget *insert_special_character;
  GtkWidget *image25281;
  GtkWidget *menuitem_goto;
  GtkWidget *menuitem_goto_menu;
  GtkWidget *next_reference1;
  GtkWidget *image608;
  GtkWidget *previous_reference1;
  GtkWidget *image609;
  GtkWidget *goto_next_project;
  GtkWidget *image19528;
  GtkWidget *goto_previous_project;
  GtkWidget *image19529;
  GtkWidget *separator19;
  GtkWidget *references_area1;
  GtkWidget *image4722;
  GtkWidget *notes_area1;
  GtkWidget *image4723;
  GtkWidget *separator11;
  GtkWidget *synchronize_other_programs2;
  GtkWidget *image4931;
  GtkWidget *check1;
  GtkWidget *check1_menu;
  GtkWidget *chapters_and_verses1;
  GtkWidget *image5580;
  GtkWidget *markers1;
  GtkWidget *image5578;
  GtkWidget *markers1_menu;
  GtkWidget *validate_usfms1;
  GtkWidget *image5579;
  GtkWidget *count_usfms1;
  GtkWidget *image6239;
  GtkWidget *compare_usfm1;
  GtkWidget *image6748;
  GtkWidget *check_markers_spacing;
  GtkWidget *image17930;
  GtkWidget *characters1;
  GtkWidget *image6867;
  GtkWidget *characters1_menu;
  GtkWidget *count_characters;
  GtkWidget *image6868;
  GtkWidget *unwanted_patterns;
  GtkWidget *image7494;
  GtkWidget *check_words;
  GtkWidget *image7111;
  GtkWidget *check_words_menu;
  GtkWidget *check_capitalization;
  GtkWidget *image7112;
  GtkWidget *check_repetition;
  GtkWidget *image7238;
  GtkWidget *unwanted_words;
  GtkWidget *image7631;
  GtkWidget *word_count_inventory;
  GtkWidget *image13715;
  GtkWidget *check_punctuation;
  GtkWidget *image7366;
  GtkWidget *check_punctuation_menu;
  GtkWidget *check_matching_pairs;
  GtkWidget *image7367;
  GtkWidget *check_sentence_structure;
  GtkWidget *image28475;
  GtkWidget *check_references;
  GtkWidget *image21826;
  GtkWidget *check_references_menu;
  GtkWidget *check_references_inventory;
  GtkWidget *image21827;
  GtkWidget *check_references_validate;
  GtkWidget *image21828;
  GtkWidget *checks_passages;
  GtkWidget *image24103;
  GtkWidget *checks_passages_menu;
  GtkWidget *check_nt_quotations_from_the_ot;
  GtkWidget *image24104;
  GtkWidget *synoptic_parallel_passages_from_the_nt;
  GtkWidget *image24105;
  GtkWidget *parallels_from_the_ot;
  GtkWidget *image24106;
  GtkWidget *check_key_terms;
  GtkWidget *my_checks;
  GtkWidget *image15438;
  GtkWidget *menutools;
  GtkWidget *menutools_menu;
  GtkWidget *line_cutter_for_hebrew_text1;
  GtkWidget *image13532;
  GtkWidget *notes_transfer;
  GtkWidget *image14659;
  GtkWidget *tool_origin_references_in_bible_notes;
  GtkWidget *image16248;
  GtkWidget *tool_project_notes_mass_update1;
  GtkWidget *image17187;
  GtkWidget *tool_generate_word_lists;
  GtkWidget *image20671;
  GtkWidget *tool_simple_text_corrections;
  GtkWidget *image21054;
  GtkWidget *tool_transfer_project_notes_to_text;
  GtkWidget *image29089;
  GtkWidget *menuitem_preferences;
  GtkWidget *menuitem_preferences_menu;
  GtkWidget *notes_preferences;
  GtkWidget *image2116;
  GtkWidget *printingprefs;
  GtkWidget *image3493;
  GtkWidget *reference_exchange1;
  GtkWidget *image5972;
  GtkWidget *ignored_references1;
  GtkWidget *image6467;
  GtkWidget *prefs_books;
  GtkWidget *image12167;
  GtkWidget *preferences_windows_outpost;
  GtkWidget *image14287;
  GtkWidget *preferences_tidy_text;
  GtkWidget *image16359;
  GtkWidget *preferences_remote_git_repository;
  GtkWidget *image18977;
  GtkWidget *preferences_features;
  GtkWidget *image20936;
  GtkWidget *preferences_password;
  GtkWidget *image20937;
  GtkWidget *preferences_text_replacement;
  GtkWidget *image23181;
  GtkWidget *pdf_viewer1;
  GtkWidget *image24540;
  GtkWidget *preferences_reporting;
  GtkWidget *image25623;
  GtkWidget *preferences_planning;
  GtkWidget *image26888;
  GtkWidget *preferences_graphical_interface;
  GtkWidget *image27031;
  GtkWidget *preferences_filters;
  GtkWidget *image28360;
  GtkWidget *menuitem_help;
  GtkWidget *menuitem_help_menu;
  GtkWidget *help_main;
  GtkWidget *image17520;
  GtkWidget *help_context;
  GtkWidget *image16053;
  GtkWidget *system_log1;
  GtkWidget *image4388;
  GtkWidget *about1;
  GtkWidget *toolbar1;
  GtkWidget *hbox5;
  GtkWidget *hbox7;
  GtkWidget *statuslabel_style;
  GtkWidget *label_git;
  GtkWidget *statusbar1;
  GtkAccelGroup *accel_group;

  // Navigator object.
  GuiNavigation navigation;

  /* Initialization */
  void enable_or_disable_widgets(bool enable);

  /* Menu callbacks */
  static void on_open1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void open();
  static void on_new1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void newproject();
  static void on_properties1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void editproject();
  static void on_delete1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void deleteproject();
  static void on_quit1_activate(GtkMenuItem * menuitem, gpointer user_data);
  static void on_system_log1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void viewlog();
  static void on_help_context_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_help_context();
  static void on_help_main_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_help_main();
  static void on_about1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void showabout();
  static void on_undo1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void menu_undo();
  static void on_redo1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void menu_redo();
  static void on_edit1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void menu_edit();
  static void on_find_and_replace1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void menu_replace();
  static void on_findspecial1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void menu_findspecial();
  static void on_import1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void menu_import();
  static gboolean on_mainwindow_delete_event(GtkWidget *widget, GdkEvent *event, gpointer user_data);
  static void on_insert1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_insert();
  static void on_notes_preferences_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_notes_preferences();
  static void on_copy_project_to_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_copy_project_to();
  static void on_compare_with1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_compare_with();
  static void on_printingprefs_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_printing_preferences();
  static void on_prefs_books_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_prefs_books();
  static void on_preferences_tidy_text_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_tidy_text();
  static void on_tool_generate_word_lists_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_tool_generate_word_lists();
  static void on_preferences_features_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_features();
  static void on_preferences_password_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_password();
  static void on_tool_simple_text_corrections_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_tool_simple_text_corrections();
  static void on_preferences_text_replacement_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_text_replacement();
  static void on_parallel_passages1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_parallel_passages1();
  static void on_pdf_viewer1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_pdf_viewer();
  static void on_view_usfm_code_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_usfm_code();
  static void on_insert_special_character_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_insert_special_character();
  static void on_preferences_graphical_interface_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_graphical_interface();

  /* Navigation  */
  static void on_navigation_new_reference_clicked(GtkButton *button, gpointer user_data);
  void on_navigation_new_reference();
  void goto_next_verse();
  void goto_previous_verse();
  void goto_next_chapter();
  void goto_previous_chapter();
  void goto_next_book();
  void goto_previous_book();
  void goto_reference_interactive();
  void go_to_new_reference();
  static void on_new_verse_signalled(GtkButton *button, gpointer user_data);
  void on_new_verse();
  void on_text_area_activate();
  static void on_tools_area1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_tools_area_activate();
  static void on_notes_area1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_notes_area_activate();
  static void on_synchronize_other_programs2_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_synchronize_other_programs();
  static void on_goto_next_project_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_goto_previous_project_activate(GtkMenuItem *menuitem, gpointer user_data);

  /* Clipboard */
  static void on_cut1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_cut();
  static void on_copy1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_copy();
  static void on_copy_without_formatting_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_copy_without_formatting();
  static void on_paste1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_paste();

  /* References */
  void show_references_window();
  WindowReferences * window_references;
  static void on_window_references_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_references_delete_button();
  static void on_window_references_general_signal_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_references_general_signal_button();
  void on_list_goto();
  static void on_open_references1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_open_references();
  static void on_references_save_as_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_save_references();
  static void on_close_references_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_clear_references();
  static void on_delete_references_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_delete_references();
  static void on_next_reference1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_next_reference();
  static void on_previous_reference1_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_previous_reference();
  static void on_ignored_references1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_ignored_references();
  static void on_reference_hide_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_reference_hide();
  void treeview_references_display_quick_reference();

  // Bibledit Windows Outpost
  // BibleTime
  static bool mainwindow_on_external_programs_timeout(gpointer data);
  bool on_external_programs_timeout();
  WindowsOutpost * windowsoutpost;
  static void on_reference_exchange1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_reference_exchange();
  BibleTime bibletime;
  int got_new_bt_reference;
  ustring bibledit_bt_previous_reference;
  ustring bibletime_previous_reference;
  static void on_send_word_to_toolbox_signalled(GtkButton *button, gpointer user_data);
  void send_word_to_toolbox();
  static void on_preferences_windows_outpost_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_windows_outpost();

  /* Title bar and status bar, GUI */
  static bool on_gui_timeout(gpointer data);
  void on_gui();
  gint editor_undo_redo_accelerator_state;

  /* Project notes */
  bool project_notes_enabled;
  void view_project_notes();
  WindowNotes * window_notes;
  static void on_window_notes_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_notes_delete_button();
  static void on_new_note_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_new_note();
  static void on_delete_note_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_viewnotes_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_notes();
  void notes_redisplay();
  static void on_find_in_notes1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void find_in_notes();
  static void on_import_notes_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_import_notes();
  static void on_export_notes_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_export_notes();
  static void on_standard_text_1_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_standard_text_2_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_standard_text_3_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_standard_text_4_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_insert_standard_text(GtkMenuItem *menuitem);
  static void on_current_reference1_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_menuitem_view_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menuitem_view();
  static void on_get_references_from_note_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_get_references_from_note();
  void notes_get_references_from_id(gint id);

  /* Export */
  static void on_export_usfm_files_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_export_zipped_unified_standard_format_markers1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_export_usfm_files(bool zipped);
  static void on_to_bibleworks_version_compiler_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_to_bibleworks_version_compiler();
  static void on_export_to_sword_module_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_export_to_sword_module();
  static void on_export_opendocument_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_export_opendocument();

  /* Checks */
  static void on_check1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_check();
  static void on_markers1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_check_markers();
  static void on_validate_usfms1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_check_markers_validate();
  static void on_count_usfms1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_check_markers_count();
  static void on_compare_usfm1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_check_markers_compare();
  static void on_chapters_and_verses1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_menu_check_chapters_and_verses();
  static void on_count_characters_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_count_characters();
  static void on_unwanted_patterns_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_unwanted_patterns();
  static void on_check_capitalization_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_capitalization();
  static void on_check_repetition_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_repetition();
  static void on_check_matching_pairs_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_matching_pairs();
  static void on_unwanted_words_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_unwanted_words();
  static void on_word_count_inventory_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_word_count_inventory();
  static bool on_check_httpd_timeout(gpointer data);
  void on_check_httpd();
  static void on_my_checks_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_my_checks();
  static void on_check_markers_spacing_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_markers_spacing();
  static void on_check_references_inventory_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_references_inventory();
  static void on_check_references_validate_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_references_validate();
  static void on_check_nt_quotations_from_the_ot_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_nt_quotations_from_the_ot();
  static void on_synoptic_parallel_passages_from_the_nt_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_synoptic_parallel_passages_from_the_nt();
  static void on_parallels_from_the_ot_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_parallels_from_the_ot();
  static void on_check_sentence_structure_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_sentence_structure();

  /* Styles */
  void on_goto_styles_area();
  static void on_file_styles_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_styles();
  WindowStyles * window_styles;
  static void on_window_styles_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_styles_delete_button();
  void stylesheet_open_named(const ustring& stylesheet);
  static void on_style_button_open_clicked(GtkButton *button, gpointer user_data);
  void on_style_button_open();
  static void on_style_button_apply_clicked(GtkButton *button, gpointer user_data);
  void on_style_apply();
  static void on_editor_style_changed(GtkButton *button, gpointer user_data);
  void editor_style_changed();
  static void on_style_edited(GtkButton *button, gpointer user_data);

  /* Footnotes, endnotes, crossreferences */
  static void on_edit_bible_note_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_edit_bible_note();

  /* Tools */
  static void on_menutools_activate(GtkMenuItem *menuitem, gpointer user_data);
  static void on_line_cutter_for_hebrew_text1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_line_cutter_for_hebrew_text();
  static void on_notes_transfer_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_notes_transfer();
  static void on_tool_origin_references_in_bible_notes_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_tool_origin_references_in_bible_notes();
  static void on_tool_project_notes_mass_update1_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_tool_project_notes_mass_update();
  static void on_tool_transfer_project_notes_to_text_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_tool_transfer_project_notes_to_text();

  /* Webserver */
  Httpd httpd;

  /* Keyterms */
  static void on_check_key_terms_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_check_key_terms();
  WindowCheckKeyterms * window_check_keyterms;
  static void on_window_check_keyterms_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_check_keyterms_delete_button();
  static void on_keyterms_new_reference(GtkButton *button, gpointer user_data);
  void check_move_new_reference();
  static void on_view_keyterms_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_keyterms();
  WindowShowKeyterms * window_show_keyterms;
  static void on_window_show_keyterms_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_show_keyterms_delete_button();

  /* Backups */
  static void on_project_backup_incremental_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_project_backup_incremental();
  static void on_project_backup_flexible_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_project_backup_flexible();

  /* Git */
  static void on_view_git_tasks_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_git_tasks();
  static void on_preferences_remote_git_repository_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_remote_git_repository();
  void on_git_reopen_project();
  static void on_project_changes_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_project_changes();
  static void on_edit_revert_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_edit_revert();
  void git_update_intervals_initialize();
  map <ustring, unsigned int> git_update_intervals;
  unsigned int git_update_interval_event_id;
  static bool on_git_update_timeout(gpointer user_data);
  void git_update_timeout();
  GitChapterState * gitchapterstate;
  bool git_reopen_project;

  /* Fonts */
  static void on_view_text_font_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_text_font();
  static void on_view_notes_font_activate(GtkMenuItem * menuitem, gpointer user_data);
  void on_notes_font();
  void set_fonts();

  /* Outline */
  static void on_view_outline_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_outline();
  WindowOutline * window_outline;
  static void on_window_outline_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_outline_delete_button();
  static void on_button_outline_clicked(GtkButton *button, gpointer user_data);
  void on_button_outline();

  /* Interprocess communications */
  static void on_ipc_method_called(GtkButton *button, gpointer user_data);
  void on_ipc_method();

  /* Reporting and Planning */
  static void on_preferences_reporting_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_reporting();
  static void on_editstatus_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_editstatus();
  static void on_view_status_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_status();
  static void on_edit_planning_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_edit_planning();
  static void on_view_planning_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_planning();
  static void on_preferences_planning_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_planning();

  /* Resources */
  vector <WindowResource *> resource_windows;
  static void on_window_resource_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_resource_delete_button(GtkButton *button);
  WindowResource * last_focused_resource_window();
  static void on_file_resources_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_resources();
  static void on_file_resources_open_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_resources_open(ustring resource);
  static void on_file_resources_close_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_resources_close();
  static void on_file_resources_new_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_resources_new();
  static void on_file_resources_edit_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_resources_edit();
  static void on_file_resources_delete_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_resources_delete();

  /* Text editors */// Todo working here
  vector <WindowEditor *> editor_windows;
  static void on_window_editor_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_editor_delete_button(GtkButton *button);
  WindowEditor * last_focused_editor_window();
  void on_file_project_open(const ustring& project);
  void handle_editor_focus();
  void save_editors();

  
  static void on_editor_reload_clicked(GtkButton *button, gpointer user_data);
  void on_editor_reload();
  void jump_start_editors(const ustring& project);
  static void on_editorsgui_changed_clicked(GtkButton *button, gpointer user_data);
  void on_editorsgui_changed();
  void reload_project();

  /* Merge */
  static void on_file_projects_merge_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_file_projects_merge();
  WindowMerge * window_merge;
  static void on_window_merge_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_merge_delete_button();
  static void on_merge_window_get_text_button_clicked(GtkButton *button, gpointer user_data);
  void on_merge_window_get_text_button();
  static void on_merge_window_new_reference_button_clicked(GtkButton *button, gpointer user_data);
  void on_merge_window_new_reference_button();
  static void on_merge_window_save_editors_button_clicked(GtkButton *button, gpointer user_data);
  void on_merge_window_save_editors_button();

  /* Diglot */
  static void on_preferences_filters_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_preferences_filters();

  /* Print */
  static void on_print_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_print();

  /* Windowing system */
  static void on_view_screen_layout_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_screen_layout();
  WindowLayout * window_screen_layout;
  static void on_window_screen_layout_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_screen_layout_button();
  static bool on_windows_startup_timeout(gpointer data);
  bool on_windows_startup();
  unsigned int windows_startup_pointer;
  ustring focused_project_last_session;
  void shutdown_windows();
  static void on_window_focus_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_focus_button(GtkButton *button);
  GtkWidget * now_focused_signal_button;
  GtkWidget * last_focused_signal_button;
  void present_windows(GtkWidget * widget);
  static gboolean on_mainwindow_focus_in_event (GtkWidget *widget, GdkEventFocus *event, gpointer user_data);
  void mainwindow_focus_in_event(GdkEventFocus *event);
  static gboolean on_mainwindow_window_state_event (GtkWidget *widget, GdkEvent *event, gpointer user_data);
  void mainwindow_window_state_event(GdkEvent *event);

  /* Quick references */
  static void on_view_quick_references_activate(GtkMenuItem *menuitem, gpointer user_data);
  void on_view_quick_references();
  WindowShowQuickReferences * window_show_quick_references;
  static void on_window_show_quick_references_delete_button_clicked(GtkButton *button, gpointer user_data);
  void on_window_show_quick_references_delete_button();
  static void on_show_quick_references_signal_button_clicked(GtkButton *button, gpointer user_data);
  void on_show_quick_references_signal_button(GtkButton *button);

  // Accelerators.
  GtkAccelGroup *accelerator_group;
  static void accelerator_undo_callback(gpointer user_data);
  void accelerator_undo();
  static void accelerator_redo_callback(gpointer user_data);
  void accelerator_redo();
  static void accelerator_cut_callback(gpointer user_data);
  void accelerator_cut();
  static void accelerator_copy_callback(gpointer user_data);
  void accelerator_copy();
  static void accelerator_paste_callback(gpointer user_data);
  void accelerator_paste();
  static void accelerator_standard_text_1_callback(gpointer user_data);
  static void accelerator_standard_text_2_callback(gpointer user_data);
  static void accelerator_standard_text_3_callback(gpointer user_data);
  static void accelerator_standard_text_4_callback(gpointer user_data);
  void accelerator_standard_text_n(unsigned int selector);
  static void accelerator_new_project_note_callback(gpointer user_data);
  static void accelerator_next_verse_callback(gpointer user_data);
  static void accelerator_previous_verse_callback(gpointer user_data);
  static void accelerator_next_chapter_callback(gpointer user_data);
  static void accelerator_previous_chapter_callback(gpointer user_data);
  static void accelerator_next_book_callback(gpointer user_data);
  static void accelerator_previous_book_callback(gpointer user_data);
  static void accelerator_next_reference_in_history_callback(gpointer user_data);
  void accelerator_next_reference_in_history();
  static void accelerator_previous_reference_in_history_callback(gpointer user_data);
  void accelerator_previous_reference_in_history();
  static void accelerator_go_to_reference_callback(gpointer user_data);
  static void accelerator_close_window_callback(gpointer user_data);
  void accelerator_close_window();
  static void accelerator_goto_styles_area_callback(gpointer user_data);
  static void accelerator_quit_program_callback(gpointer user_data);
  static void accelerator_activate_text_area_callback(gpointer user_data);

};

#endif
