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

#ifndef __HTTP_SERVER_MULTITHREAD_H__
#define __HTTP_SERVER_MULTITHREAD_H__

#include <thread>
#include "http_server_base.h"

class HTTPServerMultithread : public HTTPServerBase {
 public:
	void start_serving();
	void client_handler( int );

	HTTPServerMultithread( int port ) : 
		HTTPServerBase( port ) { }
};

#endif
