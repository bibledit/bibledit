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


#include <webserver/http.h>
#include <bootstrap/bootstrap.h>
#include <webserver/request.h>
#include <config/globals.h>
#include <database/logs.h>
#include <config.h>
#include <webserver/io.h>


void webserver () 
{
  // Setup server behaviour.
  if (strcmp (CLIENT_INSTALLATION, "1") == 0) config_globals_client_prepared = true;
  if (strcmp (OPEN_INSTALLATION, "1") == 0) config_globals_open_installation = true;

  // Server port to listen on.
  unsigned short porti = 8080;

  // Create a listening socket.
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) cerr << "Error opening socket: It returns a descriptor of " << listenfd << endl;

  // Eliminate "Address already in use" error from bind.
  int optval = 1;
  int result = setsockopt (listenfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &optval, sizeof (int));
  if (result != 0) cerr << "Error setting socket options" << endl;

  // The listening socket will be an endpoint for all requests to a port on any IP address for this host.
  typedef struct sockaddr SA;
  struct sockaddr_in serveraddr;
  memset (&serveraddr, 0, sizeof (serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl (INADDR_ANY);
  serveraddr.sin_port = htons (porti);
  // Probably put this in C file and compile with C else it fails. Todo
  result = mybind (listenfd, (SA *) &serveraddr, sizeof (serveraddr));
  if (result != 0) cerr << "Error binding server to socket" << endl;

  // Make it a listening socket ready to accept many connection requests.
  result = listen (listenfd, 100);
  if (result != 0) cerr << "Error listening on socket" << endl;

  // Ignore SIGPIPE signal: When the browser cancels the request, it won't kill Bibledit.
  signal (SIGPIPE, SIG_IGN);

  // Keep waiting for, accepting, and processing connections.
  config_globals_running = true;
  while (config_globals_running) {

    // The environment for this request.
    // It gets passed around from function to function during the entire request.
    // This provides thread-safety to the request.
    Webserver_Request * request = new Webserver_Request ();

    // Socket and file descriptor for the client connection.
    struct sockaddr_in clientaddr;
    socklen_t clientlen = sizeof(clientaddr);
    int connfd = accept(listenfd, (SA *)&clientaddr, &clientlen);

    // The client's remote IPv4 address in dotted notation.
    char remote_address[256];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, remote_address, sizeof(remote_address));
    request->remote_address = remote_address;

    try {
      if (config_globals_running) {
        #define BUFLEN 65535
        // Read the client's request.
        char buffer [BUFLEN];
        memset(&buffer, 0, BUFLEN); // Fix valgrind unitialized value message.
        size_t bytes_read;
        bytes_read = read(connfd, buffer, sizeof(buffer));
        if (bytes_read) {};
        string input = buffer;
  
        // Parse the browser's request's headers.
        http_parse_headers (input, request);
    
        // Assemble response.
        bootstrap_index (request);
        http_assemble_response (request);
    
        // Send response to browser.    
        const char * output = request->reply.c_str();
        // The C function strlen () fails on null characters in the reply, so take string::size()
        size_t length = request->reply.size ();
        int written = write(connfd, output, length);
        if (written) {};
      }

    } catch (exception & e) {
      Database_Logs::log (e.what ());
    } catch (exception * e) {
      Database_Logs::log (e->what ());
    } catch (...) {
      Database_Logs::log ("A general internal error occurred in the web server");
    }

    // Clear memory.
    delete request;

    // Done: Close.
    close(connfd);
  }
}

