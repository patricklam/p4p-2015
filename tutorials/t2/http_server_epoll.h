#include <sys/epoll.h>

#include "http_server_base.h"

#define EPOLL_QUEUE_LEN 256

class HTTPServerEpoll : public HTTPServerBase {
 public:
	void start_serving();

	HTTPServerEpoll( int port ) : 
		HTTPServerBase( port ) { }
};
