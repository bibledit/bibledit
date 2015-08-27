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


#include <lexicon/definition.h>
#include <filter/roles.h>
#include <filter/string.h>
#include <filter/url.h>
#include <webserver/request.h>
#include <lexicon/logic.h>
#include <database/kjv.h>
#include <database/morphhb.h>
#include <database/morphgnt.h>


string lexicon_definition_url ()
{
  return "lexicon/definition";
}


bool lexicon_definition_acl (void * webserver_request)
{
  return Filter_Roles::access_control (webserver_request, Filter_Roles::consultant ());
}


string lexicon_definition (void * webserver_request)
{
  // Retrieve the id: It may contain a Strong's number or a lemma.
  Webserver_Request * request = (Webserver_Request *) webserver_request;
  string id = request->query["id"];
  
  vector <string> renderings;
  
  if (!id.empty ()) {
    
    string letter = id.substr (0, 1);
    if (letter == HEBREW_ETCBE4_PREFIX) {
      
      // ETCBC4 database.
      renderings.push_back (lexicon_logic_render_etcb4_morphology (id));
      
    } else if (letter == KJV_LEXICON_PREFIX) {
      
      // King James Bible with Strong's numbers.
      id = id.substr (1);
      Database_Kjv database_kjv;
      string strong = database_kjv.strong (convert_to_int (id));
      string rendering = lexicon_logic_render_definition (strong);
      if (!rendering.empty ()) renderings.push_back (rendering);
      
    } else if (letter == MORPHHB_PREFIX) {
      
      // Open Scriptures Hebrew with Strong's numbers.
      id = id.substr (1);
      Database_MorphHb database_morphhb;
      string parsing = database_morphhb.parsing (convert_to_int (id));
      vector <string> strongs = lexicon_logic_convert_morphhb_parsing_to_strong (parsing);
      for (auto strong : strongs) {
        string rendering = lexicon_logic_render_definition (strong);
        if (!rendering.empty ()) renderings.push_back (rendering);
      }

    } else if (letter == SBLGNT_PREFIX) {
      
      // SBL Greek New Testament plus morphology.
      Database_MorphGnt database_morphgnt;
      id = id.substr (1);
      int rowid = convert_to_int (id);
      // The part of speech.
      string pos = database_morphgnt.pos (rowid);
      string rendering = lexicon_logic_render_morphgnt_part_of_speech (pos);
      rendering.append (" ");
      // The parsing.
      string parsing = database_morphgnt.parsing (rowid);
      rendering.append (lexicon_logic_render_morphgnt_parsing_code (parsing));
      renderings.push_back (rendering);
      
    } else if (letter == "H") {
      
      // Strong's Hebrew.
      string rendering = lexicon_logic_render_definition (id);
      if (!rendering.empty ()) renderings.push_back (rendering);
      
    } else if (letter == "G") {
      
      // Strong's Greek.
      string rendering = lexicon_logic_render_definition (id);
      if (!rendering.empty ()) renderings.push_back (rendering);
      
    } else {

      renderings.push_back (id);
      
    }
    
  }
  
  return filter_string_implode (renderings, "<br>");
}
