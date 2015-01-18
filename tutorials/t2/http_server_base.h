/* ECE 459 Tutorial 2
 * Simple HTTP server base
 *
 * Copyright 2015, David Xi Cheng <david.cheng at uwaterloo.ca>
 * All rights reserved.
 */

#define HTTP_RESPONSE "HTTP/1.0 200 OK\r\n"\ 
"Content-Type: text/html; charset=UTF-8\r\n"\
"Server: SimpleHTTPServer\r\n"\
"Hello World!\r\n"

#include <cstdlib>
#include <cstring>

#include <iostream>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class HTTPServerBase{
 protected:
	int port;
	int server_sock;
	struct sockaddr_in server_addr;
	
	int create_bind_listen();
	int accept_client();
	int process_client( int );

 public:
	void shutdown_server();
	virtual void start_serving() = 0;
	HTTPServerBase( int );
};
