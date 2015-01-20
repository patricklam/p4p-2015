#include "http_server_select.h"

void HTTPServerSelect::start_serving(){
	if( create_bind_listen(true) < 0 ){
		return;
	}

	int client_sock, fd_max;
	fd_set sock_fds, read_fds;
	
	FD_ZERO( &sock_fds );
	FD_ZERO( &read_fds );
	FD_SET( server_sock, &sock_fds );
	fd_max = server_sock;

	while( true ){
		//make a copy of sock_fds
		read_fds = sock_fds;
		
		if( select(fd_max+1, &read_fds, NULL, NULL, NULL) < 0 ){
			perror( "select" );
			return;
		}
		
		//read_fds is now modified with only active fds set.
		for( int i = 0; i <= fd_max; ++i ) {
			if( FD_ISSET(i, &read_fds) ){
				if( i == server_sock ){
					//server_sock is ready to accept
					client_sock = accept_client_nonblock();
					if( client_sock < 0 )	return;
					if( client_sock == 0 )	continue;
					else{
						//add client_sock to sock_fds
						FD_SET( client_sock, &sock_fds );
						if( client_sock > fd_max ){ fd_max = client_sock; }
					}
				}

				else{
					//either error / remote close
					//remove from select queue
					if( process_client( i ) < 0 ){
						FD_CLR( i, &sock_fds );
						close( i );
					}	
				}
			}
		}
	}
}
