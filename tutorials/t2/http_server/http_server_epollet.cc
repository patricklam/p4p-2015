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

#include "http_server_epoll.h"

void HTTPServerEpoll::start_serving(){
	if( create_bind_listen(true) < 0 ){
		return;
	}
	
	int epfd = epoll_create1( 0 );
	if( epfd == -1 ){
		perror( "epoll_create" );
		return;
	}

	//adding server_sock to the epoll queue
	struct epoll_event ev, events[EPOLL_QUEUE_LEN];
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = server_sock;
	if( epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &ev) < 0 ){
		perror( "epoll_ctl" );
		return;
	}

	int num_fds, client_sock;
	while( true ){	
		num_fds = epoll_wait( epfd, events, EPOLL_QUEUE_LEN, -1 );

		if( num_fds == -1 ){
			perror( "epoll_wait" );
			return;
		}
	
		//fds in events are active
		for( int i = 0; i < num_fds; ++i ) {
			if( events[i].data.fd == server_sock ){
				//server_sock is ready to accept
				std::vector<int> client_socks;

				client_sock = accept_all_nonblock(client_socks);
				if( client_sock < 0 )	return;
				if( client_sock == 0 ) 	continue;
				else{
					for( int client_sock : client_socks ){
						//add client_sock to sock_fds
						ev.events = EPOLLIN | EPOLLET;
						ev.data.fd = client_sock;
						if( epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &ev) < 0 ){
							perror( "epoll_ctl" );
							return;
						}	
					}
				}		
			}

			else{ //client_sock is ready to read
				//either error or remote close
				//remove from epoll queue
				if( process_all( events[i].data.fd ) < 0 ){
					//This doesn't work in kernel version < 2.6.9
					epoll_ctl( epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL );
					close( events[i].data.fd );
				}
			}
		}
	}
}

