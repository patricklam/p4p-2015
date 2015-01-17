#include "http_server_select.h"

void HTTPServerSelect::start_serving(){
	if( create_bind_listen() < 0 ){
		return;
	}

	int client_sock, fdmax;
	fd_set sock_fds, read_fds;
	
	FD_ZERO( &sock_fds );
	FD_ZERO( &read_fds );
	FD_SET( server_sock, &sock_fds );
	fd_max = server_sock;

	while( true ){
		//make a copy of sock_fds
		read_fds = sock_fds;
		
		if( select(fd_max+1, &read_fds, NULL, NULL, NULL) == -1 ){
			std::cerr << "error selecting" << std::endl;
			return;
		}
		
		//read_fds is now modified with only active fds set.
		for( int i = 0; i <= fd_max; ++i ) {
			if( FD_ISSET(i, &read_fds) ){
				if( i == server_sock ){
					//server_sock is ready to accept
					client_sock = accept_client();
					if( client_sock < 0 )	continue;
					else{
						//add client_sock to sock_fds
						FD_SET( client_sock, &sock_fds );
						if( client_sock > fd_max ){	fd_max = client_sock; }
					}
				}
			}

			else{
				//client_sock is ready to read
				process_client( i );
				FD_CLR( i, &sock_fds );	
			}
		}
	}
}
