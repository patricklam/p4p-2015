/* ECE 459 Tutorial 2
 * Simple HTTP server base
 *
 * Copyright 2015, David Xi Cheng <david.cheng at uwaterloo.ca>
 * All rights reserved.
 */

#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\n"\ 
"Content-Length: 13\r\n"\
"Content-Type: text/html\r\n"\
"Server: SimpleHTTPServer\r\n\r\n"\
"Hello World!"

#include <cstdlib>
#include <cstring>

#include <iostream>

#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class HTTPServerBase{
 protected:
	int port;
	int server_sock;
	struct sockaddr_in server_addr;
	
	int create_bind_listen(bool);
	int create_bind_listen();
	int accept_client();
	int accept_client_nonblock();
	int process_client( int );
	int make_nonblock( int );
 public:
	void shutdown_server();
	virtual void start_serving() = 0;
	HTTPServerBase( int );
};
