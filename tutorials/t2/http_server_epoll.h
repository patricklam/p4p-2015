#include <sys/epoll.h>

#include "http_server_base.h"

class HTTPServerEpoll : HTTPServerBase {
 public:
	void start_serving();

	HTTPServerEpoll( int port ) : 
		HTTPServerBase( port ) { }
};
