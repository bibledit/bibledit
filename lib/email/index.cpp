/*
Copyright (©) 2003-2014 Teus Benschop.

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


#include <email/index.h>
#include <assets/view.h>
#include <assets/page.h>
#include <filter/roles.h>
#include <filter/url.h>
#include <config/logic.h>
#include <database/config/general.h>
#include <webserver/request.h>
#include <email/send.h>


string email_index_url ()
{
  return "email/index";
}


bool email_index_acl (void * webserver_request)
{
  return Filter_Roles::access_control (webserver_request, Filter_Roles::admin ());
}


string email_index (void * webserver_request)
{
  Webserver_Request * request = (Webserver_Request *) webserver_request;
  
  string page;

  page = Assets_Page::header (gettext("Mail"), webserver_request, "");

  Assets_View view = Assets_View ();

  // Site name and email.
  if (request->post ["email"] != "") {
    bool form_is_valid = true;
    string sitename = request->post ["sitename"];
    string sitemail = request->post ["sitemail"];
    if (sitemail.length () > 0) {
      if (!filter_url_email_is_valid (sitemail)) {
        form_is_valid = false;
        view.set_variable ("site_name_error", gettext("The email address does not appear to be valid"));
      }
    }
    if (form_is_valid) {
      Database_Config_General::setSiteMailName (sitename);
      Database_Config_General::setSiteMailAddress (sitemail);
      view.set_variable ("site_name_success", gettext("The name and email address were saved"));
    }
  }
  view.set_variable ("sitename", Database_Config_General::getSiteMailName ());
  view.set_variable ("sitemail", Database_Config_General::getSiteMailAddress ());

  // Email retrieval.
  if (request->post ["retrieve"] != "") {
    string storagehost = request->post ["storagehost"];
    string storageusername = request->post ["storageusername"];
    string storagepassword = request->post ["storagepassword"];
    string storagesecurity = request->post ["storagesecurity"];
    string storageport = request->post ["storageport"];
    string storage_error;
    Database_Config_General::setMailStorageHost (storagehost);
    Database_Config_General::setMailStorageUsername (storageusername);
    Database_Config_General::setMailStoragePassword (storagepassword);
    Database_Config_General::setMailStorageSecurity (storagesecurity);
    Database_Config_General::setMailStoragePort (storageport);
    string storage_success (" ");
    storage_success.append (gettext("The details were saved."));
    /* Todo
    mail = new Mail_Receive ();
    storage_success += " " + gettext("The account was accessed successfully.") + " " + gettext ("Messages on server:") + " " + convert_to_string (mail.count) + ".";
    storage_error += " " + e.getMessage ();
    */
    view.set_variable ("storage_success", storage_success);
    view.set_variable ("storage_error", storage_error);
  }
  view.set_variable ("storagehost", Database_Config_General::getMailStorageHost ());
  view.set_variable ("storageusername", Database_Config_General::getMailStorageUsername ());
  view.set_variable ("storagepassword", Database_Config_General::getMailStoragePassword ());
  if (Database_Config_General::getMailStorageSecurity () == "SSL") view.set_variable ("storagessl", "selected=\"selected\"");
  if (Database_Config_General::getMailStorageSecurity () == "TLS") view.set_variable ("storagetls", "selected=\"selected\"");
  view.set_variable ("storageport", Database_Config_General::getMailStoragePort ());
  
  // Sending email.
  if (request->post ["send"] != "") {
    string sendhost = request->post ["sendhost"];
    string sendauthentication = request->post ["sendauthentication"];
    string sendusername = request->post ["sendusername"];
    string sendpassword = request->post ["sendpassword"];
    string sendsecurity = request->post ["sendsecurity"];
    string sendport  = request->post ["sendport"];
    Database_Config_General::setMailSendHost (sendhost);
    Database_Config_General::setMailSendUsername (sendusername);
    Database_Config_General::setMailSendPassword (sendpassword);
    Database_Config_General::setMailSendPort (sendport);
    string send_success  = gettext("The details were saved.");
    string send_error;
    string send_debug;
    string result = email_send (Database_Config_General::getSiteMailAddress(), Database_Config_General::getSiteMailName(), "Test", "This is to try out whether Bibledit can send email.");
    if (result.empty()) {
      send_success.append (" ");
      send_success.append (gettext("For the purpose of trying whether Bibledit can send email, a test email was sent out to the account above:"));
      send_success.append (" ");
      send_success.append (Database_Config_General::getSiteMailAddress());
    } else {
      send_error = result;
    }
    view.set_variable ("send_success", send_success);;
    view.set_variable ("send_error", send_error);
    view.set_variable ("send_debug", send_debug);
  }
  view.set_variable ("sendhost", Database_Config_General::getMailSendHost ());
  view.set_variable ("sendusername", Database_Config_General::getMailSendUsername ());
  view.set_variable ("sendpassword", Database_Config_General::getMailSendPassword ());
  view.set_variable ("sendport", Database_Config_General::getMailSendPort ());

  page += view.render ("email", "index");

  page += Assets_Page::footer ();

  return page;
}
