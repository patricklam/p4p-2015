#include "http_server_base.h"

class HTTPServerSync : public HTTPServerBase {
 public:
	void start_serving();

	HTTPServerSync( int port ) : 
		HTTPServerBase( port ) { }
};
