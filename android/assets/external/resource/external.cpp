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


#include <resource/external.h>
#include <filter/string.h>
#include <filter/url.h>
#include <webserver/request.h>
#include "json/json.h"
#include "assets/view.h"


// Local forward declarations:


unsigned int resource_external_count ();
int gbs_digitaal_json_callback (void *userdata, int type, const char *data, uint32_t length);
string gbs_digitaal_processor (string url, int chapter, int verse);
string gbs_digitaal_plus_processor (string url, int verse);
string studylight_processor (string directory, int book, int chapter, int verse);
string bibleserver_processor (string directory, int book, int chapter, int verse);
string resource_external_get_statenbijbel_gbs (int book, int chapter, int verse);
string resource_external_get_statenbijbel_plus_gbs (int book, int chapter, int verse);
string resource_external_get_king_james_version_gbs (int book, int chapter, int verse);
string resource_external_get_king_james_version_plus_gbs (int book, int chapter, int verse);
string resource_external_get_biblehub_interlinear (int book, int chapter, int verse);
string resource_external_get_biblehub_scrivener (int book, int chapter, int verse);
string resource_external_get_biblehub_westminster (int book, int chapter, int verse);
string resource_external_get_net_bible (int book, int chapter, int verse);
string resource_external_get_calvin_commentaries (int book, int chapter, int verse);
string resource_external_get_jamieson_fausset_brown (int book, int chapter, int verse);
string resource_external_get_john_gill_exposition_whole_bible (int book, int chapter, int verse);
string resource_external_get_matthew_poole_annotations (int book, int chapter, int verse);
string resource_external_get_blue_letter_bible (int book, int chapter, int verse);
string resource_external_get_elberfelder_bibel (int book, int chapter, int verse);
string resource_external_convert_book_biblehub (int book);
string resource_external_convert_book_netbible (int book);
string resource_external_convert_book_blueletterbible (int book);
string resource_external_convert_book_bibleserver (int book);


typedef struct
{
  const char *name;
  const char *versification;
  const char *mapping;
  string (* func) (int, int, int);
} resource_record;


// Implementation:


resource_record resource_table [] =
{
  { "Statenbijbel GBS", "Dutch Traditional", "Dutch Traditional", & resource_external_get_statenbijbel_gbs },
  { "Statenbijbel Plus GBS", "Dutch Traditional", "Dutch Traditional", & resource_external_get_statenbijbel_plus_gbs },
  { "King James Version GBS", "English", "English", & resource_external_get_king_james_version_gbs },
  { "King James Version Plus GBS", "English", "English", & resource_external_get_king_james_version_plus_gbs },
  { "Biblehub Interlinear", "English", "English", & resource_external_get_biblehub_interlinear },
  { "Scrivener Greek", "English", "English", & resource_external_get_biblehub_scrivener },
  { "Westminster Hebrew", "English", "English", & resource_external_get_biblehub_westminster },
  { "NET Bible", "English", "English", & resource_external_get_net_bible },
  { "Calvin Commentaries", "English", "English", & resource_external_get_calvin_commentaries },
  { "Jamieson Fausset Brown", "English", "English", & resource_external_get_jamieson_fausset_brown },
  { "John Gill Exposition Whole Bible", "English", "English", & resource_external_get_john_gill_exposition_whole_bible },
  { "Matthew Poole Annotations", "English", "English", & resource_external_get_matthew_poole_annotations },
  { "Blue Letter Bible", "English", "English", & resource_external_get_blue_letter_bible },
  { "Elberfelder Bibel", "English", "English", & resource_external_get_elberfelder_bibel },
};


int gbs_digitaal_json_callback (void *userdata, int type, const char *data, uint32_t length)
{
  if (length) {};
  vector <string> * history = (vector <string> *) userdata;
  switch (type) {
    case JSON_OBJECT_BEGIN:
    case JSON_ARRAY_BEGIN:
    case JSON_OBJECT_END:
    case JSON_ARRAY_END:
      break;
    case JSON_KEY:
    case JSON_STRING:
    case JSON_INT:
    case JSON_FLOAT:
    {
      history->push_back (data);
      break;
    }
    case JSON_NULL:
    {
      history->push_back ("");
      break;
    }
    case JSON_TRUE:
    case JSON_FALSE:
      break;
  }
  return 0;
}


// This function displays the canonical text from gbsdigitaal.nl.
string gbs_digitaal_processor (string url, int verse)
{
  string text;

  string json = filter_url_http_get (url, text);

  vector <string> history;
  
  json_parser parser;
  if (json_parser_init (&parser, NULL, gbs_digitaal_json_callback, &history)) {
    text.append ("Could not initialize");
  }
  if (json_parser_string (&parser, json.c_str(), json.length(), NULL)) {
    text.append ("Could not parse the data");
  }
  json_parser_free (&parser);

  // The history sequence to get a verse is this:
  // uid -> 0 -> number -> <verse number> -> text -> "verse text"
  for (unsigned int i = 0; i < history.size (); i++) {
    if (history[i] == "text") {
      if (i >= 5) {
        if (history [i - 4] == "uid") {
          if (history [i - 3] == "0") {
            if (history [i - 2] == "number") {
              if (history [i - 1] == convert_to_string (verse)) {
                if (history.size () > i + 1) {
                  text = history [i + 1];
                }
              }
            }
          }
        }
      }
    }
  }

  // Take out breaks.
  text = filter_string_str_replace ("<br />", "", text);
  // Remove the note callers.
  filter_string_replace_between (text, "<sup", "</sup>", "");
  // Remove the pilcrow sign / paragraph sign if it's there.
  text = filter_string_str_replace ("¶", "", text);
  // Add new line.
  text += "\n";
  
  // Done.
  return text;
}


// This funcion processes and displays Bible data from gbsdigitaal.nl
// plus extra content like headers, introductions, cross references, and notes.
// It is supposed to be called by a more specific script.
string gbs_digitaal_plus_processor (string url, int chapter, int verse)
{
  string text;
  
  string json = filter_url_http_get (url, text);

  vector <string> history;
  
  json_parser parser;
  if (json_parser_init (&parser, NULL, gbs_digitaal_json_callback, &history)) {
    text.append ("Could not initialize");
  }
  if (json_parser_string (&parser, json.c_str(), json.length(), NULL)) {
    text.append ("Could not parse the data");
  }
  json_parser_free (&parser);
  
  string historyblock;
  bool collect_notes = false;
  int text_block_counter = 0;
  for (unsigned int i = 0; i < history.size (); i++) {
    // Add possible book title.
    if (chapter == 0) {
      if (history[i] == "title") {
        if (history.size () > i + 1) {
          text.append (" " + history [i + 1]);
        }
      }
    }
    // Add possible book summary.
    if (chapter == 0) {
      if (history[i] == "summary") {
        if (history.size () > i + 1) {
          text.append (" " + history [i + 1]);
        }
      }
    }
    // Add possible chapter title.
    if (chapter == 0) {
      if (history[i] == "headerTitle") {
        if (history.size () > i + 1) {
          text.append (" " + history [i + 1]);
        }
      }
    }
    // Add possible book summary.
    if (chapter == 0) {
      if (i > 2) {
        if (history[i] == "content") {
          if (history[i-1] == "lines") {
            if (history.size () > i + 1) {
              text.append (" " + history [i + 1]);
            }
          }
        }
      }
    }
    // Add the chapter summary located at verse 0.
    if (verse == 0) {
      if (i > 2) {
        if (history[i] == "originalText") {
          if (history[i-1] == "summary") {
            if (history.size () > i + 1) {
              text.append (" " + history [i + 1]);
            }
          }
        }
      }
    }
    // History sequence to get the header above a verse:
    // text -> <header text> -> isPerikoop -> isHeader -> uid -> 0 -> number -> <verse number>
    if (history[i] == "text") {
      if (history.size () > i + 7) {
        if (history[i+2] == "isPerikoop") {
          if (history[i+7] == convert_to_string (verse)) {
            text.append ("<p><i>" + history [i+1] + "</i></p>");
          }
        }
      }
    }
    // The history sequence to get the verse text is this:
    // uid -> 0 -> number -> <verse number> -> text -> "verse text"
    if (history[i] == "text") {
      if (i >= 5) {
        if (history [i - 4] == "uid") {
          if (history [i - 3] == "0") {
            if (history [i - 2] == "number") {
              if (history [i - 1] == convert_to_string (verse)) {
                if (history.size () > i + 1) {
                  text.append (" " + history [i + 1]);
                }
                // Enable notes collection.
                collect_notes = true;
              } else {
                // Disable notes collection
                collect_notes = false;
              }
            }
          }
        }
      }
    }
    // Add notes.
    if (collect_notes && i && history [i-1] == "originalText") {
      text.append ("<p>" + history [i] + "</p>");
    }
    // Add text of Bible references.
    if (collect_notes && i && history [i-1] == "text") {
      if (text_block_counter) text.append ("<p>" + history [i] + "</p>");
      text_block_counter++;
    }
  }

  text = filter_string_str_replace ("\n", " ", text);
  text = filter_string_str_replace ("\r", " ", text);
  text = filter_string_str_replace ("\t", "", text);
  text = filter_string_str_replace ("  ", " ", text);
  text = filter_string_str_replace ("<br />", "", text);
  text = filter_string_trim (text);

  // Add new line.
  text += "\n";

  // Done.
  return text;
}


// This function commentaries from www.studylight.org.
string studylight_processor (string directory, int book, int chapter, int verse)
{
  // On StudyLight.org, Genesis equals book 0, Exodus book 1, and so on.
  book--;
  
  string url = "http://www.studylight.org/com/" + directory + "/view.cgi?bk=" + convert_to_string (book) + "&ch=" + convert_to_string (chapter);
  
  // Get the html from the server, and tidy it up.
  string error;
  string html = filter_url_http_get (url, error);
  string tidy = html_tidy (html);
  vector <string> tidied = filter_string_explode (tidy, '\n');
 
  vector <string> relevant_lines;
  bool relevant_flag = false;
  
  for (auto & line : tidied) {
    
    if (relevant_flag) relevant_lines.push_back (line);
    
    size_t pos = line.find ("</div>");
    if (pos != string::npos) relevant_flag = false;
    
    pos = line.find ("name=\"" + convert_to_string (verse) + "\"");
    if (pos != string::npos) relevant_flag = true;
  }
  
  html = filter_string_implode (relevant_lines, "\n");
  html += "<p><a href=\"" + url + "\" " + Assets_View::target_conditional_blank () + ">" + url + "</a></p>\n";
  
  return html;
}


// This filters Bibles from www.bibleserver.com.
string bibleserver_processor (string directory, int book, int chapter, int verse)
{
  string bookname = resource_external_convert_book_bibleserver (book);
  
  string url = "http://www.bibleserver.com/text/" + directory + "/" + bookname + convert_to_string (chapter);
  
  string error;
  string text = filter_url_http_get (url, error);
  string tidy = html_tidy (text);
  vector <string> tidied = filter_string_explode (tidy, '\n');

  text.clear ();
  bool relevant_line = false;
  for (auto & line : tidied) {
    size_t pos = line.find ("noscript");
    if (pos != string::npos) relevant_line = false;
    if (relevant_line) {
      if (!text.empty ()) text.append (" ");
      text.append (line);
    }
    pos = line.find ("no=\"" + convert_to_string (verse) + "," + convert_to_string (verse) + "\"");
    if (pos != string::npos) relevant_line = true;
  }
  filter_string_replace_between (text, "<", ">", "");
  text = filter_string_trim (text);
  
  text += "<p><a href=\"" + url + "\" " + Assets_View::target_conditional_blank () + ">" + url + "</a></p>\n";
  
  return text;
}


// This script fetches the Statenbijbel from the Dutch GBS.
string resource_external_get_statenbijbel_gbs (int book, int chapter, int verse)
{
  // Books OT: http://gbsdigitaal.nl/Data/Statenvertaling/1.json
  // Books NT: http://gbsdigitaal.nl/Data/Statenvertaling/2.json
  // Genesis intro http://gbsdigitaal.nl/Data/Statenvertaling/1/1/1.json
  // Genesis 1 http://gbsdigitaal.nl/Data/Statenvertaling/1/1/2.json
  // Matthew 1 http://gbsdigitaal.nl/Data/Statenvertaling/2/40/2.json
  
  // Old Testament = 1 and New Testament = 2.
  int testament = book >= 40 ? 2 : 1;
  
  // Chapter 1 of the text is chapter 2 in the URL.
  string urlchapter = convert_to_string (chapter + 1);
  
  string url = "http://gbsdigitaal.nl/Data/Statenvertaling/" + convert_to_string (testament) + "/" + convert_to_string (book) + "/" + urlchapter + ".json";
  
  return gbs_digitaal_processor (url, verse);
}


// This displays the Statenbijbel from the Dutch GBS.
// It also includes headers, introductions, and notes.
string resource_external_get_statenbijbel_plus_gbs (int book, int chapter, int verse)
{
  // Books OT: http://gbsdigitaal.nl/Data/Statenvertaling/1.json
  // Books NT: http://gbsdigitaal.nl/Data/Statenvertaling/2.json
  // Genesis intro http://gbsdigitaal.nl/Data/Statenvertaling/1/1/1.json
  // Genesis 1 http://gbsdigitaal.nl/Data/Statenvertaling/1/1/2.json
  // Matthew 1 http://gbsdigitaal.nl/Data/Statenvertaling/2/40/2.json
  
  // Old (1) or New (2) Testament.
  int testament = book >= 40 ? 2 : 1;
  
  // Introduction is chapter 1 in the URL.
  // Chapter 1 of the text is chapter 2 in the URL.
  string urlchapter = convert_to_string (chapter + 1);
  
  string url = "http://gbsdigitaal.nl/Data/Statenvertaling/" + convert_to_string (testament) + "/" + convert_to_string (book) + "/" + urlchapter + ".json";
  
  return gbs_digitaal_plus_processor (url, chapter, verse);
}


// This script displays the King James Bible published by the Dutch GBS.
string resource_external_get_king_james_version_gbs (int book, int chapter, int verse)
{
  // Books OT: http://gbsdigitaal.nl/Data/AuthorizedVersion/1.json
  // Books NT: http://gbsdigitaal.nl/Data/AuthorizedVersion/2.json
  // Genesis intro http://gbsdigitaal.nl/Data/AuthorizedVersion/1/1/1.json
  // Genesis 1 http://gbsdigitaal.nl/Data/AuthorizedVersion/1/1/2.json
  // Matthew 1 http://gbsdigitaal.nl/Data/AuthorizedVersion/2/40/2.json
  
  // Old (1) or New (2) Testament.
  int testament = book >= 40 ? 2 : 1;
  
  // Chapter 1 of the text is chapter 2 in the URL.
  string urlchapter = convert_to_string (chapter + 1);
  
  string url = "http://gbsdigitaal.nl/Data/AuthorizedVersion/" + convert_to_string (testament) + "/" + convert_to_string (book) + "/" + urlchapter + ".json";
  
  return gbs_digitaal_processor (url, verse);
}


// This script displays the Statenbijbel from the Dutch GBS.
// It also includes headers, introductions, and notes.
string resource_external_get_king_james_version_plus_gbs (int book, int chapter, int verse)
{
  // Books OT: http://gbsdigitaal.nl/Data/Statenvertaling/1.json
  // Books NT: http://gbsdigitaal.nl/Data/Statenvertaling/2.json
  // Genesis intro http://gbsdigitaal.nl/Data/Statenvertaling/1/1/1.json
  // Genesis 1 http://gbsdigitaal.nl/Data/Statenvertaling/1/1/2.json
  // Matthew 1 http://gbsdigitaal.nl/Data/Statenvertaling/2/40/2.json
  
  // Old (1) or New (2) Testament.
  int testament = book >= 40 ? 2 : 1;
  
  // Introduction is chapter 1 in the URL.
  // Chapter 1 of the text is chapter 2 in the URL.
  string urlchapter = convert_to_string (chapter + 1);
  
  string url = "http://gbsdigitaal.nl/Data/AuthorizedVersion/" + convert_to_string (testament) + "/" + convert_to_string (book) + "/" + urlchapter + ".json";
  
  return gbs_digitaal_plus_processor (url, chapter, verse);
}


// This displays the interlinear Bible from biblehub.com.
string resource_external_get_biblehub_interlinear (int book, int chapter, int verse)
{
  // Sample URL:
  // http://biblehub.com/interlinear/genesis/1-1.htm
 
  string bookname = resource_external_convert_book_biblehub (book);
  
  string url = "http://biblehub.com/interlinear/" + bookname + "/" + convert_to_string (chapter) + "-" + convert_to_string (verse) + ".htm";
  
  // Get the html from the server, and tidy it up.
  string error;
  string html = filter_url_http_get (url, error);
  string tidy = html_tidy (html);
  vector <string> tidied = filter_string_explode (tidy, '\n');
  
  vector <string> filtered_lines;
  
  int relevant_line = 0;
  for (auto & line : tidied) {
    if (line.find ("<div") != string::npos) {
      relevant_line = 0;
    }
    if (relevant_line) {
      relevant_line++;
    }
    if (relevant_line > 3) {
      if (line.find ("iframe") == string::npos) {
        filtered_lines.push_back (line);
      }
    }
    if (line.find ("\"vheading\"") != string::npos) {
      relevant_line++;
    }
  }
  
  html = filter_string_implode (filtered_lines, "\n");
  
  html = filter_string_str_replace ("title=", Assets_View::target_conditional_blank () + " title=", html);
  
  html = filter_string_str_replace ("/abbrev.htm", "http://biblehub.com/abbrev.htm", html);
  html = filter_string_str_replace ("/hebrew/", "http://biblehub.com/hebrew/", html);
  html = filter_string_str_replace ("/hebrewparse.htm", "http://biblehub.com/hebrewparse.htm", html);
  html = filter_string_str_replace ("/greek/", "http://biblehub.com/greek/", html);
  html = filter_string_str_replace ("/grammar/", "http://biblehub.com/grammar/", html);
  //html = filter_string_str_replace ("height=\"165\"", "", html);
  html = filter_string_str_replace ("height=\"160\"", "", html);
  html = filter_string_str_replace ("height=\"145\"", "", html);
  html = filter_string_str_replace ("&nbsp;&nbsp;", "&nbsp;", html);
  
  // Stylesheet for using web fonts,
  // because installing fonts on some tablets is very hard.
  string stylesheet =
  "<style>\n"
  ".tablefloat {\n"
  "  float: left;\n"
  "}\n"
  "@font-face {\n"
  "  font-family: \"Cardo\";\n"
  "src: url(/fonts/Cardo-Regular.ttf);\n"
  "}\n"
  ".greek {\n"
  "  font-family: Cardo;\n"
  "  font-size: large;\n"
  "}\n"
  ".tablefloatheb\n"
  "{\n"
  "  float : right;\n"
  "}\n"
  "@font-face {\n"
  "  font-family: \"Ezra SIL\";\n"
  "src: url(/fonts/SILEOT.ttf);\n"
  "}\n"
  ".hebrew {\n"
  "  font-family: \"Ezra SIL\";\n"
  "  font-size: x-large;\n"
  "}\n"
  "span[class*='ref'] {\n"
  "display: none;\n"
  "}\n"
  "</style>\n";
  string output = stylesheet;
  // The following line prevents the Hebrew and Greek from floating around
  // the name of the Resource, which would disturb the order of the words in Hebrew.
  output += "<p></p>\n";
  output += html;
  
  return output;
}


// Filters the Scrivener Greek text from biblehub.com.
string resource_external_get_biblehub_scrivener (int book, int chapter, int verse)
{
  string bookname = resource_external_convert_book_biblehub (book);
  
  string url = "http://biblehub.com/text/" + bookname + "/" + convert_to_string (chapter) + "-" + convert_to_string (verse) + ".htm";
  
  // Get the html from the server, and tidy it up.
  string error;
  string html = filter_url_http_get (url, error);
  string tidy = html_tidy (html);
  vector <string> tidied = filter_string_explode (tidy, '\n');

  html.clear ();
  int hits = 0;
  for (auto & line : tidied) {
    /* This is the text block we are looking at:
    <p>
    <span class="versiontext">
    <a href="/tr94/matthew/1.htm">Scrivener's Textus Receptus 1894
    </a>
    <br>
    </span>
    <span class="greek">Βίβλος γενέσεως Ἰησοῦ Χριστοῦ, υἱοῦ Δαβὶδ, υἱοῦ Ἀβραάμ.
    </span>
    */
    if (line.find ("Scrivener") != string::npos) hits = 1;
    if (hits == 1) if (line.find ("greek") != string::npos) hits = 2;
    if ((hits == 2) || (hits == 3)) {
      html.append (line);
      hits++;
    }
  }
  
  if (html.empty ()) return html;
  
  string stylesheet =
  "<style>\n"
  "@font-face {\n"
  "  font-family: Cardo;\n"
  "src: url(/fonts/Cardo-Regular.ttf);\n"
  "}\n"
  ".greek {\n"
  "  font-family: Cardo;\n"
  "  font-size: large;\n"
  "}\n"
  "</style>\n";
  
  return stylesheet + "\n" + html;
}


// This displays the Westminster Leningrad Codex from biblehub.com.
string resource_external_get_biblehub_westminster (int book, int chapter, int verse)
{
  // No New Testament in the Westminster Leningrad Codex.
  if (book >= 40) {
    return "";
  }
  
  string bookname = resource_external_convert_book_biblehub (book);
  
  // Sample URL:
  // http://biblehub.com/text/genesis/1-1.htm
  string url = "http://biblehub.com/text/" + bookname + "/" + convert_to_string (chapter) + "-" + convert_to_string (verse) + ".htm";
  
  // Get the html from the server, and tidy it up.
  string error;
  string html = filter_url_http_get (url, error);
  string tidy = html_tidy (html);
  vector <string> tidied = filter_string_explode (tidy, '\n');
  
  html.clear ();
  int hits = 0;
  for (auto & line : tidied) {
    /* This is the text block we are looking at:
     <div align="right">
     <span class="versiontext">
     <a href="/wlc/genesis/1.htm">בראשית 1:1 Hebrew OT: Westminster Leningrad Codex
     </a>
     <br>
     </span>
     <span class="heb">בְּרֵאשִׁ֖ית בָּרָ֣א אֱלֹהִ֑ים אֵ֥ת הַשָּׁמַ֖יִם וְאֵ֥ת הָאָֽרֶץ׃
     </span>
     <p>
    */
    if (line.find ("Westminster") != string::npos) hits = 1;
    if (hits == 1) if (line.find ("class=\"heb\"") != string::npos) hits = 2;
    if (hits == 2) {
      html.append (line);
      if (line.find ("</span>") != string::npos) hits = 0;
    }
  }
  
  if (html.empty ()) return html;
  
  // Stylesheet for using web fonts,
  // because installing fonts on some tablets is very hard.
  string stylesheet =
  "<style>\n"
  "@font-face {\n"
  "  font-family: \"Ezra SIL\";\n"
  "src: url(/fonts/SILEOT.ttf);\n"
  "}\n"
  ".heb {\n"
  "  font-family: \"Ezra SIL\";\n"
  "  font-size: x-large;\n"
  "}\n"
  "</style>\n";

  string output = stylesheet;
  
  // The following line prevents the Hebrew from floating around the name of the Resource,
  // which would disturb the order of the words.
  output += "<p></p>\n";
  
  output += html;

  return output;
}


// This displays the text and the notes of the NET Bible.
string resource_external_get_net_bible (int book, int chapter, int verse)
{
  string bookname = resource_external_convert_book_netbible (book);
  
  string url = bookname + " " + convert_to_string (chapter) + ":" + convert_to_string (verse);
  url = filter_url_urlencode (url);
  url.insert (0, "https://net.bible.org/resource/netTexts/");
  
  string error;
  string text = filter_url_http_get (url, error);
  
  string output = text;
  
  output += "\n";
  
  url = bookname + " " + convert_to_string (chapter) + ":" + convert_to_string (verse);
  url = filter_url_urlencode (url);
  url.insert (0, "https://net.bible.org/resource/netNotes/");
  
  string notes = filter_url_http_get (url, error);
  // If notes fail with an error, don't include the note text.
  if (!error.empty ()) notes.clear ();
  
  // The "bibleref" class experiences interference from other resources,
  // so that the reference would become invisible.
  // Remove this class, and the references will remain visible.
  notes = filter_string_str_replace ("class=\"bibleref\"", "", notes);
  
  output += notes;
  
  output += "\n";
  
  return output;
}


// Calvin Commentaries
string resource_external_get_calvin_commentaries (int book, int chapter, int verse)
{
  // The directory on studylight.org
  string directory = "cal";
  return studylight_processor (directory, book, chapter, verse);
}


// Jamieson Fausset Brown
string resource_external_get_jamieson_fausset_brown (int book, int chapter, int verse)
{
  // The directory on studylight.org
  string directory = "jfb";
  return studylight_processor (directory, book, chapter, verse);
}


// John Gill Exposition Whole Bible
string resource_external_get_john_gill_exposition_whole_bible (int book, int chapter, int verse)
{
  // The directory on studylight.org
  string directory = "geb";
  return studylight_processor (directory, book, chapter, verse);
}


// Matthew Poole Annotations
string resource_external_get_matthew_poole_annotations (int book, int chapter, int verse)
{
  // The directory on studylight.org
  string directory = "mpc";
  return studylight_processor (directory, book, chapter, verse);
}


// Blue Letter Bible.
string resource_external_get_blue_letter_bible (int book, int chapter, int verse)
{
  if (verse) {};
  
  string bookname = resource_external_convert_book_blueletterbible (book);
  
  string output;
  
  string url = "http://www.blueletterbible.org/Bible.cfm?b=" + filter_url_urlencode (bookname) + "&c=$" + convert_to_string (chapter) + "&t=KJV&ss=1";
  
  output += "<a href=\"" + url + "\" " + Assets_View::target_conditional_blank () + ">KJV</a>";
  
  output += " | ";
  
  url = "http://www.blueletterbible.org/Bible.cfm?b=" + filter_url_urlencode (bookname) + "&c=" + convert_to_string (chapter) + "&t=WLC";
  
  output += "<a href=\"" + url + "\" " + Assets_View::target_conditional_blank () + ">WLC</a>";
  
  output += " | ";
  
  url = "http://www.blueletterbible.org/Bible.cfm?b=" + filter_url_urlencode (bookname) + "&c=" + convert_to_string (chapter) + "&t=mGNT";
  
  output += "<a href=\"" + url + "\" " + Assets_View::target_conditional_blank () + ">mGNT</a>";

  return output;
}


// This displays the text of the Elberfelder Bibel.
string resource_external_get_elberfelder_bibel (int book, int chapter, int verse)
{
  // The directory on bibleserver.com
  string directory = "ELB";
  return bibleserver_processor (directory, book, chapter, verse);
}


// The number of available external resource scripts.
unsigned int resource_external_count ()
{
  return sizeof (resource_table) / sizeof (*resource_table);
}


// Gets the names of the available external resources.
vector <string> resource_external_names ()
{
  vector <string> names;
  for (unsigned int i = 0; i < resource_external_count (); i++) {
    names.push_back (resource_table [i].name);
  }
  return names;
}


// Returns the versification for the resource.
string resource_external_versification (string name)
{
  string versification = "English";
  for (unsigned int i = 0; i < resource_external_count (); i++) {
    string resource = resource_table [i].name;
    if (name == resource) {
      versification = resource_table [i].versification;
    }
  }
  return versification;
}


// Returns the versification for the resource.
string resource_external_mapping (string name)
{
  string versification = "English";
  for (unsigned int i = 0; i < resource_external_count (); i++) {
    string resource = resource_table [i].name;
    if (name == resource) {
      versification = resource_table [i].mapping;
    }
  }
  return versification;
}


// Fetches the html data for the resource / book / chapter / verse.
string resource_external_get (string name, int book, int chapter, int verse)
{
  string (* func) (int, int, int) = NULL;

  for (unsigned int i = 0; i < resource_external_count (); i++) {
    string resource = resource_table [i].name;
    if (name == resource) {
      func = resource_table [i].func;
    }
  }
  
  if (func == NULL) {
    return "";
  }

  string result = func (book, chapter, verse);
  
  return result;
}


string resource_external_convert_book_biblehub (int book)
{
  // Map Bibledit books to biblehub.com books.
  map <int, string> mapping = {
    make_pair (1, "genesis"),
    make_pair (2, "exodus"),
    make_pair (3, "leviticus"),
    make_pair (4, "numbers"),
    make_pair (5, "deuteronomy"),
    make_pair (6, "joshua"),
    make_pair (7, "judges"),
    make_pair (8, "ruth"),
    make_pair (9, "1_samuel"),
    make_pair (10, "2_samuel"),
    make_pair (11, "1_kings"),
    make_pair (12, "2_kings"),
    make_pair (13, "1_chronicles"),
    make_pair (14, "2_chronicles"),
    make_pair (15, "ezra"),
    make_pair (16, "nehemiah"),
    make_pair (17, "esther"),
    make_pair (18, "job"),
    make_pair (19, "psalms"),
    make_pair (20, "proverbs"),
    make_pair (21, "ecclesiastes"),
    make_pair (22, "songs"),
    make_pair (23, "isaiah"),
    make_pair (24, "jeremiah"),
    make_pair (25, "lamentations"),
    make_pair (26, "ezekiel"),
    make_pair (27, "daniel"),
    make_pair (28, "hosea"),
    make_pair (29, "joel"),
    make_pair (30, "amos"),
    make_pair (31, "obadiah"),
    make_pair (32, "jonah"),
    make_pair (33, "micah"),
    make_pair (34, "nahum"),
    make_pair (35, "habakkuk"),
    make_pair (36, "zephaniah"),
    make_pair (37, "haggai"),
    make_pair (38, "zechariah"),
    make_pair (39, "malachi"),
    make_pair (40, "matthew"),
    make_pair (41, "mark"),
    make_pair (42, "luke"),
    make_pair (43, "john"),
    make_pair (44, "acts"),
    make_pair (45, "romans"),
    make_pair (46, "1_corinthians"),
    make_pair (47, "2_corinthians"),
    make_pair (48, "galatians"),
    make_pair (49, "ephesians"),
    make_pair (50, "philippians"),
    make_pair (51, "colossians"),
    make_pair (52, "1_thessalonians"),
    make_pair (53, "2_thessalonians"),
    make_pair (54, "1_timothy"),
    make_pair (55, "2_timothy"),
    make_pair (56, "titus"),
    make_pair (57, "philemon"),
    make_pair (58, "hebrews"),
    make_pair (59, "james"),
    make_pair (60, "1_peter"),
    make_pair (61, "2_peter"),
    make_pair (62, "1_john"),
    make_pair (63, "2_john"),
    make_pair (64, "3_john"),
    make_pair (65, "jude"),
    make_pair (66, "revelation")
  };
  return mapping [book];
}


string resource_external_convert_book_netbible (int book)
{
  // Map Bibledit books to net.bible.com books as used at the web service.
  map <int, string> mapping = {
    make_pair (1, "Genesis"),
    make_pair (2, "Exodus"),
    make_pair (3, "Leviticus"),
    make_pair (4, "Numbers"),
    make_pair (5, "Deuteronomy"),
    make_pair (6, "Joshua"),
    make_pair (7, "Judges"),
    make_pair (8, "Ruth"),
    make_pair (9, "1 Samuel"),
    make_pair (10, "2 Samuel"),
    make_pair (11, "1 Kings"),
    make_pair (12, "2 Kings"),
    make_pair (13, "1 Chronicles"),
    make_pair (14, "2 Chronicles"),
    make_pair (15, "Ezra"),
    make_pair (16, "Nehemiah"),
    make_pair (17, "Esther"),
    make_pair (18, "Job"),
    make_pair (19, "Psalms"),
    make_pair (20, "Proverbs"),
    make_pair (21, "Ecclesiastes"),
    make_pair (22, "Song of Solomon"),
    make_pair (23, "Isaiah"),
    make_pair (24, "Jeremiah"),
    make_pair (25, "Lamentations"),
    make_pair (26, "Ezekiel"),
    make_pair (27, "Daniel"),
    make_pair (28, "Hosea"),
    make_pair (29, "Joel"),
    make_pair (30, "Amos"),
    make_pair (31, "Obadiah"),
    make_pair (32, "Jonah"),
    make_pair (33, "Micah"),
    make_pair (34, "Nahum"),
    make_pair (35, "Habakkuk"),
    make_pair (36, "Zephaniah"),
    make_pair (37, "Haggai"),
    make_pair (38, "Zechariah"),
    make_pair (39, "Malachi"),
    make_pair (40, "Matthew"),
    make_pair (41, "Mark"),
    make_pair (42, "Luke"),
    make_pair (43, "John"),
    make_pair (44, "Acts"),
    make_pair (45, "Romans"),
    make_pair (46, "1 Corinthians"),
    make_pair (47, "2 Corinthians"),
    make_pair (48, "Galatians"),
    make_pair (49, "Ephesians"),
    make_pair (50, "Philippians"),
    make_pair (51, "Colossians"),
    make_pair (52, "1 Thessalonians"),
    make_pair (53, "2 Thessalonians"),
    make_pair (54, "1 Timothy"),
    make_pair (55, "2 Timothy"),
    make_pair (56, "Titus"),
    make_pair (57, "Philemon"),
    make_pair (58, "Hebrews"),
    make_pair (59, "James"),
    make_pair (60, "1 Peter"),
    make_pair (61, "2 Peter"),
    make_pair (62, "1 John"),
    make_pair (63, "2 John"),
    make_pair (64, "3 John"),
    make_pair (65, "Jude"),
    make_pair (66, "Revelation")
  };
  return mapping [book];
}


string resource_external_convert_book_blueletterbible (int book)
{
  // This array maps the the book identifiers from Bibledit-web
  // to the book names as used by the blueletterbible.org web service.
  map <int, string> mapping = {
    make_pair (1, "Gen"),
    make_pair (2, "Exd"),
    make_pair (3, "Lev"),
    make_pair (4, "Num"),
    make_pair (5, "Deu"),
    make_pair (6, "Jos"),
    make_pair (7, "Jdg"),
    make_pair (8, "Rth"),
    make_pair (9, "1Sa"),
    make_pair (10, "2Sa"),
    make_pair (11, "1Ki"),
    make_pair (12, "2Ki"),
    make_pair (13, "1Ch"),
    make_pair (14, "2Ch"),
    make_pair (15, "Ezr"),
    make_pair (16, "Neh"),
    make_pair (17, "Est"),
    make_pair (18, "Job"),
    make_pair (19, "Psa"),
    make_pair (20, "Pro"),
    make_pair (21, "Ecc"),
    make_pair (22, "Sgs"),
    make_pair (23, "Isa"),
    make_pair (24, "Jer"),
    make_pair (25, "Lam"),
    make_pair (26, "Eze"),
    make_pair (27, "Dan"),
    make_pair (28, "Hsa"),
    make_pair (29, "Joe"),
    make_pair (30, "Amo"),
    make_pair (31, "Oba"),
    make_pair (32, "Jon"),
    make_pair (33, "Mic"),
    make_pair (34, "Nah"),
    make_pair (35, "Hab"),
    make_pair (36, "Zep"),
    make_pair (37, "Hag"),
    make_pair (38, "Zec"),
    make_pair (39, "Mal"),
    make_pair (40, "Mat"),
    make_pair (41, "Mar"),
    make_pair (42, "Luk"),
    make_pair (43, "Jhn"),
    make_pair (44, "Act"),
    make_pair (45, "Rom"),
    make_pair (46, "1Cr"),
    make_pair (47, "2Cr"),
    make_pair (48, "Gal"),
    make_pair (49, "Eph"),
    make_pair (50, "Phl"),
    make_pair (51, "Col"),
    make_pair (52, "1Th"),
    make_pair (53, "2Th"),
    make_pair (54, "1Ti"),
    make_pair (55, "2Ti"),
    make_pair (56, "Tts"),
    make_pair (57, "Phm"),
    make_pair (58, "Hbr"),
    make_pair (59, "Jam"),
    make_pair (60, "1Pe"),
    make_pair (61, "2Pe"),
    make_pair (62, "1Jo"),
    make_pair (63, "2Jo"),
    make_pair (64, "3Jo"),
    make_pair (65, "Jud"),
    make_pair (66, "Rev")
  };
  return mapping [book];
}


string resource_external_convert_book_bibleserver (int book)
{
  // On bibleserver.com, Genesis is "1.Mose", Exodus is "2.Mose", and so on.
  map <int, string> mapping = {
    make_pair (1, "1.Mose"),
    make_pair (2, "2.Mose"),
    make_pair (3, "3.Mose"),
    make_pair (4, "4.Mose"),
    make_pair (5, "5.Mose"),
    make_pair (6, "Josua"),
    make_pair (7, "Richter"),
    make_pair (8, "Rut"),
    make_pair (9, "1.Samuel"),
    make_pair (10, "2.Samuel"),
    make_pair (11, "1.Könige"),
    make_pair (12, "2.Könige"),
    make_pair (13, "1.Chronik"),
    make_pair (14, "2.Chronik"),
    make_pair (15, "Esra"),
    make_pair (16, "Nehemia"),
    make_pair (17, "Esther"),
    make_pair (18, "Hiob"),
    make_pair (19, "Psalm"),
    make_pair (20, "Sprüche"),
    make_pair (21, "Prediger"),
    make_pair (22, "Hoheslied"),
    make_pair (23, "Jesaja"),
    make_pair (24, "Jeremia"),
    make_pair (25, "Klagelieder"),
    make_pair (26, "Hesekiel"),
    make_pair (27, "Daniel"),
    make_pair (28, "Hosea"),
    make_pair (29, "Joel"),
    make_pair (30, "Amos"),
    make_pair (31, "Obadja"),
    make_pair (32, "Jona"),
    make_pair (33, "Micha"),
    make_pair (34, "Nahum"),
    make_pair (35, "Habakuk"),
    make_pair (36, "Zefanja"),
    make_pair (37, "Haggai"),
    make_pair (38, "Sacharja"),
    make_pair (39, "Maleachi"),
    make_pair (40, "Matthäus"),
    make_pair (41, "Markus"),
    make_pair (42, "Lukas"),
    make_pair (43, "Johannes"),
    make_pair (44, "Apostelgeschichte"),
    make_pair (45, "Römer"),
    make_pair (46, "1.Korinther"),
    make_pair (47, "2.Korinther"),
    make_pair (48, "Galater"),
    make_pair (49, "Epheser"),
    make_pair (50, "Philipper"),
    make_pair (51, "Kolosser"),
    make_pair (52, "1.Thessalonicher"),
    make_pair (53, "2.Thessalonicher"),
    make_pair (54, "1.Timotheus"),
    make_pair (55, "2.Timotheus"),
    make_pair (56, "Titus"),
    make_pair (57, "Philemon"),
    make_pair (58, "Hebräer"),
    make_pair (59, "Jakobus"),
    make_pair (60, "1.Petrus"),
    make_pair (61, "2.Petrus"),
    make_pair (62, "1.Johannes"),
    make_pair (63, "2.Johannes"),
    make_pair (64, "3.Johannes"),
    make_pair (65, "Judas"),
    make_pair (66, "Offenbarung")
  };
  return mapping [book];
}

