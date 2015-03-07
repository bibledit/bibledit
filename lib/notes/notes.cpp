/*
 Copyright (©) 2003-2015 Teus Benschop.
 
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


#include <notes/notes.h>
#include <assets/view.h>
#include <assets/page.h>
#include <assets/header.h>
#include <filter/roles.h>
#include <filter/usfm.h>
#include <filter/string.h>
#include <webserver/request.h>
#include <locale/translate.h>
#include <database/notes.h>
#include <access/bible.h>
#include <ipc/focus.h>


string notes_notes_url ()
{
  return "notes/notes";
}


bool notes_notes_acl (void * webserver_request)
{
  return Filter_Roles::access_control (webserver_request, Filter_Roles::consultant ());
}


string notes_notes (void * webserver_request)
{
  Webserver_Request * request = (Webserver_Request *) webserver_request;
  Database_Notes database_notes = Database_Notes (webserver_request);

  
  string bible = access_bible_clamp (webserver_request, request->database_config_user()->getBible());
  int book = Ipc_Focus::getBook (webserver_request);
  int chapter = Ipc_Focus::getChapter (webserver_request);
  int verse = Ipc_Focus::getVerse (webserver_request);

  
  int passage_selector = request->database_config_user()->getConsultationNotesPassageSelector();
  int edit_selector = request->database_config_user()->getConsultationNotesEditSelector();
  int non_edit_selector = request->database_config_user()->getConsultationNotesNonEditSelector();
  string status_selector = request->database_config_user()->getConsultationNotesStatusSelector();
  string bible_selector = request->database_config_user()->getConsultationNotesBibleSelector();
  string assignment_selector = request->database_config_user()->getConsultationNotesAssignmentSelector();
  int subscription_selector = request->database_config_user()->getConsultationNotesSubscriptionSelector();
  int severity_selector = request->database_config_user()->getConsultationNotesSeveritySelector();
  int text_selector = request->database_config_user()->getConsultationNotesTextSelector();
  string search_text = request->database_config_user()->getConsultationNotesSearchText();
  int passage_inclusion_selector = request->database_config_user()->getConsultationNotesPassageInclusionSelector();
  int text_inclusion_selector = request->database_config_user()->getConsultationNotesTextInclusionSelector();

  
  // The Bibles the current user has access to.
  vector <string> bibles = access_bible_bibles (webserver_request, request->session_logic()->currentUser ());
  
  
  // The admin disables notes selection on Bibles, so the admin sees all notes, including notes referring to non-existing Bibles.
  if (request->session_logic ()->currentLevel () == Filter_Roles::admin ()) bibles.clear ();
  
  
  vector <int> identifiers = database_notes.selectNotes (bibles, book, chapter, verse, passage_selector, edit_selector, non_edit_selector, status_selector, bible_selector, assignment_selector, subscription_selector, severity_selector, text_selector, search_text, 0);

  
  string notesblock;
  cout << identifiers.size () << endl; // Todo
  for (auto & identifier : identifiers) {

    string summary = database_notes.getSummary (identifier);
    vector <Passage> passages = database_notes.getPassages (identifier);
    string verses = filter_passage_display_inline (passages);
    summary += " | " + verses;

    string verse_text;
    if (passage_inclusion_selector) {
      vector <Passage> passages = database_notes.getPassages (identifier);
      for (auto & passage : passages) {
        string usfm = request->database_bibles()->getChapter (bible, passage.book, passage.chapter);
        string text = usfm_get_verse_text (usfm, convert_to_int (passage.verse));
        if (!verse_text.empty ()) verse_text.append ("<br>");
        verse_text.append (text);
      }
    }
    
    string content;
    if (text_inclusion_selector) {
      content = database_notes.getContents (identifier);
    }
    
    notesblock.append ("<a name=\"note" + to_string (identifier) + "\"></a>\n");
    notesblock.append ("<p><a href=\"note?id=" + to_string (identifier) + "\">" + summary + "</a></p>\n");
    if (!verse_text.empty ()) notesblock.append ("<p>" + verse_text + "</p>\n");
    if (!content.empty ()) notesblock.append ("<p>" + content + "</p>\n");
  }

  
  if (identifiers.empty ()) {
    return translate("This selection does not display any notes.");
  }
  return notesblock;
}
