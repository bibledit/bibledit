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


#include <manage/hyphenation.h>
#include <assets/view.h>
#include <assets/page.h>
#include <filter/roles.h>
#include <filter/string.h>
#include <dialog/list.h>
#include <access/bible.h>
#include <locale/translate.h>
#include <tasks/logic.h>
#include <database/config/bible.h>


const char * manage_hyphenation_url ()
{
  return "manage/hyphenation";
}


bool manage_hyphenation_acl (void * webserver_request)
{
  return Filter_Roles::access_control (webserver_request, Filter_Roles::manager ());
}


string manage_hyphenation (void * webserver_request)
{
  Webserver_Request * request = (Webserver_Request *) webserver_request;
  
  
  string page;
  page = Assets_Page::header ("Hyphenation", webserver_request);
  Assets_View view;
  
  
  string bible = access_bible_clamp (webserver_request, request->database_config_user()->getBible ());
  
  
  string success;
  string error;
  
  
  // Character sets submission.
  if (request->post.count ("sets")) {
    string firstset = request->post["firstset"];
    Database_Config_Bible::setHyphenationFirstSet (bible, firstset);
    string secondset = request->post["secondset"];
    Database_Config_Bible::setHyphenationSecondSet (bible, secondset);
    success = translate("The two sets of characters were saved");
  }
  
  
  if (request->query.count ("bible")) {
    string bible = request->query ["bible"];
    if (bible == "") {
      Dialog_List dialog_list = Dialog_List ("", translate("Which Bible would you like to take the data from?"), "", "");
      vector <string> bibles = access_bible_bibles (webserver_request);
      for (auto bible : bibles) {
        dialog_list.add_row (bible, "bible", bible);
      }
      page += dialog_list.run();
      return page;
    } else {
      request->database_config_user()->setBible (bible);
    }
  }
  bible = access_bible_clamp (webserver_request, request->database_config_user()->getBible ());
  
  
  string firstset = Database_Config_Bible::getHyphenationFirstSet (bible);
  string secondset = Database_Config_Bible::getHyphenationSecondSet (bible);
  if (request->query.count ("run")) {
    if (bible == "") {
      error = translate("No Bible given");
    } else if (firstset == "") {
      error = translate("No first set of characters given");
    } else if (secondset == "") {
      error = translate("No second set of characters given");
    } else {
      tasks_logic_queue (HYPHENATE, {bible, request->session_logic()->currentUser ()});
      success = translate("The Bible is being hyphenated.") + " " + translate ("See the Journal for details.");
    }
  }
  
  
  view.set_variable ("firstset", firstset);
  view.set_variable ("secondset", secondset);
  view.set_variable ("bible", bible);
  view.set_variable ("success", success);
  view.set_variable ("error", error);
  
  
  page += view.render ("manage", "hyphenation");
  page += Assets_Page::footer ();
  return page;
}
