#include "http_server_poll.h"

void HTTPServerPoll::start_serving(){
	if( create_bind_listen(true) < 0 ){
		return;
	}
	
	std::vector<struct pollfd> sock_fds;

	struct pollfd server_pfd;
	server_pfd.fd = server_sock;
	server_pfd.events = POLLIN;
	
	sock_fds.push_back( server_pfd );

	int client_sock;
	while( true ){	
		int ret = poll( &sock_fds[0], sock_fds.size(), -1 );

		if( ret == -1 ){
			std::cerr << "error polling" << std::endl;
			return;
		}
	
		//now check for sock_fds
		for( auto it = sock_fds.begin(); it != sock_fds.end(); ++it ) {
			//we only care about fds that are ready to read
			if( !((*it).revents & POLLIN) )	continue; 

			if( (*it).fd == server_sock ){
				//server_sock is ready to accept
				client_sock = accept_client_nonblock();
				if( client_sock < 0 )	return;
				if( client_sock == 0 )	continue;
				else{
					//add client_sock to sock_fds
					struct pollfd client_pfd;
					client_pfd.fd = client_sock;
					client_pfd.events = POLLIN;
					sock_fds.push_back( client_pfd );
				}		
			}

			else{ //client_sock is ready to read
				
				//either error / remote close
				//remove from poll queue	
				if( process_client( (*it).fd ) < 0 ){
					close( (*it).fd );
					//delete the fd from vector
					std::iter_swap( sock_fds.end()-1, it );
					sock_fds.pop_back();

					if( it == sock_fds.end() )	break;
				}
			}
		}
	}
}
