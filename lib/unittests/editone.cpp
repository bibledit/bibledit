/*
Copyright (©) 2003-2016 Teus Benschop.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/


#include <unittests/editone.h>
#include <unittests/utilities.h>
#include <styles/logic.h>
#include <filter/url.h>
#include <editone/logic.h>


// Test the logic used in the visual verse editor.
void test_editone_logic ()
{
  trace_unit_tests (__func__);

  string stylesheet = styles_logic_standard_sheet ();
  string directory = filter_url_create_root_path ("unittests", "tests");

  // Prefix.
  {
    string usfm = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_1.usfm"));
    string html;
    string last_paragraph_style;
    editone_logic_prefix_html_stage_one (usfm, stylesheet, html, last_paragraph_style);
    string standard = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_1.html"));
    evaluate (__LINE__, __func__, standard, html);
    evaluate (__LINE__, __func__, "p", last_paragraph_style);
  }
  
  // Editable verse text.
  {
    // Convert USFM to html.
    string usfm = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_2.usfm"));
    string html;
    string last_paragraph_style;
    string focused_verse_applied_p_style;
    editone_old_logic_editable_html ("p", usfm, stylesheet, html, last_paragraph_style, focused_verse_applied_p_style);
    string standard = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_2.html"));
    evaluate (__LINE__, __func__, standard, html);
    evaluate (__LINE__, __func__, "p", last_paragraph_style);
    evaluate (__LINE__, __func__, "p", focused_verse_applied_p_style);
    
    // Convert the html back to USFM again.
    string round_tripped_usfm = editone_old_logic_html_to_usfm (stylesheet, html, focused_verse_applied_p_style);
    evaluate (__LINE__, __func__, usfm, round_tripped_usfm);
  }
  
  // Editable verse text including a \b.
  {
    // Convert USFM to html.
    string usfm = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_4.usfm"));
    string html;
    string last_paragraph_style;
    string focused_verse_applied_p_style;
    editone_old_logic_editable_html ("p", usfm, stylesheet, html, last_paragraph_style, focused_verse_applied_p_style);
    string standard = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_4.html"));
    evaluate (__LINE__, __func__, standard, html);
    evaluate (__LINE__, __func__, "q1", last_paragraph_style);
    evaluate (__LINE__, __func__, "p", focused_verse_applied_p_style);
    
    // Convert the html back to USFM again.
    string round_tripped_usfm = editone_old_logic_html_to_usfm (stylesheet, html, focused_verse_applied_p_style);
    evaluate (__LINE__, __func__, usfm, round_tripped_usfm);
  }

  // Suffix.
  {
    string usfm = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_3.usfm"));
    string html;
    string last_paragraph_style;
    editone_logic_suffix_html ("q1", usfm, stylesheet, html);
    string standard = filter_url_file_get_contents (filter_url_create_path (directory, "edit_one_3.html"));
    evaluate (__LINE__, __func__, standard, html);
    evaluate (__LINE__, __func__, "", last_paragraph_style);
  }
  
  // Removing notes from the prefix and appending them to the notes in the suffix.
  {
    string prefix;
    string suffix;
    string standard;
    prefix = filter_url_file_get_contents (filter_url_create_path (directory, "editone1prefix1.html"));
    suffix = filter_url_file_get_contents (filter_url_create_path (directory, "editone1suffix1.html"));
    editone_logic_move_notes (prefix, suffix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone1prefix2.html"));
    evaluate (__LINE__, __func__, standard, prefix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone1suffix2.html"));
    evaluate (__LINE__, __func__, standard, suffix);
  }

  // Prefix without notes, so moving nothing to the notes in the suffix.
  {
    string prefix;
    string suffix;
    string standard;
    prefix = filter_url_file_get_contents (filter_url_create_path (directory, "editone2prefix1.html"));
    suffix = filter_url_file_get_contents (filter_url_create_path (directory, "editone2suffix1.html"));
    editone_logic_move_notes (prefix, suffix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone2prefix2.html"));
    evaluate (__LINE__, __func__, standard, prefix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone2suffix2.html"));
    evaluate (__LINE__, __func__, standard, suffix);
  }
  
  // Move notes from the prefix to a suffix that does not have notes of its own.
  {
    string prefix;
    string suffix;
    string standard;
    prefix = filter_url_file_get_contents (filter_url_create_path (directory, "editone3prefix1.html"));
    suffix = filter_url_file_get_contents (filter_url_create_path (directory, "editone3suffix1.html"));
    editone_logic_move_notes (prefix, suffix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone3prefix2.html"));
    evaluate (__LINE__, __func__, standard, prefix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone3suffix2.html"));
    evaluate (__LINE__, __func__, standard, suffix);
  }
  
  // Test that a empty prefix works fine when trying to move notes from prefix to suffix.
  {
    string prefix;
    string suffix;
    string standard;
    prefix = filter_url_file_get_contents (filter_url_create_path (directory, "editone4prefix1.html"));
    suffix = filter_url_file_get_contents (filter_url_create_path (directory, "editone4suffix1.html"));
    editone_logic_move_notes (prefix, suffix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone4prefix2.html"));
    evaluate (__LINE__, __func__, standard, prefix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone4suffix2.html"));
    evaluate (__LINE__, __func__, standard, suffix);
  }
  
  // Test that notes from the prefix get moved even to an empty suffix.
  {
    string prefix;
    string suffix;
    string standard;
    prefix = filter_url_file_get_contents (filter_url_create_path (directory, "editone5prefix1.html"));
    suffix = filter_url_file_get_contents (filter_url_create_path (directory, "editone5suffix1.html"));
    editone_logic_move_notes (prefix, suffix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone5prefix2.html"));
    evaluate (__LINE__, __func__, standard, prefix);
    standard = filter_url_file_get_contents (filter_url_create_path (directory, "editone5suffix2.html"));
    evaluate (__LINE__, __func__, standard, suffix);
  }
}
