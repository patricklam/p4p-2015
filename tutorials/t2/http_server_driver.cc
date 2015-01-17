#include <iostream>

#include "http_server_sync.h"
#include "http_server_select.h"
#include "http_server_poll.h"
#include "http_server_epoll.h"

int main( int argc, char* argv[] ){
	if( argc != 2 ){
		std::cerr << "Usage: ./server PORT_NUMBER" << std::endl;
		exit( EXIT_FAILURE );
	}

	int port = std::stoi( argv[1] );

#ifdef SYNC
	HTTPServerSync server( port );
#endif

#ifdef SELECT
	HTTPServerSelect server( port );
#endif

#ifdef POLL
	HTTPServerPoll server( port );
#endif

#ifdef EPOLL
	HTTPServerEpoll server( port );
#endif

	server.start_serving();

	return 0;
}
