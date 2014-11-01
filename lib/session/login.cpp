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


#include <assets/view.h>
#include <assets/page.h>
#include <assets/header.h>
#include <session/login.h>
#include <locale/translate.h>
#include <webserver/request.h>
#include <filter/url.h>
#include <filter/roles.h>
#include <database/logs.h>
#include <database/config/general.h>


string session_login (void * webserver_request)
{
  /*
  This script can have several functions:
  
  1. Display login form.
  The user is not logged in. 
  The standard form is displayed.
  
  2. Incorrect credentials entered.
  The user did not enter correct credentials.
  The login form is displayed, with an error description.
  
  3. Forward to URL.
  The script is called with a query for where to forward the user to.
  */

  Webserver_Request * request = (Webserver_Request *) webserver_request;

  Assets_View view = Assets_View (0);

  // Form submission handler.
  if (request->post["submit"] != "") {
    bool form_is_valid = true;
    string user = request->post["user"];
    string pass = request->post["pass"];
    if (user.length () < 2) {
      form_is_valid = false;
      view.set_variable ("username_email_invalid", gettext ("Username should be at least two characters long"));
    }
    if (pass.length() < 4) {
      form_is_valid = false;
      view.set_variable ("password_invalid", gettext ("Password should be at least four characters long"));
    }
    if (form_is_valid) {
      if (request->session_logic()->attemptLogin (user, pass)) {
        // Log the login.
        Database_Logs::log (request->session_logic()->currentUser () + " logged in");
        // Store web site's base URL.
        string siteUrl = filter_url_page_url (request);
        Database_Config_General::setSiteURL (siteUrl);
      } else {
        view.set_variable ("error_message", gettext ("Username or email address or password are not correct"));
        request->session_logic()->logout();
        // Log the login failure for the Administrator(s), so that others cannot reverse engineer a user's password based on the failure information.
        Database_Logs::log ("Failed login attempt for user $user with password $pass", Filter_Roles::admin ());
      }
    }
  }

  string page;

  string query = request->query;
  
  if (request->session_logic ()->loggedIn ()) {
    if (query.length () >= 8) query = query.substr (8);
    if (query != "") {
      // After login, the user is forwarded to the originally requested URL, if any.
      filter_url_redirect (query, request);
      return page;
    }
    page += session_login_display_header (webserver_request);
    view.set_variable ("welcome", gettext ("Welcome"));
    view.set_variable ("loggedin", gettext ("You have logged in."));
    page += view.render ("session", "loggedin");
  } else {
    page += session_login_display_header (webserver_request);
    view.set_variable ("query", query);
    view.enable_zone ("logging_in");
    view.set_variable ("login", gettext ("Login"));
    view.set_variable ("username_email", gettext ("Username or email address"));
    view.set_variable ("password", gettext ("Password"));
    view.set_variable ("forgot_password", gettext ("Forgot password?"));
    view.set_variable ("not_registered", gettext ("Not registered?"));
    view.set_variable ("signup", gettext ("Sign up!"));
    page += view.render ("session", "login");
  }

  page += Assets_Page::footer ();

  return page;
}


string session_login_display_header (void * webserver_request)
{
  /*
  Postpone displaying the header for two reasons:
  1. The logged-in state is likely to change during this script.
     Therefore the header should wait till the new state is known.
  2. The script may forward the user to another page.
     Therefore no output should be sent so the forward headers work.
  */
  Assets_Header header = Assets_Header (gettext ("Login"), webserver_request);
  header.setBodyOnload ("document.form.user.focus();");
  return header.run ();
}
