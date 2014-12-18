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


#include <dialog/entry.h>
#include <assets/view.h>
#include <assets/page.h>


// Entry dialog constructor
// $url     : The base URL of the page.
// $query   : A map with query variables, e.g. ("search" => "bibledit").
//            If any $query is passed, if Cancel is clicked in this dialog, it should go go back
//            to the original caller page  with the $query added. Same for Ok, it would post
//            the value entered, but also add the $query to the url.
// $question: The question to be asked.
// $value   : The initial value to be put into the entry.
// $submit  : Name of POST request to submit the information.
// $help    : Help information explaining to the user what's going on.
Dialog_Entry::Dialog_Entry (const string& url, map <string, string> query, const string& question, const string& value, const string& submit, const string& help)
{
  Assets_View * view = new Assets_View ();
  string base_url (url);
  /* Todo
  $base_url = $_SERVER['PHP_SELF'];
  if (is_array ($query)) {
    $base_url .= "?" . http_build_query ($query);
  }
  */
  view->set_variable ("base_url", base_url);
  view->set_variable ("question", question);
  view->set_variable ("value", value);
  view->set_variable ("submit", submit);
  view->set_variable ("help", help);
  assets_view = view;
}


Dialog_Entry::~Dialog_Entry ()
{
  Assets_View * view = (Assets_View *) assets_view;
  delete view;
}


string Dialog_Entry::run ()
{
  Assets_View * view = (Assets_View *) assets_view;
  string page = view->render ("dialog", "entry");
  page += Assets_Page::footer ();
  return page;
}

