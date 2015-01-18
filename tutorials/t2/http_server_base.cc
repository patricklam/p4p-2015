#include "http_server_base.h"

HTTPServerBase::HTTPServerBase( int port ) : 
	port( port ) {
	//init server addr struct
	memset( &server_addr, 0, sizeof(server_addr) );
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons( port );
	server_addr.sin_addr.s_addr = htonl( INADDR_ANY );
}

int HTTPServerBase::create_bind_listen(){
	//create socket
	server_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( server_sock < 0 ){
		std::cerr << "error creating server socket" << std::endl;
		return -1;
	}

	//bind socket with addr
	int ret = bind( server_sock, 
		(struct sockaddr *) &server_addr, sizeof(server_addr) );

	if( ret < 0 ){
		std::cerr << "error binding server socket" << std::endl;
		close( server_sock );
		return -1;
	}

	ret = listen( server_sock, 5 );
	if( ret < 0 ){
		std::cerr << "error listening on server socket" << std::endl;
		close( server_sock );
		return -1;
	}

	std::cout << "listening on port " << port << std::endl;
	return 1;
}

int HTTPServerBase::accept_client() {
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof( client_addr );

	//accept a client
	int client_sock = accept( server_sock, 
		(struct sockaddr *) &client_addr, &client_addr_len );

	if( client_sock < 0 ){
		std::cerr << "error accepting client" << std::endl;
		return -1;
	}

	return client_sock;
}

int HTTPServerBase::process_client( int client_sock ){
	char buf[1024];
	recv( client_sock, buf, 1024, 0 );
	send( client_sock, HTTP_RESPONSE, sizeof(HTTP_RESPONSE), 0 );
	
	//only support HTTP 1.0, close socket after one round-trip.	
	close( client_sock );

	return 0;
}

void HTTPServerBase::shutdown_server() {
	shutdown( server_sock, SHUT_RDWR );
	close( server_sock );
}
