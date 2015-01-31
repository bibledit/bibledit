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


#include <sendreceive/settings.h>
#include <filter/url.h>
#include <filter/roles.h>
#include <filter/string.h>
#include <tasks/logic.h>
#include <config/logic.h>
#include <database/config/general.h>
#include <database/config/bible.h>
#include <database/logs.h>
#include <database/bibles.h>
#include <client/logic.h>
#include <locale/translate.h>
#include <webserver/request.h>
#include <sync/logic.h>


void sendreceive_send_settings () // Todo
{
  Database_Logs::log (gettext("Sending and receiving Settings"), Filter_Roles::translator ());
  
  Webserver_Request request;

  string response = client_logic_connection_setup ();
  int iresponse = convert_to_int (response);
  if (iresponse < Filter_Roles::guest () || iresponse > Filter_Roles::admin ()) {
    Database_Logs::log (gettext("Failure sending and receiving Settings"), Filter_Roles::translator ());
    return;
  }

  // Set the correct user in the session: The sole user on the Client.
  vector <string> users = request.database_users ()->getUsers ();
  if (users.empty ()) {
    Database_Logs::log (gettext("No user found"), Filter_Roles::translator ());
    return;
  }
  string user = users [0];
  request.session_logic ()->setUsername (user);
  
  string address = Database_Config_General::getServerAddress ();
  int port = Database_Config_General::getServerPort ();
  string url = client_logic_url (address, port, "setting");
  
  // Go through all settings flagged as having been updated on this client.
  vector <int> ids = request.database_config_user()->getUpdatedSettings ();
  if (!ids.empty ()) {
    Database_Logs::log (gettext("Sending settings"), Filter_Roles::translator ());
  }
  
  for (auto id : ids) {

    map <string, string> post = {
      make_pair ("u", bin2hex (user)),
      make_pair ("p", request.database_users ()->getmd5 (user)),
      make_pair ("l", convert_to_string (request.database_users ()->getUserLevel (user))),
      make_pair ("i", convert_to_string (id))
    };

    /* Todo
    switch (id) {
      case Sync_Logic::WORKBENCH_SETTING:
      {
        $urls = request.database_config_user()->getWorkbenchURLs ();
        $urls = unserialize ($urls);
        $widths = request.database_config_user()->getWorkbenchWidths ();
        $widths = unserialize ($widths);
        $heights = request.database_config_user()->getWorkbenchHeights ();
        $heights = unserialize ($heights);
        $setting = array ();
        $setting ['urls'] = $urls;
        $setting ['widths'] = $widths;
        $setting ['heights'] = $heights;
        break;
      }
    }
    */
    
  }

  /* Todo
   

    // POST the setting to the server.
    $response = Sync_Logic::post ($post, $url);
    
    // Handle server's response.
    if ($response === false) {
      Database_Logs::log ("Failure sending setting to server", Filter_Roles::translator ());
    } else {
      request.database_config_user()->removeUpdatedSetting ($id);
    }
   */
  
  
  // All changed settings have now been sent to the server.
  // The client will now synchronize its settings with the server's settings.
  tasks_logic_queue (SYNCSETTINGS);
}


void sendreceive_sync_settings () // Todo write.
{
  /*
  require_once ("../bootstrap/bootstrap"); // Todo port.
  
  
  // Security: The script runs from the cli SAPI only.
  Filter_Cli::assert ();
  
  
  $database_logs = Database_Logs::getInstance ();
  $database_config_bible = Database_Config_Bible::getInstance ();
  
  $database_config_user = Database_Config_User::getInstance ();
  $database_users = Database_Users::getInstance ();
  $session_logic = Session_Logic::getInstance ();
  
  
  $address = Database_Config_General::getServerAddress ();
  $url = "$address/sync/settings";
  
  
  // The client is supposed to have one user.
  // Get this username to send it to the server.
  // The server uses it to access the proper settings for the user.
  $users = request.database_users ()->getUsers ();
  $user = $users [0];
  request.session_logic ()->setUsername ($user);
  
  
  // The script requests the checksum of all relevant settings from the server.
  // It compares this with the local checksum.
  // If it matches, that means that the local settings match the settings on the server.
  // The script is then ready.
  $post = array (
                 "a" => "total",
                 "u" => bin2hex ($user)
                 );
  $response = Sync_Logic::post ($post, $url);
  @$response = unserialize ($response);
  if ($response === false) {
    Database_Logs::log ("Failure synchronizing Settings while requesting totals", Filter_Roles::translator ());
    die;
  }
  $checksum = Sync_Logic::settings_checksum ();
  if ($response == $checksum) {
    Database_Logs::log ("The Settings are up to date", Filter_Roles::translator ());
    die;
  }
  
  
  // At this stage the checksum of all relevant settings on the client differs from the same on the server.
  // Requests all settings from the server.
  $post = array (
                 "a" => Sync_Logic::WORKBENCH_SETTING,
                 "u" => bin2hex ($user)
                 );
  $response = Sync_Logic::post ($post, $url);
  @$response = unserialize ($response);
  if ($response === false) {
    Database_Logs::log ("Failure receiving Settings", Filter_Roles::translator ());
    die;
  }
  $urls = $response ['urls'];
  $widths = $response ['widths'];
  $heights = $response ['heights'];
  $urls = serialize ($urls);
  $widths = serialize ($widths);
  $heights = serialize ($heights);
  request.database_config_user()->setWorkbenchURLs ($urls);
  request.database_config_user()->setWorkbenchWidths ($widths);
  request.database_config_user()->setWorkbenchHeights ($heights);
  
  
*/
  // Done.
  Database_Logs::log ("The Settings have been updated", Filter_Roles::translator ());
}

