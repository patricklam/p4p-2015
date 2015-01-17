#include "http_server_epoll.h"

void HTTPServerEpoll::start_serving(){
	if( create_bind_listen() < 0 ){
		return;
	}
	
	int epfd = epoll_create( EPOLL_QUEUE_LEN );
	if( epfd == -1 ){
		std::cerr << "error creating epoll" << std::endl;
		return;
	}

	//adding server_sock to the epoll queue
	struct epoll_event ev, events[EPOLL_QUEUE_LEN];
	ev.events = EPOLLIN | EPOLLERR;
	ev.data.fd = server_sock;
	epoll.ctl( epfd, EPOLL_CTL_ADD, server_sock, &ev );

	int num_fds, client_sock;
	while( true ){	
		num_fds = epoll_wait( epfd, events, EPOLL_QUEUE_LEN, -1 );

		if( ret == -1 ){
			std::cerr << "error epolling" << std::endl;
			return;
		}
	
		//fds in events are active
		for( int i = 0; i < num_fds; ++i ) {
			if( events[i].data.fd == server_sock ){
				//server_sock is ready to accept
				client_sock = accept_client();
				if( client_sock < 0 )	continue;
				else{
					//add client_sock to sock_fds
					ev.events = EPOLLIN | EPOLLERR;
					ev.data.fd = client_sock;
					epoll_ctl( epollfd, EPOLL_CTL_ADD, client_sock, &ev );	
				}		
			}

			else{
				//client_sock is ready to read
				process_client( events[i].data.fd );
				//This doesn't work in kernel version < 2.6.9
				epoll_ctl( epollfd, EPOLL_CTL_DEL, events[i].data.fd, NULL );
			}
		}
	}
}

