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

#include "http_server_multithread.h"

void HTTPServerMultithread::client_handler( int client_sock ){
	while( true ){
		//either error/remote close
		if( process_client(client_sock) < 0 ){
			close( client_sock );
			break;
		}
	}
}

void HTTPServerMultithread::start_serving(){
	if( create_bind_listen() < 0 ){
		return;
	}

	int client_sock;

	while( true ){
		//accept a client
		client_sock = accept_client();
		
		if( client_sock < 0 ){
			return;
		}

		//create a thread to handle the client
		std::thread t( &HTTPServerMultithread::client_handler, this, client_sock );
		t.detach();
	}
}
