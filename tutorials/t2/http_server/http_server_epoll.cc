#include "http_server_epoll.h"

void HTTPServerEpoll::start_serving(){
	if( create_bind_listen(true) < 0 ){
		return;
	}
	
	int epfd = epoll_create1( 0 );
	if( epfd == -1 ){
		std::cerr << "error creating epoll" << std::endl;
		return;
	}

	//adding server_sock to the epoll queue
	struct epoll_event ev, events[EPOLL_QUEUE_LEN];
	ev.events = EPOLLIN;
	ev.data.fd = server_sock;
	if( epoll_ctl(epfd, EPOLL_CTL_ADD, server_sock, &ev) < 0 ){
		std::cerr << "error in epoll_ctl" << std::endl;
	}

	int num_fds, client_sock;
	while( true ){	
		num_fds = epoll_wait( epfd, events, EPOLL_QUEUE_LEN, -1 );

		if( num_fds == -1 ){
			std::cerr << "error epolling" << std::endl;
			return;
		}
	
		//fds in events are active
		for( int i = 0; i < num_fds; ++i ) {
			if( events[i].data.fd == server_sock ){
				//server_sock is ready to accept
				client_sock = accept_client();
				if( client_sock < 0 )	return;
				if( client_sock == 0 ) 	continue;
				else{
					//add client_sock to sock_fds
					ev.events = EPOLLIN | EPOLLERR;
					ev.data.fd = client_sock;
					if( epoll_ctl(epfd, EPOLL_CTL_ADD, client_sock, &ev) < 0 ){
						std::cerr << "error in epoll_ctl" << std::endl;
						return;
					}	
				}		
			}

			else{ //client_sock is ready to read
				//either error or remote close
				//remove from epoll queue
				if( process_client( events[i].data.fd ) < 0 ){
					//This doesn't work in kernel version < 2.6.9
					epoll_ctl( epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL );
					close( events[i].data.fd );
				}
			}
		}
	}
}

