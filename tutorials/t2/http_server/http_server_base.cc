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
	return create_bind_listen( false );
}

int HTTPServerBase::create_bind_listen( bool is_nonblock ){
	//create socket
	server_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( server_sock < 0 ){
		perror( "socket" );
		return -1;
	}
	
	if( is_nonblock )	make_nonblock( server_sock );

	//bind socket with addr
	int ret = bind( server_sock, 
		(struct sockaddr *) &server_addr, sizeof(server_addr) );

	if( ret < 0 ){
		perror( "bind" );
		close( server_sock );
		return -1;
	}

	ret = listen( server_sock, 5 );
	if( ret < 0 ){
		perror( "listen" );
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
		if( errno == EWOULDBLOCK )	return 0;
		
		perror( "accept" );
		return -1;
	}

	return client_sock;
}

int HTTPServerBase::accept_client_nonblock(){
	int client_sock = accept_client();
	if( client_sock <= 0 ){	
		return client_sock;
	}

	make_nonblock( client_sock );

	return client_sock;
}

int HTTPServerBase::process_client( int client_sock ){
	char buf[1024];
	int nb_recv = recv( client_sock, buf, 1024, 0 );
	if( nb_recv == 0 )	return -1;

	if( nb_recv < 0 ) {
		if( errno == EWOULDBLOCK )	return 0;
		
		return -1;
	}

	int nb_send = send( client_sock, HTTP_RESPONSE, sizeof(HTTP_RESPONSE), 0 );
	
	return ( (nb_send > 0) ? 1 : -1 );
}

int HTTPServerBase::make_nonblock( int sockfd ){
	int flags = fcntl( sockfd, F_GETFL, 0 );
	flags |= O_NONBLOCK;
	return fcntl( sockfd, F_SETFL, flags );
}

void HTTPServerBase::shutdown_server() {
	shutdown( server_sock, SHUT_RDWR );
	close( server_sock );
}
