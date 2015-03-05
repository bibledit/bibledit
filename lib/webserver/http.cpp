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


#include <webserver/http.h>
#include <vector>
#include <sstream>
#include <filter/url.h>
#include <config/globals.h>
#include <filter/string.h>
#include <parsewebdata/ParseWebData.h>


/*
 The http headers from a browser could look as follows:
 
 GET /index/page HTTP/1.1
 Host: localhost:8080
 Connection: keep-alive
 User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.101 Safari/537.36
 Accept-Language: sn,en-US;q=0.8,en;q=0.6
 
 The function extracts the relevant information from the headers.
 
 It returns true if a header was (or could have been) parsed.
 */
bool http_parse_header (string header, Webserver_Request * request)
{
  // Clean the header line.
  header = filter_string_trim (header);
  
  // Deal with a header like this: GET /css/stylesheet.css?1.0.1 HTTP/1.1
  // Or like this: POST /session/login?request= HTTP/1.1
  bool get = false;
  if (header.substr (0, 3) == "GET") get = true;
  if (header.substr (0, 4) == "POST") {
    get = true;
    request->is_post = true;
  }
  if (get) {
    string query_data;
    vector <string> get = filter_string_explode (header, ' ');
    if (get.size () >= 2) {
      request->get = get [1];
      // The GET or POST value may be, for example: stylesheet.css?1.0.1.
      // Split it up into two parts: The part before the ?, and the part after the ?.
      istringstream issquery (request->get);
      int counter = 0;
      string s;
      while (getline (issquery, s, '?')) {
        if (counter == 0) request->get = s;
        if (counter == 1) query_data = s;
        counter++;
      }
    }
    // Read and parse the GET data.
    try {
      if (!query_data.empty ()) {
        ParseWebData::WebDataMap dataMap;
        ParseWebData::parse_get_data (query_data, dataMap);
        for (ParseWebData::WebDataMap::const_iterator iter = dataMap.begin(); iter != dataMap.end(); ++iter) {
          request->query [(*iter).first] = filter_url_urldecode ((*iter).second.value);
        }
      }
    } catch (...) {
    }
  }
  
  // Extract the User-Agent from a header like this:
  // User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.104 Safari/537.36
  if (header.substr (0, 10) == "User-Agent") {
    request->user_agent = header.substr (12);
  }
  
  // Extract the Accept-Language from a header like this:
  // Accept-Language: sn,en-US;q=0.8,en;q=0.6
  if (header.substr (0, 15) == "Accept-Language") {
    request->accept_language = header.substr (17);
  }
  
  // Extract the Host from a header like this:
  // Host: 192.168.1.139:8080
  if (header.substr (0, 4) == "Host") {
    request->host = header.substr (6);
    vector <string> bits = filter_string_explode (request->host, ':');
    if (!bits.empty ()) request->host = bits [0];
  }
  
  // Extract the Content-Type from a header like this:
  // Content-Type: application/x-www-form-urlencoded
  if (header.substr (0, 12) == "Content-Type") {
    request->content_type = header.substr (14);
  }

  // Extract the Content-Length from a header.
  if (header.substr (0, 14) == "Content-Length") {
    request->content_length = convert_to_int (header.substr (16));
  }
  
  // Something was or could have been parsed if the header contained something.
  return !header.empty ();
}


// Takes data POSTed from the browser, and parses it.
void http_parse_post (string content, Webserver_Request * request)
{
  // Read and parse the POST data.
  try {
    if (!content.empty ()) {
      // Standard parse.
      bool urlencoded = request->content_type.find ("urlencoded") != string::npos;
      ParseWebData::WebDataMap dataMap;
      ParseWebData::parse_post_data (content, request->content_type, dataMap);
      for (ParseWebData::WebDataMap::const_iterator iter = dataMap.begin(); iter != dataMap.end(); ++iter) {
        string value;
        if (urlencoded) value = filter_url_urldecode ((*iter).second.value);
        else value = (*iter).second.value;
        request->post [(*iter).first] = value;
      }
      // Special case: Extract the filename in case of a file upload.
      if (content.length () > 1000) content.resize (1000);
      if (content.find ("filename=") != string::npos) {
        vector <string> lines = filter_string_explode (content, '\n');
        for (auto & line : lines) {
          if (line.find ("Content-Disposition") == string::npos) continue;
          size_t pos = line.find ("filename=");
          if (pos == string::npos) continue;
          line = line.substr (pos + 10);
          line = filter_string_trim (line);
          line.pop_back ();
          request->post ["filename"] = line;
        }
      }
    }
  } catch (...) {
  }
}


/*
The function assembles the response to be given to the browser.
code: an integer response code, normally this is 200.
header: An extra header to be sent with the response. May be empty.
contents: the response body to be sent.
The function inserts the correct headers,
and creates the entire result to be sent back to the browser.
*/
void http_assemble_response (Webserver_Request * request)
{
  ostringstream length;
  length << request->reply.size ();

  // Assemble the HTTP response code fragment.
  string http_response_code_fragment = filter_url_http_response_code_text (request->response_code);
  
  // Assemble the Content-Type.
  string extension = filter_url_get_extension (request->get);
  string content_type;
       if (extension == "js")     content_type = "application/javascript";
  else if (extension == "css")    content_type = "text/css";
  else if (extension == "ico")    content_type = "image/vnd.microsoft.icon";
  else if (extension == "gif")    content_type = "image/gif";
  else if (extension == "jpg")    content_type = "image/jpeg";
  else if (extension == "jpeg")   content_type = "image/jpeg";
  else if (extension == "jpeg")   content_type = "image/png";
  else if (extension == "svg")    content_type = "image/svg+xml";
  else if (extension == "txt")    content_type = "text/plain";
  else if (extension == "otf")    content_type = "font/opentype";
  else if (extension == "ttf")    content_type = "application/x-font-ttf";
  else if (extension == "sqlite") content_type = "application/octet-stream";
  else                            content_type = "text/html";

  // Assemble the complete response for the browser.
  vector <string> response;
  response.push_back ("HTTP/1.1 " + http_response_code_fragment);
  response.push_back ("Accept-Ranges: bytes");
  response.push_back ("Content-Length: " + length.str());
  response.push_back ("Content-Type: " + content_type);
  if (!request->header.empty ()) response.push_back (request->header);
  response.push_back ("");
  if (!request->reply.empty ()) response.push_back (request->reply);

  string assembly;
  for (unsigned int i = 0; i < response.size (); i++) {
    if (i > 0) assembly += "\n";
    assembly += response [i];
  }
  request->reply = assembly;
}


// This function serves a file.
void http_serve_file (Webserver_Request * request)
{
  // Full path to the file.
  string filename = filter_url_create_root_path (filter_url_urldecode (request->get));
  
  // Get file's contents.
  request->reply = filter_url_file_get_contents (filename);
}
