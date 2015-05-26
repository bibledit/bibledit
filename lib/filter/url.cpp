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


#include <filter/url.h>
#include <webserver/http.h>
#include <config/globals.h>
#include <filter/UriCodec.cpp>
#include <filter/string.h>
#include <filter/date.h>
#include <config/logic.h>
#include <config.h>
#include <database/books.h>
#include <database/logs.h>


// Gets the base URL of current Bibledit installation.
string get_base_url (Webserver_Request * request)
{
  // E.g. http or https: Always use http for just now.
  string scheme = "http";  
  // Port
  string port = config_logic_network_port ();
  // Full URL.  
  string url = scheme + "://" + request->host + ":" + port + "/";
  return url;
}


// This function redirects the browser to "path".
// "path" is an absolute value.
void redirect_browser (Webserver_Request * request, string path)
{
  // A location header needs to contain an absolute url, like http://localhost/some.
  // See 14.30 in the specification http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html.
  string location = get_base_url (request) + path;
  request->header = "Location: " + location;
  request->response_code = 302;
}


// C++ replacement for the dirname function, see http://linux.die.net/man/3/dirname.
string filter_url_dirname (string url)
{
  if (!url.empty ()) {
    if (url.find_last_of (DIRECTORY_SEPARATOR) == url.length () - 1) {
      // Remove trailing slash.
      url = url.substr (0, url.length () - 1);
    }
    size_t pos = url.find_last_of (DIRECTORY_SEPARATOR);
    if (pos != string::npos) url = url.substr (0, pos);
    else url = "";
  }
  if (url.empty ()) url = ".";
  return url;
}


// C++ replacement for the basename function, see http://linux.die.net/man/3/basename.
string filter_url_basename (string url)
{
  if (!url.empty ()) {
    if (url.find_last_of (DIRECTORY_SEPARATOR) == url.length () - 1) {
      // Remove trailing slash.
      url = url.substr (0, url.length () - 1);
    }
    size_t pos = url.find_last_of (DIRECTORY_SEPARATOR);
    if (pos != string::npos) url = url.substr (pos + 1);
  }
  return url;
}


void filter_url_unlink (string filename)
{
  unlink (filename.c_str ());
}


void filter_url_rename (const string& oldfilename, const string& newfilename)
{
  rename (oldfilename.c_str(), newfilename.c_str());
}


// Creates a file path out of the components.
string filter_url_create_path (string part1, string part2, string part3, string part4, string part5, string part6)
{
  string path (part1);
  if (part2.length()) path += DIRECTORY_SEPARATOR + part2;
  if (part3.length()) path += DIRECTORY_SEPARATOR + part3;
  if (part4.length()) path += DIRECTORY_SEPARATOR + part4;
  if (part5.length()) path += DIRECTORY_SEPARATOR + part5;
  if (part6.length()) path += DIRECTORY_SEPARATOR + part6;
  return path;
}


// Creates a file path out of the variable list of components, relative to the server's document root.
string filter_url_create_root_path (string part1, string part2, string part3, string part4, string part5)
{
  string path = config_globals_document_root;
  if (part1.length()) path += DIRECTORY_SEPARATOR + part1;
  if (part2.length()) path += DIRECTORY_SEPARATOR + part2;
  if (part3.length()) path += DIRECTORY_SEPARATOR + part3;
  if (part4.length()) path += DIRECTORY_SEPARATOR + part4;
  if (part5.length()) path += DIRECTORY_SEPARATOR + part5;
  return path;
}


// Gets the file / url extension, e.g. /home/joe/file.txt returns "txt".
string filter_url_get_extension (string url)
{
  string extension;
  size_t pos = url.find_last_of (".");
  if (pos != string::npos) {
    extension = url.substr (pos + 1);
  }
  return extension;
}


// Returns true if the file in "url" exists.
bool file_exists (string url)
{
  struct stat buffer;   
  return (stat (url.c_str(), &buffer) == 0);
}


// Wrapper for the mkdir function: make a directory.
// Creates parents where needed.
void filter_url_mkdir (string directory)
{
  int status;
#ifdef WIN32
	status = _mkdir(directory.c_str());
#else
  status = mkdir(directory.c_str(), 0777);
#endif
  if (status != 0) {
    vector <string> paths;
    paths.push_back (directory);
    directory = filter_url_dirname (directory);
    while (directory.length () > 2) {
      paths.push_back (directory);
      directory = filter_url_dirname (directory);
    }
    reverse (paths.begin (), paths.end ());
    for (unsigned int i = 0; i < paths.size (); i++) {
#ifdef WIN32
		_mkdir(paths[i].c_str());
#else
		mkdir(paths[i].c_str(), 0777);
#endif
    }
  }
}


// Removes directory recursively.
void filter_url_rmdir (string directory)
{
  DIR *dir;
  struct dirent *entry;
  char path[PATH_MAX];
  dir = opendir(directory.c_str());
  if (dir == NULL) return;
  while ((entry = readdir (dir)) != NULL) {
    if (strcmp (entry->d_name, ".") && strcmp (entry->d_name, "..")) {
      snprintf (path, (size_t) PATH_MAX, "%s/%s", directory.c_str(), entry->d_name);
      if (entry->d_type == DT_DIR) {
        filter_url_rmdir (path);
      }
      remove (path);
    }
  }
  closedir(dir);
  remove (directory.c_str());
}


// Returns true is $path points to a directory.
bool filter_url_is_dir (string path)
{
  struct stat sb;
  stat (path.c_str(), &sb);
  return (sb.st_mode & S_IFMT) == S_IFDIR;
}


bool filter_url_get_write_permission (string path)
{
  int result = access (path.c_str(), W_OK);
  if (result == 0) return true;
  return false;
}


void filter_url_set_write_permission (string path)
{
  chmod (path.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
}


// C++ rough equivalent for PHP'sfilter_url_file_get_contents.
string filter_url_file_get_contents (string filename)
{
  if (!file_exists (filename)) return "";
  try {
    ifstream ifs (filename.c_str(), ios::in | ios::binary | ios::ate);
    streamoff filesize = ifs.tellg();
    ifs.seekg (0, ios::beg);
    vector <char> bytes ((int)filesize);
    ifs.read (&bytes[0], (int)filesize);
    return string (&bytes[0], (int)filesize);
  } catch (...) {
    return "";
  }
}


// C++ rough equivalent for PHP'sfilter_url_file_put_contents.
void filter_url_file_put_contents (string filename, string contents)
{
  try {
    ofstream file;  
    file.open (filename, ios::binary | ios::trunc);
    file << contents;
    file.close ();
  } catch (...) {
  }
}


// C++ rough equivalent for PHP'sfilter_url_file_put_contents.
// Appends the data if the file exists.
void filter_url_file_put_contents_append (string filename, string contents)
{
  try {
    ofstream file;  
    file.open (filename, ios::binary | ios::app);  
    file << contents;
    file.close ();
  } catch (...) {
  }
}


// A C++ equivalent for PHP's filesize function.
int filter_url_filesize (string filename)
{
  struct stat stat_buf;
  int rc = stat (filename.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : 0;
}


// A C++ near equivalent for PHP's scandir function.
vector <string> filter_url_scandir (string folder)
{
  vector <string> files;
  DIR * dir = opendir (folder.c_str());
  if (dir) {
    struct dirent * direntry;
    while ((direntry = readdir (dir)) != NULL) {
      string name = direntry->d_name;
      if (name.substr (0, 1) == ".") continue;
      if (name == "gitflag") continue;
      files.push_back (name);
    }
    closedir (dir);
  }
  sort (files.begin(), files.end());
  return files;
}


// Recursively scans a directory for directories and files.
void filter_url_recursive_scandir (string folder, vector <string> & paths)
{
  vector <string> files = filter_url_scandir (folder);
  for (auto & file : files) {
    string path = filter_url_create_path (folder, file);
    paths.push_back (path);
    if (filter_url_is_dir (path)) {
      filter_url_recursive_scandir (path, paths);
    }
  }
}


// A C++ near equivalent for PHP's filemtime function.
int filter_url_filemtime (string filename)
{
  struct stat attributes;
  stat (filename.c_str(), &attributes);
  return (int) attributes.st_mtime;
}


// A C++ near equivalent for PHP's urldecode function.
string filter_url_urldecode (string url)
{
  url = UriDecode (url);
  replace (url.begin (), url.end (), '+', ' ');
  return url;
}


// A C++ near equivalent for PHP's urlencode function.
string filter_url_urlencode (string url)
{
  url = UriEncode (url);
  return url;
}


// Returns the name of a temporary file.
string filter_url_tempfile (const char * directory)
{
  string filename = convert_to_string (filter_date_seconds_since_epoch ()) + convert_to_string (filter_date_numerical_microseconds ());
  if (directory) {
    filename = filter_url_create_path (directory, filename);
  } else {
    filename = filter_url_create_root_path ("tmp", filename);
  }
  return filename;
}


// C++ equivalent for PHP's escapeshellarg function.
string filter_url_escape_shell_argument (string argument)
{
  argument = filter_string_str_replace ("'", "\\'", argument);
  argument.insert (0, "'");
  argument.append ("'");
  return argument;
}


// The function accepts a $path.
// The function may add a numerical suffix 
// to ensure that the $path does not yet exist in the filesystem.
string filter_url_unique_path (string path)
{
  if (!file_exists (path)) return path;
  for (unsigned int i = 1; i < 100; i++) {
    string uniquepath = path + "." + convert_to_string (i);
    if (!file_exists (uniquepath)) return uniquepath;
  }
  return path + "." + convert_to_string (filter_string_rand (100, 1000));
}


// Returns true if the email address is valid.
bool filter_url_email_is_valid (string email)
{
  const string valid_set ("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._-");
  // The @ character should appear only once.
  vector <string> atbits = filter_string_explode (email, '@');
  if (atbits.size() != 2) return false;
  // The characters on the left of @ should be from the valid set.
  string left = atbits [0];
  for (unsigned int i = 0; i < left.size(); i++) {
    char c = left [i];
    if (valid_set.find (c) == string::npos) return false;
  }
  // The characters on the right of @ should be from the valid set.
  string right = atbits [1];
  for (unsigned int i = 0; i < right.size(); i++) {
    char c = right [i];
    if (valid_set.find (c) == string::npos) return false;
  }
  // The character . should appear at least once to the right of @.
  vector <string> dotbits = filter_string_explode (right, '.');
  if (dotbits.size () < 2) return false;
  // The email address is valid.
  return true;
}


string filter_url_build_http_query (string url, const string& parameter, const string& value)
{
  size_t pos = url.find ("?");
  if (pos == string::npos) url.append ("?");
  else url.append ("&");
  url.append (parameter);
  url.append ("=");
  url.append (value);
  return url;
}


size_t filter_url_curl_write_function (void *ptr, size_t size, size_t count, void *stream)
{
  ((string *) stream)->append ((char *) ptr, 0, size * count);
  return size * count;
}


// Sends a http GET request to the $url.
// It returns the response from the server.
// It writes any error to $error.
string filter_url_http_get (string url, string& error) // Todo alternative w/o cURL
{
  string response;
#ifdef HAVE_EMBEDDEDHTTP
#else
#endif
  CURL *curl = curl_easy_init ();
  if (curl) {
    curl_easy_setopt (curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, filter_url_curl_write_function);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L); // Todo
    // Because a Bibledit client should work even over very bad networks,
    // pass some timeout options to curl so it properly deals with such networks.
    filter_url_curl_set_timeout (curl);
    CURLcode res = curl_easy_perform (curl);
    if (res == CURLE_OK) {
      error.clear ();
      long http_code = 0;
      curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
      if (http_code != 200) {
        response.append ("http code " + convert_to_string ((int)http_code));
      }
    } else {
      response.clear ();
      error = curl_easy_strerror (res);
    }
    curl_easy_cleanup (curl);
  }
  return response;
}


// Sends a http POST request to $url.
// burst: Set connection timing for burst mode, where the response comes after a relatively long silence.
// It posts the $values.
// It returns the response from the server.
// It writes any error to $error.
string filter_url_http_post (string url, map <string, string> values, string& error, bool burst) // Todo alternative w/o cURL
{
  string response;
  // Get a curl handle.
  CURL *curl = curl_easy_init ();
  if (curl) {
    // First set the URL that is about to receive the POST.
    // This can be http or https.
    curl_easy_setopt (curl, CURLOPT_URL, url.c_str());
    // Generate the post data.
    string postdata;
    for (auto & element : values) {
      if (!postdata.empty ()) postdata.append ("&");
      postdata.append (element.first);
      postdata.append ("=");
      postdata.append (filter_url_urlencode (element.second));
    }
    // Specify the POST data to curl, e.g.: "name=foo&project=bar"
    curl_easy_setopt (curl, CURLOPT_POSTFIELDS, postdata.c_str());
    // Callback for the server response.
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, filter_url_curl_write_function);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, &response);
    // Further options.
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
    // Timeouts for very bad networks, see the GET routine above for an explanation.
    filter_url_curl_set_timeout (curl, burst);
    // Perform the request.
    CURLcode res = curl_easy_perform (curl);
    // Result check.
    if (res == CURLE_OK) {
      error.clear ();
      long http_code = 0;
      curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
      if (http_code != 200) {
        error.append ("Server response " + filter_url_http_response_code_text (http_code));
      }
    } else {
      error = curl_easy_strerror (res);
    }
    // Always cleanup.
    curl_easy_cleanup (curl);
  }
  return response;
}


string filter_url_http_response_code_text (int code)
{
  string text = convert_to_string (code);
  text.append (" ");
  switch (code) {
    case 100: text += "Continue"; break;
    case 101: text += "Switching Protocols"; break;
    case 200: text += "OK"; break;
    case 201: text += "Created"; break;
    case 202: text += "Accepted"; break;
    case 203: text += "Non-Authoritative Information"; break;
    case 204: text += "No Content"; break;
    case 205: text += "Reset Content"; break;
    case 206: text += "Partial Content"; break;
    case 300: text += "Multiple Choices"; break;
    case 301: text += "Moved Permanently"; break;
    case 302: text += "Found"; break;
    case 303: text += "See Other"; break;
    case 304: text += "Not Modified"; break;
    case 305: text += "Use Proxy"; break;
    case 307: text += "Temporary Redirect"; break;
    case 308: text += "Permanent Redirect"; break;
    case 400: text += "Bad Request"; break;
    case 401: text += "Unauthorized"; break;
    case 402: text += "Payment Required"; break;
    case 403: text += "Forbidden"; break;
    case 404: text += "Not Found"; break;
    case 405: text += "Method Not Allowed"; break;
    case 406: text += "Not Acceptable"; break;
    case 407: text += "Proxy Authentication Required"; break;
    case 408: text += "Request Timeout"; break;
    case 409: text += "Conflict"; break;
    case 410: text += "Gone"; break;
    case 411: text += "Length Required"; break;
    case 412: text += "Precondition Failed"; break;
    case 413: text += "Request Entity Too Large"; break;
    case 414: text += "Request-URI Too Long"; break;
    case 415: text += "Unsupported Media Type"; break;
    case 416: text += "Requested Range Not Satisfiable"; break;
    case 417: text += "Expectation Failed"; break;
    case 426: text += "Upgrade Required"; break;
    case 428: text += "Precondition Required"; break;
    case 429: text += "Too Many Requests"; break;
    case 431: text += "Request Header Fields Too Large"; break;
    case 500: text += "Internal Server Error"; break;
    case 501: text += "Not Implemented"; break;
    case 502: text += "Bad Gateway"; break;
    case 503: text += "Service Unavailable"; break;
    case 504: text += "Gateway Timeout"; break;
    case 505: text += "HTTP Version Not Supported"; break;
    case 511: text += "Network Authentication Required"; break;
    default:  text += "Error"; break;
  }
  return text;
}


// Downloads the file at $url, and stores it at $filename.
void filter_url_download_file (string url, string filename, string& error) // Todo alternative w/o cURL
{
  CURL *curl = curl_easy_init ();
  if (curl) {
    curl_easy_setopt (curl, CURLOPT_URL, url.c_str());
    FILE* file = fopen (filename.c_str(), "w");
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt (curl, CURLOPT_FOLLOWLOCATION, 1L);
    // curl_easy_setopt (curl, CURLOPT_VERBOSE, 1L);
    filter_url_curl_set_timeout (curl);
    CURLcode res = curl_easy_perform (curl);
    if (res == CURLE_OK) {
      error.clear ();
      long http_code = 0;
      curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
      if (http_code != 200) {
        error.append ("http code " + convert_to_string ((int)http_code));
      }
    } else {
      error = curl_easy_strerror (res);
    }
    curl_easy_cleanup (curl);
    fclose (file);
  }
}


/*
 * The function returns the filename for a html file for a Bible.
 * $path    - The path where to store the files.
 * $book    - The book identifier.
 * $chapter - The chapter number.
 */
string filter_url_html_file_name_bible (string path, int book, int chapter)
{
  string filename;
  
  // If a path is given, prefix it.
  if (path != "") {
    filename = path + "/";
  }
  
  // No book ID given: Return the name for the index file for the Bible.
  if (book == 0) {
    filename += "index.html";
    return filename;
  }
  
  // Add the name for the book. No spaces.
  filename += filter_string_fill (convert_to_string (book), 2, '0');
  string sbook = Database_Books::getEnglishFromId (book);
  sbook = filter_string_str_replace (" ", "", sbook);
  filename += '-' + sbook;
  
  // Chapter given: Provide name for the chaper.
  if (chapter >= 0) {
    filename += '-' + filter_string_fill (convert_to_string (chapter), 3, '0');
  }
  
  filename += ".html";
  
  return filename;
}


// Callback function for logging cURL debug information.
int filter_url_curl_debug_callback (CURL *handle, curl_infotype type, char *data, size_t size, void *userptr)
{
  if (handle && userptr) {};
  bool log = true;
  if (type == CURLINFO_SSL_DATA_OUT) log = false;
  if (type == CURLINFO_SSL_DATA_OUT) log = false;
  if (log) {
    string message (data, size);
    Database_Logs::log (message);
  }
  return 0;
}


// Sets timeouts for cURL operations.
// burst: When true, the server gives a burst response, that is, all data arrives at once after a delay.
//        When false, the data is supposed to be downloaded gradually.
// Without these timeouts, the Bibledit client will hang on stalled sync operations.
void filter_url_curl_set_timeout (CURL *handle, bool burst)
{
  // There is a timeout on establishing a connection.
  curl_easy_setopt (handle, CURLOPT_CONNECTTIMEOUT, 10);

  // There is a also a transfer timeout for normal speeds.
  curl_easy_setopt (handle, CURLOPT_TIMEOUT, 600);
  
  // There is also a shorter transfer timeout for low speeds,
  // because low speeds indicate a stalled connection.
  // But when the server needs to do a lot of processing and then sends then response at once,
  // the low speed timeouts should be disabled,
  // else it times out before the response has come.
  if (!burst) {
    curl_easy_setopt (handle, CURLOPT_LOW_SPEED_LIMIT, 100);
    curl_easy_setopt (handle, CURLOPT_LOW_SPEED_TIME, 10);
  }
  
  // Timing out may use signals, which is not what we want.
  curl_easy_setopt (handle, CURLOPT_NOSIGNAL, 1L);
}
