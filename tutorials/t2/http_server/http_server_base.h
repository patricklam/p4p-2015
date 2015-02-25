/* ECE 459 Tutorial 2
 * A simple HTTP Server implementation to 
 * demostrate the usage/performance of asynchronous multiplexing.
 *
 * A simply working version that does NOT care about request headers
 * Send "Hello World" back WHENEVER a request is received
 * Does NOT support HTTP 1.1 pipelining
 * Use with httperf for benchmarking purposes
 *
 * Copyright 2015, David Xi Cheng <david.cheng at uwaterloo.ca>
 * All rights reserved.
 */

#ifndef __HTTP_SERVER_BASE_H__
#define __HTTP_SERVER_BASE_H__

#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\n"\
"Content-Length: 13\r\n"\
"Content-Type: text/html\r\n"\
"Connection: keep-alive\r\n"\
"Server: SimpleHTTPServer\r\n\r\n"\
"Hello World!"

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <vector>

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
	int accept_client() const;
	int accept_client_nonblock() const;
	int accept_all_nonblock( std::vector<int>& ) const;
	int process_client( int ) const;
	int process_all( int ) const;
	int make_nonblock( int ) const;
 public:
	void shutdown_server();
	virtual void start_serving() = 0;
	HTTPServerBase( int );
};

#endif
