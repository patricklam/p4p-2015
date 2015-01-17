#include "http_server_base.h"

class HTTPServerSync : HTTPServerBase {
 public:
	void start_serving();

	HTTPServerBlock( int port ) : 
		HTTPServerBase( port ) { }
};
