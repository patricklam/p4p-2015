#include "http_server_base.h"

class HTTPServerSelect : HTTPServerBase {
 public:
	void start_serving();

	HTTPServerSelect( int port ) : 
		HTTPServerBase( port ) { }
};
