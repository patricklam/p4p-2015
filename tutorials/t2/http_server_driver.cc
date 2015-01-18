#include <iostream>
#include <string>
#include <memory>

#include <signal.h>

#ifdef SYNC
#include "http_server_sync.h"
static std::unique_ptr<HTTPServerSync> server;
#else
#ifdef SELECT
#include "http_server_select.h"
static std::unique_ptr<HTTPServerSelect> server;
#else
#ifdef POLL
#include "http_server_poll.h"
static std::unique_ptr<HTTPServerPoll> server;
#else
#ifdef EPOLL
#include "http_server_epoll.h"
static std::unique_ptr<HTTPServerEpoll> server;
#endif	//EPOLL
#endif  //POLL
#endif  //SELECT
#endif  //SYNC

void sig_handler( int signum ){
	server->shutdown_server();
	exit(1);
}

int main( int argc, char* argv[] ){
	if( signal(SIGINT, sig_handler) == SIG_ERR ) {
		std::cerr<<"SIGINT Handler error"<<std::endl;
	}

	if( argc != 2 ){
		std::cerr << "Usage: ./server PORT_NUMBER" << std::endl;
		exit( EXIT_FAILURE );
	}

	int port = std::stoi( argv[1] );

#ifdef SYNC
	server.reset( new HTTPServerSync(port) );
#else
#ifdef SELECT
	server.reset( new HTTPServerSelect(port) );
#else
#ifdef POLL
	server.reset( new HTTPServerPoll(port) );
#else
#ifdef EPOLL
	server.reset( new HTTPServerEpoll(port) );
#endif  //EPOLL
#endif  //POLL
#endif  //SELECT
#endif  //SYNC

	server->start_serving();

	return 0;
}
