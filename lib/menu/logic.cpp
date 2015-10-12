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


#include <menu/logic.h>
#include <menu/index.h>
#include <filter/string.h>
#include <database/config/general.h>
#include <locale/translate.h>
#include <index/index.h>
#include <journal/index.h>
#include <edit/index.h>
#include <editone/index.h>
#include <editusfm/index.h>
#include <editverse/index.h>
#include <workbench/organize.h>
#include <workbench/index.h>
#include <workbench/logic.h>
#include <notes/index.h>
#include <resource/index.h>
#include <search/index.h>
#include <email/index.h>
#include <manage/index.h>
#include <manage/users.h>
#include <administration/language.h>
#include <administration/timezone.h>
#include <styles/indext.h>
#include <styles/indexm.h>
#include <versification/index.h>
#include <bible/manage.h>
#include <workbench/logic.h>
#include <collaboration/index.h>
#include <client/index.h>
#include <client/logic.h>
#include <sendreceive/index.h>
#include <config/globals.h>
#include <resource/manage.h>
#include <resource/admin.h>
#include <resource/print.h>
#include <mapping/index.h>
#include <changes/changes.h>
#include <changes/manage.h>
#include <index/listing.h>
#include <sprint/index.h>
#include <checks/index.h>
#include <checks/settings.h>
#include <consistency/index.h>
#include <manage/exports.h>
#include <manage/hyphenation.h>
#include <xrefs/index.h>
#include <debug/index.h>
#include <paratext/index.h>
#include <personalize/index.h>
#include <resource/images.h>
#include <resource/sword.h>
#include <workbench/logic.h>
#include <workbench/organize.h>
#include <config/logic.h>
#include <session/logout.h>
#include <session/login.h>
#include <user/notifications.h>
#include <user/account.h>


string menu_logic_href (string href)
{
  href = filter_string_str_replace ("?", "__q__", href);
  href = filter_string_str_replace ("&", "__a__", href);
  href = filter_string_str_replace ("=", "__i__", href);
  href.insert (0, "?item=");
  href.insert (0, menu_index_url ());
  href.insert (0, "/");
  return href;
}


string menu_logic_click (string item)
{
  item = filter_string_str_replace ("__q__", "?", item);
  item = filter_string_str_replace ("__a__", "&", item);
  item = filter_string_str_replace ("__i__", "=", item);
  Database_Config_General::setLastMenuClick (item);
  return item;
}


string menu_logic_create_item (string href, string text, bool history)
{
  string item;
  item.append ("<span>");
  item.append ("<a href=\"/");
  if (history) {
    item.append (menu_index_url ());
  } else {
    item.append (index_index_url ());
  }
  item.append ("?item=");
  item.append (href + "\">" + text + "</a>");
  item.append ("</span>");
  return item;
}


string menu_logic_translate_text ()
{
  return translate ("Translate");
}


string menu_logic_search_text ()
{
  return translate ("Search");
}


string menu_logic_tools_text ()
{
  return translate ("Tools");
}


string menu_logic_settings_text ()
{
  return translate ("Settings");
}


string menu_logic_help_text ()
{
  return translate ("Help");
}


// Returns the html for the main menu categories.
string menu_logic_main_categories (void * webserver_request)
{
  vector <string> html;

  if (!menu_logic_translate_category (webserver_request).empty ()) {
    html.push_back (menu_logic_create_item ("translate", menu_logic_translate_text (), false));
  }
  
  if (!menu_logic_search_category (webserver_request).empty ()) {
    html.push_back (menu_logic_create_item ("search", menu_logic_search_text (), false));
  }

  if (!menu_logic_tools_category (webserver_request).empty ()) {
    html.push_back (menu_logic_create_item ("tools", menu_logic_tools_text (), false));
  }

  if (!menu_logic_settings_category (webserver_request).empty ()) {
    html.push_back (menu_logic_create_item ("settings", menu_logic_settings_text (), false));
  }
  
  if (!menu_logic_help_category (webserver_request).empty ()) {
    html.push_back (menu_logic_create_item ("help/index", menu_logic_help_text (), true));
  }

  return filter_string_implode (html, "\n");
}


/*
 This generates a start menu.
 
 It goes through all possible menu entries.
 It reads the access levels of those entries.
 It takes the menu entries the currently logged-in user has access to.
 
 It originally self-organized the entries such that the ones used clicked often came earlier in the menu.
 But menu entries moving around creates confusion.
 Therefore it was removed again.
*/


string menu_logic_translate_category (void * webserver_request)
{
  vector <string> html;
  
  // Add the available configured desktops to the menu.
  // The user's role should be sufficiently high.
  if (workbench_organize_acl (webserver_request)) {
    vector <string> workbenches = workbenchGetWorkbenches (webserver_request);
    for (size_t i = 0; i < workbenches.size(); i++) {
      html.push_back (menu_logic_create_item (workbench_index_url () + "?bench=" + convert_to_string (i), workbenches[i] + " " + translate ("desktop"), true));
    }
  }

  if (edit_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (edit_index_url (), translate ("Visual chapter editor"), true));
  }
  
  if (editone_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (editone_index_url (), translate ("Visual verse editor"), true));
  }

  if (editusfm_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (editusfm_index_url (), translate ("USFM chapter editor"), true));
  }
    
  if (editverse_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (editverse_index_url (), translate ("USFM verse editor"), true));
  }
  
  if (notes_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (notes_index_url (), translate ("Consultation notes"), true));
  }

  if (resource_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (resource_index_url (), translate ("View resources"), true));
  }

  if (changes_changes_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (changes_changes_url (), translate ("Change notifications"), true));
  }

  if (index_listing_acl (webserver_request, "exports")) {
    html.push_back (menu_logic_create_item (index_listing_url ("exports"), translate ("Exports"), true));
  }
  
  if (!html.empty ()) {
    html.insert (html.begin (), menu_logic_translate_text () + ": ");
  }

  return filter_string_implode (html, "\n");
}


string menu_logic_search_category (void * webserver_request)
{
  vector <string> html;

  if (search_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (search_index_url (), translate ("Search"), true));
  }
  
  if (!html.empty ()) {
    html.insert (html.begin (), menu_logic_search_text () + ": ");
  }
  
  return filter_string_implode (html, "\n");
}


string menu_logic_tools_category (void * webserver_request)
{
  vector <string> html;

  if (checks_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (checks_index_url (), translate ("Mechanical checks"), true));
  }

  if (consistency_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (consistency_index_url (), translate ("Consistency"), true));
  }

  if (!config_logic_client_prepared ()) {
    if (resource_print_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (resource_print_url (), translate ("Print resources"), true));
    }
  }

  // Downloading revisions only on server, not on client.
  if (!config_logic_client_prepared ()) {
    if (index_listing_acl (webserver_request, "revisions")) {
      html.push_back (menu_logic_create_item (index_listing_url ("revisions"), translate ("Download changes"), true));
    }
  }

  if (!config_logic_client_prepared ()) {
    if (sprint_index_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (sprint_index_url (), translate ("Planning"), true));
    }
  }

  if (sendreceive_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (sendreceive_index_url (), translate ("Send / receive"), true));
  }

  if (manage_hyphenation_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (manage_hyphenation_url (), translate ("Hyphenation"), true));
  }

  if (xrefs_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (xrefs_index_url (), translate ("Cross references"), true));
  }
  
  if (debug_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (debug_index_url (), translate ("Debug"), true));
  }

  if (manage_exports_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (manage_exports_url (), translate ("Export Bibles"), true));
  }

  if (journal_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (journal_index_url (), translate ("Journal"), true));
  }
  
  if (!html.empty ()) {
    html.insert (html.begin (), menu_logic_tools_text () + ": ");
  }
  
  return filter_string_implode (html, "\n");
}


string menu_logic_settings_category (void * webserver_request)
{
  bool client = client_logic_client_enabled ();
  bool demo = config_logic_demo_enabled ();

  vector <string> html;

  if (bible_manage_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (bible_manage_url (), translate ("Bibles"), true));
  }
  
  if (workbench_organize_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (workbench_organize_url (), translate ("Desktops"), true));
  }

  if (checks_settings_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (checks_settings_url (), translate ("Checks"), true));
  }

  if (!config_logic_client_prepared ()) {
    if (resource_manage_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (resource_manage_url (), translate ("USFM resources"), true));
    }
  }

  if (!config_logic_client_prepared ()) {
    if (resource_admin_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (resource_admin_url (), translate ("External resources"), true));
    }
  }
  
  if (!config_logic_client_prepared ()) {
    if (resource_images_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (resource_images_url (), translate ("Image resources"), true));
    }
  }
  
  if (!config_logic_client_prepared ()) {
    if (resource_sword_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (resource_sword_url (), translate ("SWORD resources"), true));
    }
  }

  // Managing change notifications only on server, not on client.
  if (!config_logic_client_prepared ()) {
    if (changes_manage_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (changes_manage_url (), translate ("Change notifications"), true));
    }
  }
  
  if (personalize_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (personalize_index_url (), translate ("Personalize"), true));
  }

  if (!config_logic_client_prepared ()) {
    if (manage_users_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (manage_users_url (), translate ("Users"), true));
    }
  }

  if (manage_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (manage_index_url (), translate ("Indexes and Fonts"), true));
  }

  if (administration_language_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (administration_language_url (), translate ("Language"), true));
  }

  if (administration_timezone_acl (webserver_request)) {
    // Display menu to set the site's timezone only in case the calling program has not yet set this zone in the library.
    // So for example the app for iOS can set the timezone from the device, and in case this has been done,
    // then the user no longer can set it through Bibledit.
    if ((config_globals_timezone_offset_utc < MINIMUM_TIMEZONE)
        || (config_globals_timezone_offset_utc > MAXIMUM_TIMEZONE)) {
      html.push_back (menu_logic_create_item (administration_timezone_url (), translate ("Timezone"), true));
    }
  }

  if (!config_logic_client_prepared ()) {
    if (email_index_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (email_index_url (), translate ("Mail"), true));
    }
  }
  
  if (styles_indext_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (styles_indext_url (), translate ("Select stylesheet"), true));
  }
  
  if (versification_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (versification_index_url (), translate ("Versifications"), true));
  }
  
  if (mapping_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (mapping_index_url (), translate ("Verse mappings"), true));
  }
  
  if (collaboration_index_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (collaboration_index_url (), translate ("Collaboration"), true));
  }
  
  // If the installation is not prepared for Client mode, disable the client menu.
  // But keep the menu item in an open installation.
  bool client_menu = client_index_acl (webserver_request);
  if (!config_logic_client_prepared ()) client_menu = false;
  if (config_logic_demo_enabled ()) client_menu = true;
  if (client_menu) {
    if (client_index_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (client_index_url (), translate ("Cloud"), true));
    }
  }
  
  // Paratext can be enabled through ./configure --enable-paratext.
  if (config_logic_paratext_enabled ()) {
    if (paratext_index_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (paratext_index_url (), translate ("Paratext"), true));
    }
  }
  
  if (styles_indexm_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (styles_indexm_url (), translate ("Edit stylesheet"), true));
  }

  if (!(client || demo)) {
    string username = ((Webserver_Request *) webserver_request)->session_logic ()->currentUser ();
    if (!username.empty ()) {
      if (session_logout_acl (webserver_request)) {
        html.push_back (menu_logic_create_item (session_logout_url (), translate ("Logout"), true));
      }
    }
  }

  if (user_notifications_acl (webserver_request)) {
    html.push_back (menu_logic_create_item (user_notifications_url (), translate ("Notifications"), true));
  }

  if (!(client || demo)) {
    if (user_account_acl (webserver_request)) {
      html.push_back (menu_logic_create_item (user_account_url (), translate ("Account"), true));
    }
  }

  if (!html.empty ()) {
    html.insert (html.begin (), menu_logic_settings_text () + ": ");
  }
  
  return filter_string_implode (html, "\n");
}


// Not now in use.
// Kept for the future.
string menu_logic_help_category (void * webserver_request)
{
  if (webserver_request) {};
  
  vector <string> html;

  html.push_back (menu_logic_create_item ("help/index", translate ("Help and About"), true));

  if (!html.empty ()) {
    html.insert (html.begin (), menu_logic_help_text () + ": ");
  }
  
  return filter_string_implode (html, "\n");
}
