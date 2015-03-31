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


#include <export/web.h>
#include <export/logic.h>
#include <tasks/logic.h>
#include <database/bibles.h>
#include <database/books.h>
#include <database/logs.h>
#include <database/config/bible.h>
#include <filter/url.h>
#include <filter/string.h>
#include <filter/roles.h>
#include <filter/text.h>
#include <filter/customcss.h>
#include <fonts/logic.h>
#include <html/text.h>
#include <html/header.h>
#include <locale/translate.h>


void export_web (string bible, int book)
{
  Database_Bibles database_bibles;

  
  string directory = Export_Logic::webDirectory (bible);
  if (!file_exists (directory)) filter_url_mkdir (directory);
  
  
  string stylesheet = Database_Config_Bible::getExportStylesheet (bible);
  
  
  // Copy font to the output directory.
  string font = Database_Config_Bible::getTextFont (bible);
  if (!font.empty ()) {
    if (Fonts_Logic::fontExists (font)) {
      string fontpath = Fonts_Logic::getFontPath (font);
      string contents = filter_url_file_get_contents (fontpath);
      fontpath = filter_url_create_path (directory, font);
      filter_url_file_put_contents (fontpath, contents);
    }
  }
  
  
  string backLinkPath = Export_Logic::webBackLinkDirectory (bible);
  
  
  string bibleBookText = bible + " " + Database_Books::getEnglishFromId (book);
  
  
  // Web index file for the book.
  Html_Text html_text_rich_book_index = Html_Text (bibleBookText);
  Html_Header htmlHeader = Html_Header (&html_text_rich_book_index);
  htmlHeader.searchBackLink (backLinkPath + filter_url_html_file_name_bible ("", book), translate("Go back to") + " " + bibleBookText);
  // Todo htmlHeader->create (array (array (bible, filter_url_html_file_name_bible ()), array (Database_Books::getEnglishFromId (book), filter_url_html_file_name_bible ()) ));
  html_text_rich_book_index.newParagraph ("navigationbar");
  html_text_rich_book_index.addText ("|");
  
  
  // Go through the chapters of this book.
  vector <int> chapters = database_bibles.getChapters (bible, book);
  for (auto chapter : chapters) {
    
    // The text filter for this chapter.
    Filter_Text filter_text_chapter = Filter_Text (bible);
    
    string usfm = database_bibles.getChapter (bible, book, chapter);
    usfm = filter_string_trim (usfm);
    // Use small chunks of USFM at a time for much better performance.
    filter_text_chapter.addUsfmCode (usfm);
    
    // Interlinked web data for one chapter.
    filter_text_chapter.html_text_linked = new Html_Text (translate("Bible"));
    filter_text_chapter.html_text_linked->customClass = Filter_CustomCSS::getClass (bible);
    
    // Create breadcrumbs for the chapter.
    Html_Header htmlHeader = Html_Header (filter_text_chapter.html_text_linked);
    htmlHeader.searchBackLink (backLinkPath + filter_url_html_file_name_bible ("", book, chapter), translate("Go back to") + " " + bibleBookText + " " + to_string (chapter));
    // Todo htmlHeader.create (array (array (bible, filter_url_html_file_name_bible ()), array (Database_Books::getEnglishFromId (book), filter_url_html_file_name_bible ()), array (chapter, filter_url_html_file_name_bible ("", book))));
    
    // Create interlinked html for the chapter.
    filter_text_chapter.run (stylesheet);
    filter_text_chapter.html_text_linked->save (filter_url_html_file_name_bible (directory, book, chapter));
    
    html_text_rich_book_index.addLink (html_text_rich_book_index.currentPDomElement, filter_url_html_file_name_bible ("", book, chapter), "", to_string (chapter), "", " " + to_string (chapter) + " ");
    html_text_rich_book_index.addText ("|");
  }
  
  
  // Save the book index. Todo give the index a new name so it sorts at the top of the chapters.
  html_text_rich_book_index.save (filter_url_html_file_name_bible (directory, book));
  
  
  Database_Logs::log (translate("Exported to web") + " " + bible + " " + Export_Logic::baseBookFileName (book), Filter_Roles::translator ());
}
